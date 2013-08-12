/* Hey EMACS -*- linux-c -*- */
/* $Id: link_nul.c 1059 2005-05-14 09:45:42Z roms $ */

/*  libticalcs2 - hand-helds support library, a part of the TiLP project
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
	Pseudo-backup support (common).
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <string.h>

#include <ticonv.h>
#include "ticalcs.h"
#include "internal.h"
#include "logging.h"
#include "error.h"
#include "pause.h"

int tixx_recv_backup(CalcHandle* handle, BackupContent* content)
{
	int i, j, k;
	int i_max, j_max;
	GNode *vars, *apps;
	int nvars, ivars = 0;
	int b = 0;
	FileContent **group;
	FileContent *single;

	if(handle == NULL) return ERR_INVALID_HANDLE;
	if (content == NULL)
	{
		ticalcs_critical("tixx_recv_backup: content is NULL");
		return -1;
	}

	// Do a directory list and check for something to backup
	TRYF(handle->calc->get_dirlist(handle, &vars, &apps));
	nvars = ticalcs_dirlist_ve_count(vars);
	if(!nvars)
		return ERR_NO_VARS;

	update_->cnt2 = update_->cnt3 = 0;
	update_->max2 = update_->max3 = nvars;
	update_->pbar();

	// Check whether the last folder is empty
	b = g_node_n_children(g_node_nth_child(vars, g_node_n_children(vars) - 1));
	PAUSE(100); // needed by TI84+/USB

	// Create a group file
	k = 0;
	group = tifiles_content_create_group(nvars);

	// Receive all vars except for FLASH apps
	i_max = g_node_n_children(vars);
	for(i = 0; i < i_max; i++) 
	{
		GNode *parent = g_node_nth_child(vars, i);

		j_max = g_node_n_children(parent);
		for(j = 0; j < j_max; j++) 
		{
			GNode *node = g_node_nth_child(parent, j);
			VarEntry *ve = (VarEntry *) (node->data);

			update_->cnt2 = update_->cnt3 = ++ivars;
			update_->pbar();

			// we need to group files !
			TRYF(handle->calc->is_ready(handle));
			group[k] = tifiles_content_create_regular(handle->model);
			TRYF(handle->calc->recv_var(handle, 0, group[k++], ve));
		}
	}

	ticalcs_dirlist_destroy(&vars);
	ticalcs_dirlist_destroy(&apps);

	tifiles_group_contents(group, &single);
	tifiles_content_delete_group(group);

	// Swap content and single because we have a pointer on an allocated content
	{
		FileContent* cnt = (FileContent *)content;

		memcpy(content, single, sizeof(FileContent));
		cnt->entries = single->entries;
		strcpy(cnt->comment, tifiles_comment_set_group());
	}

	return 0;
}
