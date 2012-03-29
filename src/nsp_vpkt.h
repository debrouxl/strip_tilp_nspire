/* Hey EMACS -*- linux-c -*- */
/* $Id: cmd84p.h 2074 2006-03-31 08:36:06Z roms $ */

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

#ifndef __NSP_VPKT__
#define __NSP_VPKT__

// Convenients structures

typedef struct
{
	uint16_t	src_addr;
	uint16_t	src_port;
	uint16_t	dst_addr;
	uint16_t	dst_port;

	uint8_t		cmd;

	uint32_t	size;
	uint8_t		*data;
} VirtualPacket;

typedef struct
{
	uint16_t	id;
	const char *name;
} ServiceName;

// Constants

#define NSP_SRC_ADDR	0x6400
#define NSP_DEV_ADDR	0x6401

#define PORT_PKT_NACK		0x00D3
#define PORT_PKT_ACK1		0x00FE
#define PORT_PKT_ACK2		0x00FF
#define PORT_NULL			0x4001
#define PORT_ECHO			0x4002
#define PORT_ADDR_REQUEST	0x4003
#define PORT_ADDR_ASSIGN	0x4003
#define PORT_DEV_INFOS		0x4020
#define PORT_SCREENSHOT		0x4021
#define PORT_SCREEN_RLE		0x4024
#define PORT_LOGIN			0x4050
#define PORT_FILE_MGMT		0x4060
#define PORT_OS_INSTALL		0x4080
#define PORT_DISCONNECT		0x40DE

// Exports

extern uint16_t nsp_src_port;
extern uint16_t nsp_dst_port;

// Functions

VirtualPacket*  nsp_vtl_pkt_new(void);
VirtualPacket*  nsp_vtl_pkt_new_ex(uint32_t size, uint16_t src_addr, uint16_t src_port, uint16_t dst_addr, uint16_t dst_port);
void			nsp_vtl_pkt_del(VirtualPacket* pkt);
void			nsp_vtl_pkt_purge(void);

int nsp_session_open(CalcHandle *h, uint16_t port);
int nsp_session_close(CalcHandle *h);

int nsp_addr_request(CalcHandle *h);
int nsp_addr_assign(CalcHandle *h, uint16_t dev_addr);

int nsp_send_ack(CalcHandle *h);
int nsp_recv_ack(CalcHandle *h);
int nsp_send_nack(CalcHandle *h);

int nsp_send_data(CalcHandle* h, VirtualPacket* pkt);
int nsp_recv_data(CalcHandle* h, VirtualPacket* pkt);

int nsp_send_disconnect(CalcHandle *h);
int nsp_recv_disconnect(CalcHandle *h);

const char* nsp_sid2name(uint16_t id);

#endif
