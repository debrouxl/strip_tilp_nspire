/* $Id: files8x.c 4425 2011-12-13 20:16:12Z debrouxl $ */

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
	TI File Format handling routines
	Calcs: 73/82/83/83+/84+/85/86
*/

/*
	Thanks to Adrian Mettler <amettler@hmc.edu> for his patch which fixes
	some TI85/86 file issues (padded, not padded).
*/

#include <glib/gstdio.h>
#include <stdlib.h>
#include <string.h>

#include <ticonv.h>
#include "tifiles.h"
#include "error.h"
#include "logging.h"
#include "typesxx.h"
#include "files8x.h"
#include "rwfile.h"
#include "intelhex.h"

#ifndef DISABLE_TI8X

/********/
/* Misc */
/********/

static uint8_t fsignature85[3] = { 0x1A, 0x0C, 0x00 };	//TI85
static uint8_t fsignature8x[3] = { 0x1A, 0x0A, 0x00 };	//TI73, 82, 83, 86


static int is_ti8586(CalcModel model)
{
  return ((model == CALC_TI85) || (model == CALC_TI86));
}

static int is_ti83p(CalcModel model)
{
  return ((model == CALC_TI83P) || (model == CALC_TI84P) || (model == CALC_TI84P_USB));
}

static uint16_t compute_backup_sum(BackupContent* content)
{
  uint16_t sum= 0;

  sum += 9;
  sum += tifiles_checksum((uint8_t *)&(content->data_length1), 2);
  sum += content->type;
  sum += tifiles_checksum((uint8_t *)&(content->data_length2), 2);
  sum += tifiles_checksum((uint8_t *)&(content->data_length3), 2);
  if (content->model != CALC_TI86)
    sum += tifiles_checksum((uint8_t *)&(content->mem_address), 2);
  else
    sum += tifiles_checksum((uint8_t *)&(content->data_length4), 2);

  sum += tifiles_checksum((uint8_t *)&(content->data_length1), 2);
  sum += tifiles_checksum(content->data_part1, content->data_length1);
  sum += tifiles_checksum((uint8_t *)&(content->data_length2), 2);
  sum += tifiles_checksum(content->data_part2, content->data_length2);
  sum += tifiles_checksum((uint8_t *)&(content->data_length3), 2);
  sum += tifiles_checksum(content->data_part3, content->data_length3);
  sum += tifiles_checksum((uint8_t *)&(content->data_length4), 2);
  sum += tifiles_checksum(content->data_part4, content->data_length4);

  return sum;
}

/***********/
/* Reading */
/***********/

/**
 * ti8x_file_read_regular:
 * @filename: name of single/group file to open.
 * @content: where to store the file content.
 *
 * Load the single/group file into a Ti8xRegular structure.
 *
 * Structure content must be freed with #tifiles_content_delete_regular when
 * no longer used. If error occurs, the structure content is released for you.
 *
 * Return value: an error code, 0 otherwise.
 **/
