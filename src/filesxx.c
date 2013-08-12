/* Hey EMACS -*- linux-c -*- */
/* $Id: filesxx.c 4412 2011-11-12 12:46:08Z debrouxl $ */

/*  libtifiles - file format library, a part of the TiLP project
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

/*
  This unit contains a TI file independant API
*/

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tifiles.h"
#include "error.h"
#include "filesnsp.h"
#include "logging.h"

/**
 * tifiles_content_create_regular:
 * @model: a calculator model (required).
 *
 * Allocates a #FileContent structure.
 *
 * Return value: the allocated block.
 **/
TIEXPORT2 FileContent* TICALL tifiles_content_create_regular(CalcModel model)
{
	FileContent* content = g_malloc0(sizeof(FileContent));

	if (content != NULL)
	{
		content->model = content->model_dst = model;
		strcpy(content->comment, tifiles_comment_set_single());
	}

	return content;
}

/**
 * tifiles_content_delete_regular:
 *
 * Free the whole content of a #FileContent structure.
 *
 * Return value: none.
 **/
TIEXPORT2 int TICALL tifiles_content_delete_regular(FileContent *content)
{
	int i;

	if (content != NULL)
	{
		for (i = 0; i < content->num_entries; i++) 
		{
			VarEntry *entry = content->entries[i];

			if(entry != NULL)
			{
				g_free(entry->data);
				g_free(entry);
			}
			else
			{
				tifiles_critical("tifiles_content_delete_regular(content with NULL entry)");
			}
		}

		g_free(content->entries);
		g_free(content);
	}
	else
	{
		tifiles_critical("%s(NULL)", __FUNCTION__);
	}
	return 0;
}

/**
 * tifiles_content_dup_regular:
 *
 * Allocates and copies a new #FileContent structure.
 *
 * Return value: none.
 **/
TIEXPORT2 FileContent* TICALL tifiles_content_dup_regular(FileContent *content)
{
	FileContent *dup = NULL;
	int i;

	if (content != NULL)
	{
		dup = tifiles_content_create_regular(content->model);
		if (dup != NULL)
		{
			memcpy(dup, content, sizeof(FileContent));
			dup->entries = tifiles_ve_create_array(content->num_entries);

			if (dup->entries != NULL)
			{
				for (i = 0; i < content->num_entries; i++) 
					dup->entries[i] = tifiles_ve_dup(content->entries[i]);
			}
		}
	}
	else
	{
		tifiles_critical("%s(NULL)", __FUNCTION__);
	}
	return dup;
}

/**
 * tifiles_file_read_regular:
 * @filename: name of single/group file to open.
 * @content: where to store the file content.
 *
 * Load the single/group file into a FileContent structure.
 *
 * Structure content must be freed with #tifiles_content_delete_regular when
 * no longer used.
 *
 * Return value: an error code, 0 otherwise.
 **/
TIEXPORT2 int tifiles_file_read_regular(const char *filename, FileContent *content)
{
	if (filename == NULL || content == NULL)
	{
		tifiles_critical("%s: an argument is NULL", __FUNCTION__);
		return ERR_INVALID_FILE;
	}

	if(content->model == CALC_NSPIRE)
		return tnsp_file_read_regular(filename, (FileContent *)content);
	else
		return ERR_BAD_CALC;

	return 0;
}

/**
 * tifiles_file_write_regular:
 * @filename: name of single/group file where to write or NULL.
 * @content: the file content to write.
 * @real_fname: pointer address or NULL. Must be freed if needed when no longer needed.
 *
 * Write one (or several) variable(s) into a single (group) file. If filename is set to NULL,
 * the function build a filename from varname and allocates resulting filename in %real_fname.
 * %filename and %real_filename can be NULL but not both !
 *
 * %real_filename must be freed when no longer used.
 *
 * Return value: an error code, 0 otherwise.
 **/
TIEXPORT2 int tifiles_file_write_regular(const char *filename, FileContent *content, char **real_fname)
{
	if (content == NULL || (filename == NULL && real_fname == NULL))
	{
		tifiles_critical("%s: an argument is NULL", __FUNCTION__);
		return ERR_INVALID_FILE;
	}

	if(content->model == CALC_NSPIRE)
		return tnsp_file_write_regular(filename, (FileContent *)content, real_fname);
	else
		return ERR_BAD_CALC;

	return 0;
}

/**
 * tifiles_file_display_regular:
 * @content: the file content to show.
 *
 * Display file content informations.
 *
 * Return value: an error code, 0 otherwise.
 **/
TIEXPORT2 int TICALL tifiles_file_display_regular(FileContent *content)
{
	if (content == NULL)
	{
		tifiles_critical("%s(NULL)", __FUNCTION__);
		return ERR_INVALID_FILE;
	}

	if(content->model == CALC_NSPIRE)
		return tnsp_content_display_regular(content);
	else
		return ERR_BAD_CALC;

	return 0;
}

