/* Hey EMACS -*- linux-c -*- */
/* $Id: cmd84p.c 2077 2006-03-31 21:16:19Z roms $ */

/*  libticalcs - Ti Calculator library, a part of the TiLP project
 *  Copyright (C) 1999-2005  Romain Li�vin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
	This unit manages virtual packets from/to D-USB (DirectLink).
	Virtual packets are fragmented into one or more raw packets.
	The size of packet can be negotiated on both sides.
*/

#include <stdlib.h>
#include <string.h>

#include "ticalcs.h"
#include "logging.h"
#include "error.h"
#include "macros.h"

#include "dusb_rpkt.h"
#include "dusb_vpkt.h"

#define VPKT_DBG	1	// 1 = verbose, 2 = more verbose

// Pseudo-Constants

static unsigned int DATA_SIZE = 250;	// max length of data in raw packet

// Type to string

static const VtlPktName vpkt_types[] = 
{
	{ 0x0000, ""},
	{ 0x0001, "Ping / Set Mode"},
	{ 0x0002, "Begin OS Transfer"},
	{ 0x0003, "Acknowledgement of OS Transfer"},
	{ 0x0004, "OS Header"},
	{ 0x0005, "OS Data"},
	{ 0x0006, "Acknowledgement of EOT"},
	{ 0x0007, "Parameter Request"},
	{ 0x0008, "Parameter Data"},
	{ 0x0009, "Request Directory Listing"},
	{ 0x000A, "Variable Header"},
	{ 0x000B, "Request to Send"},
	{ 0x000C, "Request Variable"},
	{ 0x000D, "Variable Contents"},
	{ 0x000E, "Parameter Set"},
	{ 0x000F, ""},
	{ 0x0010, "Delete Variable"},
	{ 0x0011, "Remote Control"},
	{ 0x0012, "Acknowledgement of Mode Setting"},
	{ 0xAA00, "Acknowledgement of Data"},
	{ 0xBB00, "Delay Acknowledgment"},
	{ 0xDD00, "End of Transmission"},
	{ 0xEE00, "Error"},
	{ -1, NULL},
};

const char* dusb_vpkt_type2name(uint16_t id)
{
	const VtlPktName *p;

	for(p = vpkt_types; p->name != NULL; p++)
		if(p->id == id)
			return p->name;

	return "unknown: not listed";
}

// Buffer allocation

static GList *vtl_pkt_list = NULL;

VirtualPacket*  dusb_vtl_pkt_new(uint32_t size, uint16_t type)
{
	VirtualPacket* vtl = g_malloc0(sizeof(VirtualPacket));

	vtl->size = size;
	vtl->type = type;
	vtl->data = g_malloc0(size + DH_SIZE);

	vtl_pkt_list = g_list_append(vtl_pkt_list, vtl);
	return vtl;
}

void			dusb_vtl_pkt_del(VirtualPacket* vtl)
{
	vtl_pkt_list = g_list_remove(vtl_pkt_list, vtl);

	g_free(vtl->data);
	g_free(vtl);
}

void			dusb_vtl_pkt_purge(void)
{
	g_list_foreach(vtl_pkt_list, (GFunc)dusb_vtl_pkt_del, NULL);
	g_list_free(vtl_pkt_list);
	vtl_pkt_list = NULL;
}

// Raw packets

int dusb_send_buf_size_request(CalcHandle* h, uint32_t size)
{
	RawPacket raw = { 0 };

	raw.size = 4;
	raw.type = RPKT_BUF_SIZE_REQ;
	raw.data[2] = MSB(size);
	raw.data[3] = LSB(size);

	TRYF(dusb_send(h, &raw));
	ticalcs_info("  PC->TI: Buffer Size Request (%i bytes)", size);

	return 0;
}