int ti8x_file_read_regular(const char *filename, Ti8xRegular *content)
{
  FILE *f;
  uint16_t tmp = 0x000B;
  long offset = 0;
  int i, j;
  int ti83p_flag = 0;
  uint8_t name_length = 8;	// ti85/86 only
  uint16_t data_size, sum = 0;
  char signature[9];
  int padded86 = 0;
  char varname[VARNAME_MAX];

  if (!tifiles_file_is_regular(filename))
    return ERR_INVALID_FILE;

  if (content == NULL)
  {
    tifiles_critical("%s: an argument is NULL", __FUNCTION__);
    return ERR_INVALID_FILE;
  }

  f = g_fopen(filename, "rb");
  if (f == NULL) 
  {
    tifiles_warning( "Unable to open this file: %s\n", filename);
    return ERR_FILE_OPEN_2;
  }

  if(fread_8_chars(f, signature) < 0) goto tfrr;
  content->model = tifiles_signature2calctype(signature);
  if (content->model == CALC_NONE)
    return ERR_INVALID_FILE;
  if(content->model_dst == CALC_NONE) content->model_dst = content->model;
  if(fskip(f, 3) < 0) goto tfrr;
  if(fread_n_chars(f, 42, content->comment) < 0) goto tfrr;
  if(fread_word(f, &data_size) < 0) goto tfrr;

  // search for the number of entries by parsing the whole file
  offset = ftell(f);
  if(offset == -1L) goto tfrr;

  for (i = 0;; i++) 
  {
    int current_offset = (int)ftell(f);
    /* We are done finding entries once we reach the end of the data segment
     * as defined in the header.  This works better than magic numbers, as
     * as there exist files in the wild with incorrect magic numbers that
     * transmit correctly with TI's software and with this code.
     *   Adrian Mettler
     */
	  if(current_offset == -1L) goto tfrr;
	  if (current_offset >= offset + data_size)
	    break;

    if(fread_word(f, &tmp) < 0) goto tfrr;
    if (tmp == 0x0D)
      ti83p_flag = !0;		// true TI83+ file (2 extra bytes)
	if(content->model == CALC_TI85)
	{
		// length &  name with no padding
		if(fskip(f, 3) < 0) goto tfrr;
		if(fread_byte(f, &name_length) < 0) goto tfrr;
		if(name_length > 8) return ERR_INVALID_FILE;
		if(fskip(f, name_length) < 0) goto tfrr;
	}
    else if(content->model == CALC_TI86) 
	{
      /* name may follow one of four conventions: padded with SPC bytes
       * (most correct, generated by TI's software), padded with NULL bytes,
       * unpadded (like TI85) or partially padded (garbaged).  
	   * TI's software accepts all four, so we should too.
       */
		padded86 = tmp < 0x0C ? 0 : !0;	// TI-85 style file
		
		if(fskip(f, 3) < 0) goto tfrr;
		if(fread_byte(f, &name_length) < 0) goto tfrr;
		if(name_length > 8) return ERR_INVALID_FILE;
		if(fskip(f, name_length) < 0) goto tfrr;

		if(padded86)
			if(fskip(f, 8 - name_length) < 0) goto tfrr;
    }
    else if ((content->model == CALC_TI83P) || (content->model == CALC_TI84P) || (content->model == CALC_TI84P_USB))
		{ if(fskip(f, 13) < 0) goto tfrr; }
    else
		{ if(fskip(f, 11) < 0) goto tfrr; }
    if(fread_word(f, &tmp) < 0) goto tfrr;
    if(fskip(f, tmp) < 0) goto tfrr;
  }
  if(fseek(f, offset, SEEK_SET) < 0) goto tfrr;

  content->num_entries = i;
  content->entries = g_malloc0((content->num_entries + 1) * sizeof(VarEntry*));
  if (content->entries == NULL) 
  {
    fclose(f);
    return ERR_MALLOC_2;
  }

  for (i = 0; i < content->num_entries; i++) 
  {
    VarEntry *entry = content->entries[i] = g_malloc0(sizeof(VarEntry));
	uint16_t packet_length, entry_size;

    if(fread_word(f, &packet_length) < 0) goto tfrr;
    if(fread_word(f, &entry_size) < 0) goto tfrr;
    entry->size = entry_size;
    if(fread_byte(f, &(entry->type)) < 0) goto tfrr;
    if (is_ti8586(content->model))
      if(fread_byte(f, &name_length) < 0) goto tfrr;
    if(fread_n_chars(f, name_length, varname) < 0) goto tfrr;
	ticonv_varname_from_tifile_s(content->model_dst, varname, entry->name, entry->type);
	if((content->model == CALC_TI86) && padded86)
	{
		for(j = 0; j < 8-name_length; j++)
			sum += fgetc(f);
	}
    if (ti83p_flag) 
    {
      uint16_t attribute;
      if(fread_word(f, &attribute) < 0) goto tfrr;
      // Handle both the files created by TI-Connect and the files created by
      // some broken versions of libtifiles.
      if (((attribute & 0x8000) == 0x8000) || ((attribute & 0x80) == 0x80))
      {
        entry->attr = ATTRB_ARCHIVED;
      }
      else 
      {
        entry->attr = ATTRB_NONE;
      }
    }
    if(fread_word(f, NULL) < 0) goto tfrr;

    entry->data = (uint8_t *) g_malloc0(entry->size);
    if (entry->data == NULL) 
	{
      fclose(f);
      return ERR_MALLOC_2;
    }

    if(fread(entry->data, 1, entry->size, f) < entry->size) goto tfrr;

	sum += packet_length;
    sum += tifiles_checksum((uint8_t *)&(entry->size), 2);
    sum += entry->type;
    if(is_ti8586(content->model))
      sum += strlen(entry->name);
    sum += tifiles_checksum((uint8_t *)varname, name_length);
	sum += 0; // see above (file may be padded with garbage)
    sum += tifiles_checksum((uint8_t *)&(entry->size), 2);
    sum += tifiles_checksum(entry->data, entry->size);
  }

  if(fread_word(f, &(content->checksum)) < 0) goto tfrr;
#if defined(CHECKSUM_ENABLED)
  if(sum != content->checksum) 
	return ERR_FILE_CHECKSUM;
#endif

  fclose(f);
  return 0;

tfrr:	// release on exit
    fclose(f);
	tifiles_content_delete_regular(content);
	return ERR_FILE_IO;
}

/**
 * ti8x_file_read_backup:
 * @filename: name of backup file to open.
 * @content: where to store the file content.
 *
 * Load the backup file into a Ti8xBackup structure.
 *
 * Structure content must be freed with #tifiles_content_delete_backup when
 * no longer used. If error occurs, the structure content is released for you.
 *
 * Return value: an error code, 0 otherwise.
 **/
