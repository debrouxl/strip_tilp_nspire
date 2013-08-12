/* Hey EMACS -*- linux-c -*- */
/* $Id: data_log.h 2231 2006-04-18 06:31:02Z roms $ */

/*  libticables2 - link cable library, a part of the TiLP project
 *  Copyright (C) 1999-2005  Romain Lievin
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

#ifndef __LOG_NSP__
#define __LOG_NSP__

int log_nsp_start(void);
int log_nsp_1(int dir, uint8_t data);
int log_nsp_N(int dir, uint8_t *data, int len);
int log_nsp_stop(void);

#endif