int dusb_recv_buf_size_alloc(CalcHandle* h, uint32_t *size_)
{	
	RawPacket raw = { 0 };
	uint32_t size;

	TRYF(dusb_recv(h, &raw));
	
	if(raw.size != 4)
		return ERR_INVALID_PACKET;

	if(raw.type != RPKT_BUF_SIZE_ALLOC)
		return ERR_INVALID_PACKET;

	size = (raw.data[0] << 24) | (raw.data[1] << 16) | (raw.data[2] << 8) | (raw.data[3] << 0);
	if(size_)
		*size_ = size;
	ticalcs_info("  TI->PC: Buffer Size Allocation (%i bytes)", size);

	DATA_SIZE = size;

	return 0;
}

int dusb_recv_buf_size_request(CalcHandle* h, uint32_t *size)
{
	RawPacket raw = { 0 };
	uint32_t tmp;

	TRYF(dusb_recv(h, &raw));
	
	if(raw.size != 4)
		return ERR_INVALID_PACKET;

	if(raw.type != RPKT_BUF_SIZE_REQ)
		return ERR_INVALID_PACKET;

	tmp = (raw.data[0] << 24) | (raw.data[1] << 16) | (raw.data[2] << 8) | (raw.data[3] << 0);
	if(size)
		*size = tmp;
	ticalcs_info("  TI->PC: Buffer Size Request (%i bytes)", tmp);

	return 0;
}

int dusb_send_buf_size_alloc(CalcHandle* h, uint32_t size)
{	
	RawPacket raw = { 0 };

	raw.size = 4;
	raw.type = RPKT_BUF_SIZE_ALLOC;
	raw.data[2] = MSB(size);
	raw.data[3] = LSB(size);

	TRYF(dusb_send(h, &raw));
	ticalcs_info("  PC->TI: Buffer Size Allocation (%i bytes)", size);

	DATA_SIZE = size;

	return 0;
}

uint32_t dusb_get_buf_size(void)
{
	return DATA_SIZE;
}

int dusb_send_acknowledge(CalcHandle* h)
{
	RawPacket raw = { 0 };

	raw.size = 2;
	raw.type = RPKT_VIRT_DATA_ACK;
	raw.data[0] = 0xE0;
	raw.data[1] = 0x00;

	TRYF(dusb_send(h, &raw));
#if (VPKT_DBG == 2)
	ticalcs_info("  PC->TI: Virtual Packet Data Acknowledgement");
#endif

	return 0;
}

int dusb_recv_acknowledge(CalcHandle *h)
{
	RawPacket raw = { 0 };

	TRYF(dusb_recv(h, &raw));
#if (VPKT_DBG == 2)
	ticalcs_info("  TI->PC: Virtual Packet Data Acknowledgement");
#endif
	
	raw.size = raw.size;
	if(raw.size != 2 && raw.size != 4)
		return ERR_INVALID_PACKET;

	if(raw.type == RPKT_BUF_SIZE_REQ)
	{
		uint32_t size;

		if(raw.size != 4)
			return ERR_INVALID_PACKET;

		size = (raw.data[0] << 24) | (raw.data[1] << 16) | (raw.data[2] << 8) | (raw.data[3] << 0);
		ticalcs_info("  TI->PC: Buffer Size Request (%i bytes)", size);
		
		TRYF(dusb_send_buf_size_alloc(h, size));

		TRYF(dusb_recv(h, &raw));
	}
	
	if(raw.type != RPKT_VIRT_DATA_ACK)
		return ERR_INVALID_PACKET;

	if(raw.data[0] != 0xE0 && raw.data[1] != 0x00)
		return ERR_INVALID_PACKET;

	return 0;
}