int ti8x_file_read_backup(const char *filename, Ti8xBackup *content)
{
  FILE *f;
  char signature[9];
  uint16_t sum;

  if (!tifiles_file_is_backup(filename))
    return ERR_INVALID_FILE;

  if (content == NULL)
  {
    tifiles_critical("%s: an argument is NULL", __FUNCTION__);
    return ERR_INVALID_FILE;
  }

  f = g_fopen(filename, "rb");
  if (f == NULL) 
  {
    tifiles_info( "Unable to open this file: %s", filename);
    return ERR_FILE_OPEN_2;
  }

  if(fread_8_chars(f, signature) < 0) goto tfrb;
  content->model = tifiles_signature2calctype(signature);
  if (content->model == CALC_NONE)
    return ERR_INVALID_FILE;
  if(fskip(f, 3) < 0) goto tfrb;
  if(fread_n_chars(f, 42, content->comment) < 0) goto tfrb;
  if(fread_word(f, NULL) < 0) goto tfrb;

  if(fread_word(f, NULL) < 0) goto tfrb;
  if(fread_word(f, &(content->data_length1)) < 0) goto tfrb;
  if(fread_byte(f, &(content->type)) < 0) goto tfrb;
  if(fread_word(f, &(content->data_length2)) < 0) goto tfrb;
  if(fread_word(f, &(content->data_length3)) < 0) goto tfrb;
  content->data_length4 = 0;
  if (content->model != CALC_TI86)
	{ if(fread_word(f, &(content->mem_address)) < 0) goto tfrb; }
  else
	{ if(fread_word(f, &(content->data_length4)) < 0) goto tfrb;}

  if(fread_word(f, NULL) < 0) goto tfrb;
  content->data_part1 = (uint8_t *)g_malloc0(content->data_length1);
  if (content->data_part1 == NULL) 
  {
    fclose(f);
    return ERR_MALLOC_2;
  }
  if(fread(content->data_part1, 1, content->data_length1, f) < content->data_length1) goto tfrb;

  if(fread_word(f, NULL) < 0) goto tfrb;
  content->data_part2 = (uint8_t *)g_malloc0(content->data_length2);
  if (content->data_part2 == NULL) 
  {
    fclose(f);
    return ERR_MALLOC_2;
  }
  if(fread(content->data_part2, 1, content->data_length2, f) < content->data_length2) goto tfrb;

  if (content->data_length3)	// can be 0000 on TI86
  {
    if(fread_word(f, NULL) < 0) goto tfrb;
    content->data_part3 = (uint8_t *)g_malloc0(content->data_length3);
    if (content->data_part3 == NULL) 
	{
      fclose(f);
      return ERR_MALLOC_2;
    }
    if(fread(content->data_part3, 1, content->data_length3, f) < content->data_length3) goto tfrb;
  }

  if (content->model == CALC_TI86) 
  {
    if(fread_word(f, NULL) < 0) goto tfrb;
    content->data_part4 = (uint8_t *)g_malloc0(content->data_length4);
    if (content->data_part4 == NULL) 
	{
      fclose(f);
      return ERR_MALLOC_2;
    }
    if(fread(content->data_part4, 1, content->data_length4, f) < content->data_length4) goto tfrb;
  } 
  else 
  {
    content->data_length4 = 0;
    content->data_part4 = NULL;
  }

  if(fread_word(f, &(content->checksum)) < 0) goto tfrb;
  sum = compute_backup_sum(content);
#if defined(CHECKSUM_ENABLED)
  if(sum != content->checksum) 
	return ERR_FILE_CHECKSUM;
#endif

  fclose(f);
  return 0;

tfrb:	// release on exit
    fclose(f);
	tifiles_content_delete_backup(content);
	return ERR_FILE_IO;
}

static int check_device_type(uint8_t id)
{
	static const uint8_t types[] = { 0, DEVICE_TYPE_73, DEVICE_TYPE_83P };
	int i;

	for(i = 1; i < (int)(sizeof(types)/sizeof(types[0])); i++)
		if(types[i] == id)
			return i;

	return 0;
}

static int check_data_type(uint8_t id)
{
	static const uint8_t types[] = { 0, TI83p_AMS, TI83p_APPL, TI83p_CERT, TI83p_LICENSE };
	int i;

	for(i = 1; i < (int)(sizeof(types)/sizeof(types[0])); i++)
		if(types[i] == id)
			return i;

	return 0;
}

/**
 * ti8x_file_read_flash:
 * @filename: name of flash file to open.
 * @content: where to store the file content.
 *
 * Load the flash file into a #FlashContent structure.
 *
 * Structure content must be freed with #tifiles_content_delete_flash when
 * no longer used. If error occurs, the structure content is released for you.
 *
 * Return value: an error code, 0 otherwise.
 **/
