/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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

/*
  Grouping/Ungrouping routines
  Calcs: 73/82/83/83+/84+/85/86 & 89/89tm/92/92+/V200
*/

#include <stdlib.h>
#include <string.h>

#include "tifiles.h"
#include "macros.h"
#include "logging.h"

/***********/
/* Freeing */
/***********/

/**
 * tifiles_content_create_group:
 * @n_entries: number of variables to allocate
 *
 * Convenient function which create a NULL-terminated array of #FileContent 
 * structures (typically used to store a group file).
 *
 * Return value: the array or NULL if failed.
 **/
TIEXPORT2 FileContent** TICALL tifiles_content_create_group(int n_entries)
{
	return (FileContent **)g_malloc0((n_entries + 1) * sizeof(FileContent *));
}

/**
 * tifiles_content_delete_group:
 *
 * Convenient function which free a NULL-terminated array of #FileContent 
 * structures (typically used to store a group file) and the array itself.
 *
 * Return value: always 0.
 **/
TIEXPORT2 int TICALL tifiles_content_delete_group(FileContent **array)
{
	int i, n;

	if (array != NULL)
	{
		// counter number of files to group
		for (n = 0; array[n] != NULL; n++);

		// release allocated memory in structures
		for (i = 0; i < n; i++)
		{
			tifiles_content_delete_regular(array[i]);
		}
		g_free(array);
	}
	else
	{
		tifiles_critical("%s(NULL)", __FUNCTION__);
	}

	return 0;
}
