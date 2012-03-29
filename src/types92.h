/* Hey EMACS -*- linux-c -*- */
/* $Id: types92.h 4392 2011-08-01 09:24:05Z debrouxl $ */

/*  libtifiles - file format library, a part of the TiLP project
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

#ifndef __TIFILES_DEFS92__
#define __TIFILES_DEFS92__

#include "stdints2.h"

#define TI92_MAXTYPES 48

#define TI92_EXPR 0x00
#define TI92_LIST 0x04
#define TI92_MAT  0x06
#define TI92_DATA 0x0A
#define TI92_TEXT 0x0B
#define TI92_STR  0x0C
#define TI92_GDB  0x0D
#define TI92_FIG  0x0E
#define TI92_PIC  0x10
#define TI92_PRGM 0x12
#define TI92_FUNC 0x13
#define TI92_MAC  0x14
#define TI92_RDIR 0x19
#define TI92_BKUP 0x1D
#define TI92_DIR  0x1F

#define TI92_VNONE 0
#define TI92_VLOCK 1

// libtifiles: for internal use only, not exported !

extern const char *TI92_CONST[TI92_MAXTYPES + 1][4];

const char *ti92_byte2type(uint8_t data);
uint8_t ti92_type2byte(const char *s);
const char *ti92_byte2fext(uint8_t data);
uint8_t ti92_fext2byte(const char *s);

const char *ti92_byte2desc(uint8_t data);
const char *ti92_byte2icon(uint8_t data);

#endif