// Work around TI's OS behaviour: extra bulk write of 0 size required after the last raw packet in a transfer,
// when some conditions are met.
void workaround_send(CalcHandle *h, RawPacket * raw, VirtualPacket * vtl)
{
	uint8_t buf[64];

	ticalcs_info("workaround_send: vtl->size=%d\traw->size=%d", vtl->size, raw->size);

	if (h->model == CALC_TI89T_USB)
	{
		// A 1076-byte (string) variable doesn't require this workaround, but bigger (string) variables do.
		if (vtl->size > 1076 && ((raw->size + 5) % 64) == 0)
		{
			ticalcs_info("XXX triggering an extra bulk write\n\tvtl->size=%d\traw->size=%d", vtl->size, raw->size);
			ticables_cable_send(h->cable, buf, 0);
		}
	}
	else // if (h->model == CALC_TI84P_USB)
	{
		// A 244-byte (program) variable doesn't require this workaround, but bigger (program) variables do.
		if (raw->type == RPKT_VIRT_DATA_LAST && vtl->size > 244 && (vtl->size % 250) == 244)
		{
			ticalcs_info("XXX triggering an extra bulk write\n\tvtl->size=%d\traw->size=%d", vtl->size, raw->size);
			ticables_cable_send(h->cable, buf, 0);
		}
	}
}

// Fragmentation of packets

int dusb_send_data(CalcHandle *h, VirtualPacket *vtl)
{
	RawPacket raw = { 0 };
	int i, r, q;
	long offset;

	if(vtl->size <= DATA_SIZE - DH_SIZE)
	{
		// we have a single packet which is the last one, too
		raw.size = vtl->size + DH_SIZE;
		raw.type = RPKT_VIRT_DATA_LAST;

		raw.data[0] = MSB(MSW(vtl->size));
		raw.data[1] = LSB(MSW(vtl->size));
		raw.data[2] = MSB(LSW(vtl->size));
		raw.data[3] = LSB(LSW(vtl->size));
		raw.data[4] = MSB(vtl->type);
		raw.data[5] = LSB(vtl->type);
		memcpy(&raw.data[DH_SIZE], vtl->data, vtl->size);
	
		TRYF(dusb_send(h, &raw));
#if (VPKT_DBG == 2)
		ticalcs_info("  PC->TI: Virtual Packet Data Final\n\t\t(size = %08x, type = %s)", 
			vtl->size, dusb_vpkt_type2name(vtl->type));
#elif (VPKT_DBG == 1)
		ticalcs_info("  PC->TI: %s", dusb_vpkt_type2name(vtl->type));
#endif
		workaround_send(h, &raw, vtl);
		TRYF(dusb_recv_acknowledge(h));
	}
	else
	{
		// we have more than one packet: first packet has data header
		raw.size = DATA_SIZE;
		raw.type = RPKT_VIRT_DATA;

		raw.data[0] = MSB(MSW(vtl->size));
		raw.data[1] = LSB(MSW(vtl->size));
		raw.data[2] = MSB(LSW(vtl->size));
		raw.data[3] = LSB(LSW(vtl->size));
		raw.data[4] = MSB(vtl->type);
		raw.data[5] = LSB(vtl->type);
		memcpy(&raw.data[DH_SIZE], vtl->data, DATA_SIZE - DH_SIZE);
		offset = DATA_SIZE - DH_SIZE;

		TRYF(dusb_send(h, &raw));
#if (VPKT_DBG == 2)
		ticalcs_info("  PC->TI: Virtual Packet Data with Continuation\n\t\t(size = %08x, type = %s)", 
			vtl->size, dusb_vpkt_type2name(vtl->type));
#elif (VPKT_DBG == 1)
		ticalcs_info("  PC->TI: %s", dusb_vpkt_type2name(vtl->type));
#endif
		//workaround_send(h, &raw, vtl);
		TRYF(dusb_recv_acknowledge(h));

		// other packets doesn't have data header but last one has a different type
		q = (vtl->size - offset) / DATA_SIZE;
		r = (vtl->size - offset) % DATA_SIZE;

		// send full chunks (no header)
		for(i = 1; i <= q; i++)
		{
			raw.size = DATA_SIZE;
			raw.type = RPKT_VIRT_DATA;
			memcpy(raw.data, vtl->data + offset, DATA_SIZE);
			offset += DATA_SIZE;

			TRYF(dusb_send(h, &raw));
#if (VPKT_DBG == 2)
			ticalcs_info("  PC->TI: Virtual Packet Data with Continuation");
#endif
			TRYF(dusb_recv_acknowledge(h));

			h->updat->max1 = vtl->size;
			h->updat->cnt1 += DATA_SIZE;
			h->updat->pbar();
		}

		// send last chunk (type)
		//if(r)
		{
			raw.size = r;
			raw.type = RPKT_VIRT_DATA_LAST;
			memcpy(raw.data, vtl->data + offset, r);
			offset += r;

			TRYF(dusb_send(h, &raw));

#if (VPKT_DBG == 2)
			ticalcs_info("  PC->TI: Virtual Packet Data Final");
#endif
			if (h->model != CALC_TI84P_USB)
			{
				workaround_send(h, &raw, vtl);
			}
			TRYF(dusb_recv_acknowledge(h));
		}
	}

	return 0;
}