int ti8x_file_read_flash(const char *filename, Ti8xFlash *head)
{
  FILE *f;
  Ti8xFlash *content = head;
  int i, ret;
  char signature[9];

  if (!tifiles_file_is_flash(filename))
    return ERR_INVALID_FILE;

  if (head == NULL)
  {
    tifiles_critical("%s: an argument is NULL", __FUNCTION__);
    return ERR_INVALID_FILE;
  }

  f = g_fopen(filename, "rb");
  if (f == NULL) 
  {
    tifiles_info("Unable to open this file: %s", filename);
    return ERR_FILE_OPEN_2;
  }

  for (content = head;; content = content->next) 
  {
	  if(fread_8_chars(f, signature) < 0) goto tfrf;
	  content->model = tifiles_file_get_model(filename);
	  if(fread_byte(f, &(content->revision_major)) < 0) goto tfrf;
	  if(fread_byte(f, &(content->revision_minor)) < 0) goto tfrf;
	  if(fread_byte(f, &(content->flags)) < 0) goto tfrf;
	  if(fread_byte(f, &(content->object_type)) < 0) goto tfrf;
	  if(fread_byte(f, &(content->revision_day)) < 0) goto tfrf;
	  if(fread_byte(f, &(content->revision_month)) < 0) goto tfrf;
	  if(fread_word(f, &(content->revision_year)) < 0) goto tfrf;
	  if(fskip(f, 1) < 0) goto tfrf;
	  if(fread_8_chars(f, content->name) < 0) goto tfrf;
	  if(fskip(f, 23) < 0) goto tfrf;
	  if(fread_byte(f, &(content->device_type)) < 0) goto tfrf;
	  if(fread_byte(f, &(content->data_type)) < 0) goto tfrf;
	  if(fskip(f, 24) < 0) goto tfrf;
	  if(fread_long(f, &content->data_length) < 0) goto tfrf;

	  if(!check_device_type(content->device_type))
				return ERR_INVALID_FILE;
	  if(!check_data_type(content->data_type))
				return ERR_INVALID_FILE;

	  if(content->data_type == TI83p_CERT || content->data_type == TI83p_LICENSE)
	  {
		  // get data like TI9X
		  content->data_part = (uint8_t *)g_malloc0(content->data_length + 256);
		  if (content->data_part == NULL) 
		  {
			fclose(f);
			return ERR_MALLOC_2;
		  }

		  memset(content->data_part, 0xff, content->data_length + 256);
		  if(fread(content->data_part, 1, content->data_length, f) < content->data_length) goto tfrf;

		  content->next = NULL;
	  }
	  else if(content->data_type == TI83p_AMS || content->data_type == TI83p_APPL)
	  {
		  // reset/initialize block reader
		  hex_block_read(f, NULL, NULL, NULL, NULL, NULL);
		  content->pages = NULL;

		  // we should determine the number of pages, to do...
		  content->pages = g_malloc0((50+1) * sizeof(Ti8xFlashPage *));
		  if (content->pages == NULL)
			return ERR_MALLOC_2;

		  // read FLASH pages
		  content->data_length = 0;
			for(i = 0, ret = 0; !ret; i++)
			{
				uint16_t size;
				uint16_t addr;
				uint16_t page;
				uint8_t flag = 0x80;
				uint8_t data[PAGE_SIZE];
				FlashPage* fp = content->pages[i] = g_malloc0(sizeof(FlashPage));

				ret = hex_block_read(f, &size, &addr, &flag, data, &page);

				fp->data = (uint8_t *) g_malloc0(PAGE_SIZE);
				memset(fp->data, 0xff, PAGE_SIZE);
				if (fp->data == NULL)
					return ERR_MALLOC_2;

				fp->addr = addr;
				fp->page = page;
				fp->flag = flag;
				fp->size = size;
				memcpy(fp->data, data, size);

				content->data_length += size;
			} 
		  content->num_pages = i;
		  content->next = NULL;
	  }

	  // check for end of file
		if(fread_8_chars(f, signature) < 0)
			break;
		if(strcmp(signature, "**TIFL**") || feof(f))
			break;
		if(fseek(f, -8, SEEK_CUR)) goto tfrf;

		content->next = (Ti8xFlash *)g_malloc0(sizeof(Ti8xFlash));
		if (content->next == NULL) 
		{
			fclose(f);
			return ERR_MALLOC_2;
		}
  }

  fclose(f);
  return 0;

tfrf:	// release on exit
    fclose(f);
	tifiles_content_delete_flash(content);
	return ERR_FILE_IO;
}

/***********/
/* Writing */
/***********/

/**
 * ti8x_file_write_regular:
 * @filename: name of single/group file where to write or NULL.
 * @content: the file content to write.
 * @real_filename: pointer address or NULL. Must be freed if needed when no longer needed.
 *
 * Write one (or several) variable(s) into a single (group) file. If filename is set to NULL,
 * the function build a filename from varname and allocates resulting filename in %real_fname.
 * %filename and %real_filename can be NULL but not both !
 *
 * %real_filename must be freed when no longer used.
 *
 * Return value: an error code, 0 otherwise.
 **/
