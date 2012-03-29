/* Hey EMACS -*- linux-c -*- */
/* $Id: gtk_update.h 4392 2011-08-01 09:24:05Z debrouxl $ */

/*  TiLP - Tilp Is a Linking Program
 *  Copyright (C) 1999-2006  Romain Lievin
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

#ifndef __GTK_UPDATE__
#define __GTK_UPDATE__

extern CalcUpdate	gtk_update;

void tilp_gif_set_gtk(void);
void tilp_update_set_gtk(void);

#define GTK_REFRESH() { while( gtk_events_pending() ) { gtk_main_iteration(); }}

#endif