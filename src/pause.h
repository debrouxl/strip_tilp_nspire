/* Hey EMACS -*- linux-c -*- */
/* $Id: pause.h 4392 2011-08-01 09:24:05Z debrouxl $ */

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

#ifndef __TICALCS_PAUSE__
#define __TICALCS_PAUSE__

// Little pause in milli-seconds

#define PAUSE_BETWEEN_VARS	250	// 250ms

#if defined(__LINUX__) || defined(__MACOSX__) || defined(__BSD__)
# include <unistd.h>
# define PAUSE(x)  usleep(1000*(x));
#elif defined(__WIN32__)
# include <windows.h>
# define PAUSE(x)  Sleep((x));
#endif

#endif