int ti8x_file_write_regular(const char *fname, Ti8xRegular *content, char **real_fname)
{
  FILE *f;
  int i;
  uint16_t sum = 0;
  char *filename = NULL;
  uint32_t data_length;
  uint16_t packet_length = 0x0B;
  uint8_t name_length = 8;
  uint16_t attr;

  if (content->entries == NULL)
  {
    tifiles_warning("%s: skipping content with NULL content->entries", __FUNCTION__);
    return 0;
  }

  if (fname != NULL)
  {
    filename = g_strdup(fname);
    if (filename == NULL)
      return ERR_MALLOC_2;
  } 
  else 
  {
    if (content->entries[0])
    {
      filename = tifiles_build_filename(content->model_dst, content->entries[0]);
    }
    else
    {
      tifiles_warning("%s: asked to build a filename from null content->entries[0], bailing out", __FUNCTION__);
      if (real_fname != NULL)
      {
        *real_fname = NULL;
      }
      return 0;
    }
    if (real_fname != NULL)
      *real_fname = g_strdup(filename);
  }

  f = g_fopen(filename, "wb");
  if (f == NULL) 
  {
    tifiles_info( "Unable to open this file: %s", filename);
    g_free(filename);
    return ERR_FILE_OPEN_2;
  }
  g_free(filename);

  // write header
  if(fwrite_8_chars(f, tifiles_calctype2signature(content->model)) < 0) goto tfwr;
  if(fwrite(content->model == CALC_TI85 ? fsignature85 : fsignature8x, 1, 3, f) < 3) goto tfwr;
  if(fwrite_n_bytes(f, 42, (uint8_t *)content->comment) < 0) goto tfwr;
  for (i = 0, data_length = 0; i < content->num_entries; i++) 
  {
    VarEntry *entry = content->entries[i];
    if (entry == NULL)
    {
      tifiles_warning("%s: skipping null content entry %d", __FUNCTION__, i);
      continue;
    }

    if(content->model == CALC_TI82 || content->model == CALC_TI73)
      data_length += entry->size + 15;
    if(content->model == CALC_TI83)
      data_length += entry->size + 15;
    else if(content->model == CALC_TI85)
      data_length += entry->size + 8 + strlen(entry->name);
    else if(content->model == CALC_TI86)
      data_length += entry->size + 16;
    else if (is_ti83p(content->model))
      data_length += entry->size + 17;
  }

  if (data_length > 65535)
    return ERR_GROUP_SIZE;
  if(fwrite_word(f, (uint16_t) data_length) < 0) goto tfwr;

  // write data section
  for (i = 0, sum = 0; i < content->num_entries; i++) 
  {
    VarEntry *entry = content->entries[i];
	char varname[VARNAME_MAX];

	switch (content->model) 
	  {
	  case CALC_TI85:
		packet_length = 4 + strlen(entry->name);	//offset to data length
		break;
	  case CALC_TI86:
		packet_length = 0x0C;
		break;
	  case CALC_TI83P:
	  case CALC_TI84P:
	  case CALC_TI84P_USB:
		packet_length = 0x0D;
		break;
	  case CALC_TI73:
	  case CALC_TI82:
	  case CALC_TI83:
		packet_length = 0x0B;
		break;
	  default:
		break;
	  }

    if(fwrite_word(f, packet_length) < 0) goto tfwr;
    if(fwrite_word(f, (uint16_t)entry->size) < 0) goto tfwr;
    if(fwrite_byte(f, entry->type) < 0) goto tfwr;
	memset(varname, 0, sizeof(varname));
	ticonv_varname_to_tifile_s(content->model_dst, entry->name, varname, entry->type);
    if (is_ti8586(content->model)) 
	{
      name_length = strlen(varname);
      if(fwrite_byte(f, (uint8_t)name_length) < 0) goto tfwr;
	  if(content->model == CALC_TI85)
		{ if(fwrite_n_chars(f, name_length, varname) < 0) goto tfwr; }
	  else
		{ if(fwrite_n_chars2(f, 8, varname) < 0) goto tfwr; } // space padded
    }
    else
    	if(fwrite_n_chars(f, 8, varname) < 0) goto tfwr;
    // XXX non-zero version byte not handled (noted by Benjamin Moody).
    if (is_ti83p(content->model))
    {        
      attr = (uint16_t)((entry->attr == ATTRB_ARCHIVED) ? 0x8000 : 0x00);
      if(fwrite_word(f, attr) < 0) goto tfwr;
    }
    if(fwrite_word(f, (uint16_t)entry->size) < 0) goto tfwr;
    if(fwrite(entry->data, 1, entry->size, f) < entry->size) goto tfwr;

    sum += packet_length;
    sum += MSB(entry->size);
	sum += LSB(entry->size);
    sum += entry->type;
    if (is_ti8586(content->model))
      sum += strlen(entry->name);
    sum += tifiles_checksum((uint8_t *)varname, name_length);
	if(content->model == CALC_TI86)
		sum += (8 - name_length) * ' ';
    sum += MSB(entry->size);
	sum += LSB(entry->size);
    sum += tifiles_checksum(entry->data, entry->size);
    if (is_ti83p(content->model))
    {
      sum += tifiles_checksum((uint8_t *)&attr, sizeof(attr));
    }
  }

  //checksum is the sum of all bytes in the data section
  content->checksum = sum;
  if(fwrite_word(f, content->checksum) < 0) goto tfwr;

  fclose(f);
  return 0;

tfwr:	// release on exit
    fclose(f);
	return ERR_FILE_IO;
}