/**
 * tifiles_content_create_backup:
 * @model: a calculator model or CALC_NONE.
 *
 * Allocates a BackupContent structure.
 *
 * Return value: the allocated block.
 **/
TIEXPORT2 BackupContent* TICALL tifiles_content_create_backup(CalcModel model)
{
	BackupContent* content = g_malloc0(sizeof(BackupContent));

	if (content != NULL)
	{
		content->model = model;
		strcpy(content->comment, tifiles_comment_set_backup());
	}

	return content;
}

/**
 * tifiles_content_delete_backup:
 *
 * Free the whole content of a BackupContent structure.
 *
 * Return value: none.
 **/
TIEXPORT2 int TICALL tifiles_content_delete_backup(BackupContent *content)
{
	if (content != NULL)
	{
		g_free(content);
	}
	else
	{
		tifiles_critical("%s(NULL)", __FUNCTION__);
	}

	return 0;
}

/**
 * tifiles_file_read_backup:
 * @filename: name of backup file to open.
 * @content: where to store the file content.
 *
 * Load the backup file into a BackupContent structure.
 *
 * Structure content must be freed with #tifiles_content_delete_backup when
 * no longer used.
 *
 * Return value: an error code, 0 otherwise.
 **/
TIEXPORT2 int tifiles_file_read_backup(const char *filename, BackupContent *content)
{
	if (filename == NULL || content == NULL)
	{
		tifiles_critical("%s: an argument is NULL", __FUNCTION__);
		return ERR_INVALID_FILE;
	}

	return ERR_BAD_CALC;

	return 0;
}

/**
 * tifiles_file_write_backup:
 * @filename: name of backup file where to write.
 * @content: the file content to write.
 *
 * Write backup into file.
 *
 * Return value: an error code, 0 otherwise.
 **/
TIEXPORT2 int tifiles_file_write_backup(const char *filename, BackupContent *content)
{
	if (filename == NULL || content == NULL)
	{
		tifiles_critical("%s: an argument is NULL", __FUNCTION__);
		return ERR_INVALID_FILE;
	}

	return ERR_BAD_CALC;

	return 0;
}

/**
 * tifiles_file_display_backup:
 * @content: the file content to show.
 *
 * Display file content informations.
 *
 * Return value: an error code, 0 otherwise.
 **/
TIEXPORT2 int TICALL tifiles_file_display_backup(BackupContent *content)
{
	if (content == NULL)
	{
		tifiles_critical("%s(NULL)", __FUNCTION__);
		return ERR_INVALID_FILE;
	}

	return ERR_BAD_CALC;

	return 0;
}

/**
 * tifiles_content_create_flash:
 * @model: a calculator model (compulsory).
 *
 * Allocates a #FlashContent structure.
 *
 * Return value: the allocated block.
 **/
TIEXPORT2 FlashContent* TICALL tifiles_content_create_flash(CalcModel model)
{
	FlashContent* content = g_malloc0(sizeof(FlashContent));

	if (content != NULL)
	{
		content->model = model;
	}

	return content;
}

/**
 * tifiles_content_delete_flash:
 *
 * Free the whole content of a #FlashContent structure.
 *
 * Return value: none.
 **/
TIEXPORT2 int TICALL tifiles_content_delete_flash(FlashContent *content)
{
	if (content != NULL)
	{
		return ERR_BAD_CALC;
	}
	else
	{
		tifiles_critical("%s(NULL)", __FUNCTION__);
	}

	return 0;
}

/**
 * tifiles_file_read_flash:
 * @filename: name of FLASH file to open.
 * @content: where to store the file content.
 *
 * Load the FLASH file into a FlashContent structure.
 *
 * Structure content must be freed with #tifiles_content_delete_flash when
 * no longer used.
 *
 * Return value: an error code, 0 otherwise.
 **/
TIEXPORT2 int tifiles_file_read_flash(const char *filename, FlashContent *content)
{
	if (filename == NULL || content == NULL)
	{
		tifiles_critical("%s: an argument is NULL", __FUNCTION__);
		return ERR_INVALID_FILE;
	}

	if(content->model == CALC_NSPIRE)
		return tnsp_file_read_flash(filename, content);
	else
		return ERR_BAD_CALC;

	return 0;
}

/**
 * tifiles_file_write_flash2:
 * @filename: name of flash file where to write or NULL.
 * @content: the file content to write.
 * @real_fname: pointer address or NULL. Must be freed if needed when no longer needed.
 *
 * Write a FLASH content to a file. If filename is set to NULL, the function build a filename 
 * from appname and allocates resulting filename in %real_fname.
 * %filename and %real_fname can be NULL but not both !
 *
 * %real_fname must be freed when no longer used.
 *
 * Return value: an error code, 0 otherwise.
 **/
TIEXPORT2 int tifiles_file_write_flash2(const char *filename, FlashContent *content, char **real_fname)
{
	if (content == NULL || (filename == NULL && real_fname == NULL))
	{
		tifiles_critical("%s: an argument is NULL", __FUNCTION__);
		return ERR_INVALID_FILE;
	}

	return ERR_BAD_CALC;

	return 0;
}

