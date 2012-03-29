/* Hey EMACS -*- win32-c -*- */
/* $Id: dha.h 3701 2007-08-07 18:03:36Z roms $ */

/*  libticables2 - link cable library, a part of the TiLP project
 *  Copyright (C) 2007  Romain Lievin
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

#ifndef __RWP_H__
#define __RWP_H__

int rwp_detect(int* result);

int rwp_open(void);
int rwp_close(void);

int rwp_read_byte(unsigned short address, unsigned char *data);
int rwp_write_byte(unsigned short address, unsigned char data);

#endif
