/* Hey EMACS -*- linux-c -*- */
/* $Id: cmd82.h 4392 2011-08-01 09:24:05Z debrouxl $ */

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

#ifndef __TICALCS_CMD82__
#define __TICALCS_CMD82__

int ti82_send_VAR_h(CalcHandle*, uint16_t varsize, uint8_t vartype, const char *varname);
int ti82_send_CTS_h(CalcHandle*);
int ti82_send_XDP_h(CalcHandle*, int length, uint8_t * data);
int ti82_send_SKP_h(CalcHandle*, uint8_t rej_code);
int ti82_send_ACK_h(CalcHandle*);
int ti82_send_ERR_h(CalcHandle*);
int ti82_send_SCR_h(CalcHandle*);
int ti83_send_KEY_h(CalcHandle*, uint16_t scancode);
int ti82_send_KEY_h(CalcHandle*, uint16_t scancode);
int ti82_send_EOT_h(CalcHandle*);
int ti82_send_REQ_h(CalcHandle*, uint16_t varsize, uint8_t vartype, const char *varname);
int ti82_send_RTS_h(CalcHandle*, uint16_t varsize, uint8_t vartype, const char *varname);

int ti82_recv_VAR_h(CalcHandle*, uint16_t * varsize, uint8_t * vartype, char *varname);
int ti82_recv_CTS_h(CalcHandle*);
int ti82_recv_SKP_h(CalcHandle*, uint8_t * rej_code);
int ti82_recv_XDP_h(CalcHandle*, uint16_t * length, uint8_t * data);
int ti82_recv_ACK_h(CalcHandle*, uint16_t * status);
int ti82_recv_RTS_h(CalcHandle*, uint16_t * varsize, uint8_t * vartype, char *varname);

// ---

#define ti82_send_VAR(a,b,c)		ti82_send_VAR_h(handle, a, b, c)
#define ti82_send_CTS()				ti82_send_CTS_h(handle)
#define ti82_send_XDP(a,b)			ti82_send_XDP_h(handle, a, b)
#define ti82_send_SKP(a,b)			ti82_send_SKP_h(handle, a, b)
#define ti82_send_ACK()				ti82_send_ACK_h(handle)	
#define ti82_send_ERR()				ti82_send_ERR_h(handle)			
#define ti82_send_SCR()				ti82_send_SCR_h(handle)			
#define ti83_send_KEY(a)			ti83_send_KEY_h(handle,a)
#define ti82_send_KEY(a)			ti82_send_KEY_h(handle, a)
#define ti82_send_EOT()				ti82_send_EOT_h(handle)	
#define ti82_send_REQ(a,b,c)		ti82_send_REQ_h(handle, a, b, c)
#define ti82_send_RTS(a,b,c)		ti82_send_RTS_h(handle, a, b, c)

#define ti82_recv_VAR(a,b,c)		ti82_recv_VAR_h(handle, a, b, c)
#define ti82_recv_CTS()				ti82_recv_CTS_h(handle)	
#define ti82_recv_SKP(a)			ti82_recv_SKP_h(handle, a)	
#define ti82_recv_XDP(a,b)			ti82_recv_XDP_h(handle, a, b)	
#define ti82_recv_ACK(a)			ti82_recv_ACK_h(handle, a)
#define ti82_recv_RTS(a,b,c)		ti82_recv_RTS_h(handle, a, b, c)

#endif