/**
 * ti8x_file_write_backup:
 * @filename: name of backup file where to write.
 * @content: the file content to write.
 *
 * Write content to a backup file.
 *
 * Return value: an error code, 0 otherwise.
 **/
int ti8x_file_write_backup(const char *filename, Ti8xBackup *content)
{
  FILE *f;
  uint16_t data_length;

  if (filename == NULL || content == NULL)
  {
    tifiles_critical("%s: an argument is NULL", __FUNCTION__);
    return ERR_INVALID_FILE;
  }

  f = g_fopen(filename, "wb");
  if (f == NULL) 
  {
    tifiles_info( "Unable to open this file: %s", filename);
    return ERR_FILE_OPEN_2;
  }
  // write header
  if(fwrite_8_chars(f, tifiles_calctype2signature(content->model)) < 0) goto tfwb;
  if(fwrite(content->model == CALC_TI85 ? fsignature85 : fsignature8x, 1, 3, f) < 3) goto tfwb;
  if(fwrite_n_bytes(f, 42, (uint8_t *)content->comment) < 0) goto tfwb;
  data_length =
      content->data_length1 + content->data_length2 +
      content->data_length3 + 17;
  data_length += content->data_length4;
  if(fwrite_word(f, data_length) < 0) goto tfwb;

  // write backup header
  if(fwrite_word(f, 0x09) < 0) goto tfwb;
  if(fwrite_word(f, content->data_length1) < 0) goto tfwb;
  if(fwrite_byte(f, content->type) < 0) goto tfwb;
  if(fwrite_word(f, content->data_length2) < 0) goto tfwb;
  if(fwrite_word(f, content->data_length3) < 0) goto tfwb;
  if (content->model != CALC_TI86)
	{ if(fwrite_word(f, content->mem_address) < 0) goto tfwb; }
  else
	{ if(fwrite_word(f, content->data_length4) < 0) goto tfwb; }

  // write data num_entries
  if(fwrite_word(f, content->data_length1) < 0) goto tfwb;
  if(fwrite(content->data_part1, 1, content->data_length1, f) < content->data_length1) goto tfwb;
  if(fwrite_word(f, content->data_length2) < 0) goto tfwb;
  if(fwrite(content->data_part2, 1, content->data_length2, f) < content->data_length2) goto tfwb;
  if (content->data_length3)	// TI86: can be NULL
    if(fwrite_word(f, content->data_length3) < 0) goto tfwb;
  if(fwrite(content->data_part3, 1, content->data_length3, f) < content->data_length3) goto tfwb;
  if (content->model == CALC_TI86) {
    if(fwrite_word(f, content->data_length4) < 0) goto tfwb;
    if(fwrite(content->data_part4, 1, content->data_length4, f) < content->data_length4) goto tfwb;
  }

  // checksum = sum of all bytes in bachup headers and data num_entries
  content->checksum = compute_backup_sum(content);
  if(fwrite_word(f, content->checksum) < 0) goto tfwb;

  fclose(f);
  return 0;

tfwb:	// release on exit
    fclose(f);
	return ERR_FILE_IO;
}

/**
 * ti8x_file_write_flash:
 * @filename: name of flash file where to write.
 * @content: the file content to write.
 * @real_filename: pointer address or NULL. Must be freed if needed when no longer needed.
 *
 * Write content to a flash file (os or app).
 *
 * Return value: an error code, 0 otherwise.
 **/