/**
 * tifiles_file_write_flash:
 * @filename: name of flash file where to write or NULL.
 * @content: the file content to write.
 *
 * Write a FLASH content to a file. If filename is set to NULL, the function build a filename 
 * from appname.
 *
 * Return value: an error code, 0 otherwise.
 **/
TIEXPORT2 int tifiles_file_write_flash(const char *filename, FlashContent *content)
{
	return tifiles_file_write_flash2(filename, content, NULL);
}

/**
 * tifiles_content_dup_flash:
 *
 * Allocates and copies a new FlashContent structure.
 *
 * Return value: none.
 **/
TIEXPORT2 FlashContent* TICALL tifiles_content_dup_flash(FlashContent *content)
{
	FlashContent *dup = NULL;
	FlashContent *p, *q;

	if (content != NULL)
	{
		dup = tifiles_content_create_flash(content->model);
		if (dup != NULL)
		{
			for(p = content, q = dup; p; p = p->next, q = q->next)
			{
				memcpy(q, p, sizeof(FlashContent));

				if(p->next)
					q->next = tifiles_content_create_flash(p->model);
			}
		}
	}
	else
	{
		tifiles_critical("%s(NULL)", __FUNCTION__);
	}

	return dup;
}

/**
 * tifiles_file_display_flash:
 * @content: the file content to show.
 *
 * Display file content informations.
 *
 * Return value: an error code, 0 otherwise.
 **/
TIEXPORT2 int TICALL tifiles_file_display_flash(FlashContent *content)
{
	if (content == NULL)
	{
		tifiles_critical("%s(NULL)", __FUNCTION__);
		return ERR_INVALID_FILE;
	}

	return ERR_BAD_CALC;

	return 0;
}

/**
 * tifiles_file_display:
 * @filename: a TI file.
 *
 * Determine file class and display internal content.
 *
 * Return value: an error code, 0 otherwise.
 **/
TIEXPORT2 int TICALL tifiles_file_display(const char *filename)
{
	return ERR_BAD_CALC;
}

/*****************/
/* Miscellaneous */
/*****************/

/**
 * tifiles_create_table_of_entries:
 * @content: a TI file content structure.
 * @nfolders: returns the number of folders in the file.
 *
 * The goal of this function is to parse the file content structure in order to build
 * a table of entries so that it's easy to write it just after the header in a group
 * file. Mainly used as an helper.
 * The returned 'table' is an NULL-terminated array of int* pointers.
 * Each pointers points on an integer array. Each cell are an index on the 'VarEntry*  
 * entries' array.
 * 
 * In fact, this array represents a kind of tree. The array of pointer is the folder list
 * and each pointer is the variable list for each folder.
 * For accessing the entry, we use the index.
 *
 * This function may be difficult to understand but it avoids to use trees (and
 * linked list) which will require an implementation.
 *
 * Return value: a 2-dimensions allocated integer array. Must be freed with g_free when
 * no longer used.
 **/
TIEXPORT2 int** tifiles_create_table_of_entries(FileContent *content, int *nfolders)
{
	int num_folders = 0;
	int i, j;
	char **ptr, *folder_list[32768] = { 0 };
	int **table;

	if (content == NULL)
	{
		tifiles_critical("%s: an argument is NULL", __FUNCTION__);
		return NULL;
	}

	// determine how many folders we have
	for (i = 0; i < content->num_entries; i++) 
	{
		VarEntry *entry = content->entries[i];

		// scan for an existing folder entry
		for (ptr = folder_list; *ptr != NULL; ptr++) 
		{
			if (!strcmp(*ptr, entry->folder)) 
			{
				//printf("break: %s\n", entry->folder);
				break;
			}
		}
		if (*ptr == NULL) 
		{		// add new folder entry
			folder_list[num_folders] = (char *) g_malloc0(257);
			//printf("%i: adding '%s'\n", num_folders, entry->folder);
			strcpy(folder_list[num_folders], entry->folder);
			folder_list[num_folders + 1] = NULL;
			num_folders++;
		}
	}
	if (nfolders != NULL)
	{
		*nfolders = num_folders;
	}

	// allocate the folder list
	table = (int **) g_malloc0((num_folders + 1) * sizeof(int *));
	table[num_folders] = NULL;

	// for each folder, determine how many variables we have
	// and allocate array with indexes
	for (j = 0; j < num_folders; j++) 
	{
		int k;

		for (i = 0, k = 0; i < content->num_entries; i++) 
		{
			VarEntry *entry = content->entries[i];

			if (!strcmp(folder_list[j], entry->folder)) 
			{
				table[j] = (int *) g_realloc(table[j], (k + 2) * sizeof(int));
				table[j][k] = i;
				//printf("%i %i: adding %i\n", j, k, i); 
				table[j][k + 1] = -1;
				k++;
			}
		}
	}

	// g_free( memory
	for (j = 0; j < num_folders + 1; j++)
		g_free(folder_list[j]);

	return table;
}