// Work around TI's OS behaviour: extra bulk read of 0 size required after the last raw packet in a transfer,
// when some conditions are met.
void workaround_recv(CalcHandle *h, RawPacket * raw, VirtualPacket * vtl)
{
	uint8_t buf[64];

	ticalcs_info("workaround_recv: vtl->size=%d\traw->size=%d", vtl->size, raw->size);

	if (h->model == CALC_TI89T_USB)
	{
		if ((raw->size % 64) == 0)
		{
			ticalcs_info("XXX triggering an extra bulk read\n\tvtl->size=%d\traw->size=%d", vtl->size, raw->size);
			ticables_cable_recv(h->cable, buf, 0);
		}
	}
	else // if (h->model == CALC_TI84P_USB)
	{
		if (((raw->size + 5) % 64) == 0)
		{
			ticalcs_info("XXX triggering an extra bulk read\n\tvtl->size=%d\traw->size=%d", vtl->size, raw->size);
			ticables_cable_recv(h->cable, buf, 0);
		}
	}
}

// beware: data field may be re-allocated in size !
int dusb_recv_data(CalcHandle* h, VirtualPacket* vtl)
{
	RawPacket raw = { 0 };
	uint8_t buf[64];
	int i = 0;
	long offset = 0;

	do
	{
		TRYF(dusb_recv(h, &raw));
		if(raw.type != RPKT_VIRT_DATA && raw.type != RPKT_VIRT_DATA_LAST)
			return ERR_INVALID_PACKET;

		if(!i++)
		{
			// first packet has a data header
			vtl->size = (raw.data[0] << 24) | (raw.data[1] << 16) | (raw.data[2] << 8) | (raw.data[3] << 0);
			vtl->type = (uint16_t)((raw.data[4] << 8) | (raw.data[5] << 0));
			vtl->data = realloc(vtl->data, vtl->size);
			memcpy(vtl->data, &raw.data[DH_SIZE], raw.size - DH_SIZE);
			offset = raw.size - DH_SIZE;
#if (VPKT_DBG == 2)
			ticalcs_info("  TI->PC: %s\n\t\t(size = %08x, type = %s)", 
				raw.type == RPKT_VIRT_DATA_LAST ? "Virtual Packet Data Final" : "Virtual Packet Data with Continuation",
				vtl->size, dusb_vpkt_type2name(vtl->type));
#elif (VPKT_DBG == 1)
			ticalcs_info("  TI->PC: %s", dusb_vpkt_type2name(vtl->type));
#endif
			if(vtl->type == 0xEE00)
				ticalcs_info("    Error Code : %04x\n", (vtl->data[0] << 8) | vtl->data[1]);
		}
		else
		{
			// others have more data
			memcpy(vtl->data + offset, raw.data, raw.size);
			offset += raw.size;
#if (VPKT_DBG == 2)
			ticalcs_info("  TI->PC: %s", raw.type == RPKT_VIRT_DATA_LAST ? "Virtual Packet Data Final" : "Virtual Packet Data with Continuation");
#endif

			h->updat->max1 = vtl->size;
			h->updat->cnt1 += DATA_SIZE;
			h->updat->pbar();
		}

		workaround_recv(h, &raw, vtl);

		TRYF(dusb_send_acknowledge(h));

	} while(raw.type != RPKT_VIRT_DATA_LAST);

	//printf("dusb_recv_data: rpkt.size=%d\n", raw.size);

	return 0;
}