int ti8x_file_write_flash(const char *fname, Ti8xFlash *head, char **real_fname)
{
  FILE *f;
  Ti8xFlash *content = head;
  int i;
  int bytes_written = 0;
  long pos;
  char *filename;

  if (head == NULL)
  {
    tifiles_critical("%s: head is NULL", __FUNCTION__);
    return ERR_INVALID_FILE;
  }

  if (fname)
  {
	  filename = g_strdup(fname);
	  if(filename == NULL)
		  return ERR_MALLOC_2;
  }
  else
  {
	  VarEntry ve;

	  for (content = head; content != NULL; content = content->next)
		if(content->data_type == TI83p_AMS || content->data_type == TI83p_APPL)
			break;

	  strcpy(ve.name, content->name);
	  ve.type = content->data_type;

	  filename = tifiles_build_filename(content->model, &ve);
	  if (real_fname != NULL)
		*real_fname = g_strdup(filename);
  }

  f = g_fopen(filename, "wb");
  if (f == NULL) 
  {
    tifiles_info("Unable to open this file: %s", filename);
    return ERR_FILE_OPEN_2;
  }
  
  for (content = head; content != NULL; content = content->next) 
  {
	  // header
    if(fwrite_8_chars(f, "**TIFL**") < 0) goto tfwf;
    if(fwrite_byte(f, content->revision_major) < 0) goto tfwf;
    if(fwrite_byte(f, content->revision_minor) < 0) goto tfwf;
    if(fwrite_byte(f, content->flags) < 0) goto tfwf;
    if(fwrite_byte(f, content->object_type) < 0) goto tfwf;
    if(fwrite_byte(f, content->revision_day) < 0) goto tfwf;
    if(fwrite_byte(f, content->revision_month) < 0) goto tfwf;
    if(fwrite_word(f, content->revision_year) < 0) goto tfwf;
    if(fwrite_byte(f, (uint8_t) strlen(content->name)) < 0) goto tfwf;
    if(fwrite_8_chars(f, content->name) < 0) goto tfwf;
    if(fwrite_n_chars(f, 23, "") < 0) goto tfwf;
    if(fwrite_byte(f, content->device_type) < 0) goto tfwf;
    if(fwrite_byte(f, content->data_type) < 0) goto tfwf;
    if(fwrite_n_chars(f, 24, "") < 0) goto tfwf;
	pos = ftell(f);
	if(pos == -1L) goto tfwf;
    if(fwrite_long(f, content->data_length) < 0) goto tfwf;

	// data
	if(content->data_type == TI83p_CERT || content->data_type == TI83p_LICENSE)
	{
		if(fwrite(content->data_part, 1, content->data_length, f) < content->data_length) goto tfwf;
	}
	else if(content->data_type == TI83p_AMS || content->data_type == TI83p_APPL)
	{
		// write
		for (i = 0; i < content->num_pages; i++)
		  {
			  uint32_t app_length, page_length;
			  int extra_bytes = 0;

			  page_length = content->pages[i]->size;

			  if (content->data_type == TI83p_APPL && i == content->num_pages - 1
			      && content->pages[0]->data[0] == 0x80 && content->pages[0]->data[1] == 0x0f)
			  {
				  /* Flash app signing programs will usually add some
				     padding to the end of the app, and some programs
				     seem to expect that padding to be there.

				     The following is designed to mimic the behavior
				     of these programs, padding to 96 bytes beyond the
				     end of the application proper. */

				  /* get actual app length */
				  app_length = 6 + (content->pages[0]->data[2] << 24
				                    | content->pages[0]->data[3] << 16
				                    | content->pages[0]->data[4] << 8
				                    | content->pages[0]->data[5]);

				  /* remove any existing padding */
				  while (page_length > 0 && content->pages[i]->data[page_length - 1] == 0xff)
					  page_length--;

				  extra_bytes = app_length + 96 - i * 0x4000 - page_length;

				  /* don't add padding beyond the end of the page */
				  if (page_length + extra_bytes >= 0x3fff)
					  extra_bytes = 0x3fff - page_length;

				  if (extra_bytes < 0)
					  extra_bytes = 0;
				  else if (extra_bytes > 96)
					  extra_bytes = 96;
			  }

			  bytes_written += hex_block_write(f, 
				  page_length, content->pages[i]->addr,
				  content->pages[i]->flag, content->pages[i]->data, 
				  content->pages[i]->page, extra_bytes);
		  }

		  // final block
		  bytes_written += hex_block_write(f, 0, 0, 0, NULL, 0, 0);
		  if(fseek(f, -bytes_written - 4, SEEK_CUR)) goto tfwf;
		  if(fwrite_long(f, bytes_written) < 0) goto tfwf;
		  if(fseek(f, SEEK_END, 0L) ) goto tfwf;
	}
  }  

  fclose(f);
  return 0;

tfwf:	// release on exit
    fclose(f);
	return ERR_FILE_IO;
}


/**************/
/* Displaying */
/**************/

/**
 * ti8x_content_display_regular:
 * @content: a Ti8xRegular structure.
 *
 * Display fields of a Ti8xRegular structure.
 *
 * Return value: an error code, 0 otherwise.
 **/
int ti8x_content_display_regular(Ti8xRegular *content)
{
  int i;
  char trans[17];

  if (content == NULL)
  {
    tifiles_critical("%s(NULL)", __FUNCTION__);
    return ERR_INVALID_FILE;
  }

  tifiles_info("Signature:     %s",
	  tifiles_calctype2signature(content->model));
  tifiles_info("Comment:       %s", content->comment);
  tifiles_info("# of entries:  %i", content->num_entries);

  for (i = 0; i < content->num_entries /*&& i<5 */ ; i++) 
  {
    tifiles_info("Entry #%i", i);
    tifiles_info("  name:        %s",
		ticonv_varname_to_utf8_s(content->model, content->entries[i]->name, 
			trans, content->entries[i]->type));
    tifiles_info("  type:        %02X (%s)",
	    content->entries[i]->type,
	    tifiles_vartype2string(content->model, content->entries[i]->type));
    tifiles_info("  attr:        %s",
	    tifiles_attribute_to_string(content->entries[i]->attr));
    tifiles_info("  length:      %04X (%i)",
	    content->entries[i]->size, content->entries[i]->size);
  }

  tifiles_info("Checksum:      %04X (%i) ", content->checksum,
	  content->checksum);

  return 0;
}

/**
 * ti8x_content_display_backup:
 * @content: a Ti8xBackup structure.
 *
 * Display fields of a Ti8xBackup structure.
 *
 * Return value: an error code, 0 otherwise.
 **/
int ti8x_content_display_backup(Ti8xBackup *content)
{
  if (content == NULL)
  {
    tifiles_critical("%s(NULL)", __FUNCTION__);
    return ERR_INVALID_FILE;
  }

  tifiles_info("Signature:      %s",
	  tifiles_calctype2signature(content->model));
  tifiles_info("Comment:        %s", content->comment);
  tifiles_info("Type:           %02X (%s)", content->type,
	  tifiles_vartype2string(content->model, content->type));
  tifiles_info("Mem address:    %04X (%i)",
	  content->mem_address, content->mem_address);

  tifiles_info("\n");

  tifiles_info("data_length1:   %04X (%i)",
	  content->data_length1, content->data_length1);
  tifiles_info("data_length2:   %04X (%i)",
	  content->data_length2, content->data_length2);
  tifiles_info("data_length3:   %04X (%i)",
	  content->data_length3, content->data_length3);
  if (content->model == CALC_TI86)
    tifiles_info("data_length4:   %04X (%i)",
	    content->data_length4, content->data_length4);

  tifiles_info("Checksum:       %04X (%i) ", content->checksum,
	  content->checksum);

  return 0;
}

/**
 * ti8x_content_display_flash:
 * @content: a Ti8xFlash structure.
 *
 * Display fields of a Ti8xFlash structure.
 *
 * Return value: an error code, 0 otherwise.
 **/
int ti8x_content_display_flash(Ti8xFlash *content)
{
  Ti8xFlash *ptr;

  for (ptr = content; ptr != NULL; ptr = ptr->next) 
  {
	  tifiles_info("Signature:       %s",
		  tifiles_calctype2signature(ptr->model));
	  tifiles_info("Revision:        %i.%i", ptr->revision_major,
		  ptr->revision_minor);
	  tifiles_info("Flags:           %02X", ptr->flags);
	  tifiles_info("Object type:     %02X", ptr->object_type);
	  tifiles_info("Date:            %02X/%02X/%02X%02X",
		  ptr->revision_day, ptr->revision_month,
		  ptr->revision_year&0xff, (ptr->revision_year&0xff00) >> 8);
	  tifiles_info("Name:            %s", ptr->name);
	  tifiles_info("Device type:     %s",
		  ptr->device_type == DEVICE_TYPE_83P ? "ti83+" : "ti73");
	  tifiles_info("Data type:       ");
	  switch (ptr->data_type) 
	  {
	  case 0x23:
		tifiles_info("OS data");
		break;
	  case 0x24:
		tifiles_info("APP data");
		break;
	  case 0x20:
	  case 0x25:
		tifiles_info("certificate");
		break;
	  case 0x3E:
		tifiles_info("license");
		break;
	  default:
		tifiles_info("Unknown (mailto tilp-users@lists.sf.net)\n");
		break;
	  }
	  tifiles_info("Length:          %08X (%i)", ptr->data_length,
			ptr->data_length);
	  tifiles_info("");
	  tifiles_info("Number of pages: %i", ptr->num_pages);
  }

  return 0;
}

/**
 * ti8x_file_display:
 * @filename: a TI file.
 *
 * Determine file class and display internal content.
 *
 * Return value: an error code, 0 otherwise.
 **/
int ti8x_file_display(const char *filename)
{
  Ti8xRegular *content1;
  Ti8xBackup *content2;
  Ti8xFlash *content3;

  if (tifiles_file_is_flash(filename)) 
  {
	content3 = tifiles_content_create_flash(CALC_TI83P);
    ti8x_file_read_flash(filename, content3);
    ti8x_content_display_flash(content3);
    tifiles_content_delete_flash(content3);
  } 
  else if (tifiles_file_is_regular(filename)) 
  {
	content1 = tifiles_content_create_regular(CALC_NONE);
    ti8x_file_read_regular(filename, content1);
    ti8x_content_display_regular(content1);
    tifiles_content_delete_regular(content1);
  } 
  else if (tifiles_file_is_backup(filename)) 
  {
	content2 = tifiles_content_create_backup(CALC_NONE);
    ti8x_file_read_backup(filename, content2);
    ti8x_content_display_backup(content2);
    tifiles_content_delete_backup(content2);
  }
  else 
  {
    tifiles_info("Unknown file type !");
    return ERR_BAD_FILE;
  }

  return 0;
}

#endif
