/* Hey EMACS -*- linux-c -*- */
/* $Id: typesxx.c 4412 2011-11-12 12:46:08Z debrouxl $ */

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

#include <stdio.h>
#include "gettext.h"
#include "tifiles.h"
#include "logging.h"
#include "error.h"
#include "typesxx.h"
#include "rwfile.h"

/********************************/
/* Calculator independant types */
/********************************/

/**
 * tifiles_vartype2string:
 * @model: a calculator model.
 * @data: a type ID.
 *
 * Returns the type of variable (REAL, EQU, PRGM, ...).
 *
 * Return value: a string like "REAL".
 **/
TIEXPORT2 const char *TICALL tifiles_vartype2string(CalcModel model, uint8_t data)
{
  switch (model) 
  {
  case CALC_NSPIRE:
    return nsp_byte2type(data);
  default:
    tifiles_critical("%s: invalid model argument.", __FUNCTION__);
    return "";
  }
}

/**
 * tifiles_string2vartype:
 * @model: a calculator model.
 * @s: a type as string (like "REAL").
 *
 * Returns the type of variable.
 *
 * Return value: a type ID.
 **/
TIEXPORT2 uint8_t TICALL tifiles_string2vartype(CalcModel model, const char *s)
{
  if (s == NULL)
  {
    tifiles_critical("%s: invalid string !", __FUNCTION__);
    return 0;
  }

  switch (model) 
  {
  case CALC_NSPIRE:
    return nsp_type2byte(s);
  default:
    tifiles_critical("%s: invalid model argument.", __FUNCTION__);
    return 0;
  }
}

/**
 * tifiles_vartype2fext:
 * @model: a calculator model.
 * @data: a type ID.
 *
 * Returns the file extension tipcially used to store this kind of variable(REAL, EQU, PRGM, ...).
 *
 * Return value: a string like "REAL".
 **/
TIEXPORT2 const char *TICALL tifiles_vartype2fext(CalcModel model, uint8_t data)
{
  switch (model) 
  {
  case CALC_NSPIRE:
    return nsp_byte2fext(data);
  default:
    tifiles_critical("%s: invalid model argument.", __FUNCTION__);
    return "";
  }
}

/**
 * tifiles_fext2vartype:
 * @model: a calculator model.
 * @s: a file extension as string (like 89p).
 *
 * Returns the type ID of variable (REAL, EQU, PRGM, ...).
 *
 * Return value: a string like "PRGM".
 **/
TIEXPORT2 uint8_t TICALL tifiles_fext2vartype(CalcModel model, const char *s)
{
  if (s == NULL)
  {
    tifiles_critical("%s: invalid string !", __FUNCTION__);
    return 0;
  }

  switch (model) 
  {
  case CALC_NSPIRE:
    return nsp_fext2byte(s);
  default:
    tifiles_critical("%s: invalid model argument.", __FUNCTION__);
    return 0;
  }
}

/**
 * tifiles_vartype2type:
 * @model: a calculator model.
 * @id: a vartype ID.
 *
 * Returns the type ID of variable as string ("Real", "Program", ...).
 * The function is localized.
 *
 * Return value: a string like "Assembly Program".
 **/
TIEXPORT2 const char *TICALL tifiles_vartype2type(CalcModel model, uint8_t vartype)
{
  switch (model)
  {
  case CALC_NSPIRE:
    return nsp_byte2desc(vartype);
  default:
    tifiles_critical("%s: invalid model argument.", __FUNCTION__);
    return "";
  }
}

/**
 * tifiles_vartype2icon:
 * @model: a calculator model.
 * @id: a vartype ID.
 *
 * Returns the type ID of variable as string ("Real", "Program", ...).
 * Same as #tifiles_vartype2type but un-localized.
 *
 * Return value: a string like "Assembly Program".
 **/
TIEXPORT2 const char *TICALL tifiles_vartype2icon(CalcModel model, uint8_t vartype)
{
  switch (model) 
  {
  case CALC_NSPIRE:
    return nsp_byte2icon(vartype);
  default:
    tifiles_critical("%s: invalid model argument.", __FUNCTION__);
    return "";
  }
}

/****************/
/* Global types */
/****************/

static const char FLASH_OS_FILE_EXT[CALC_MAX + 1][4] = 
{
	"XxX", 
	"73u", "???", "???", "8Xu", "8Xu", "???", "???",
	"89u", "89u", "???", "9Xu", "v2u", 
	"tno",
};

/**
 * tifiles_folder_type
 * @model: a calculator model in #CalcModel enumeration.
 *
 * Returns the variable type ID used for encoding folders.
 *
 * Return value: a type ID.
 **/
TIEXPORT2 uint8_t TICALL tifiles_folder_type(CalcModel model)
{
  switch (model) 
  {
  case CALC_NSPIRE:
    return NSP_DIR;
  default:
    tifiles_critical("%s: invalid model argument.", __FUNCTION__);
    break;
  }

  return -1;
}

/**
 * tifiles_flash_type
 * @model: a calculator model in #CalcModel enumeration.
 *
 * Returns the variable type ID used for encoding FLASH apps.
 *
 * Return value: a type ID.
 **/
TIEXPORT2 uint8_t TICALL tifiles_flash_type(CalcModel model)
{
  switch (model) 
  {
  case CALC_NSPIRE:
    return -1;
  default:
    tifiles_critical("%s: invalid model argument.", __FUNCTION__);
    break;
  }

  return -1;
}

/**
 * tifiles_idlist_type
 * @model: a calculator model in #CalcModel enumeration.
 *
 * Returns the variable type ID used for encoding IDLIST variable.
 *
 * Return value: a type ID.
 **/
TIEXPORT2 uint8_t TICALL tifiles_idlist_type(CalcModel model)
{
  switch (model) 
  {
  case CALC_NSPIRE:
    return -1;
  default:
    tifiles_critical("%s: invalid model argument.", __FUNCTION__);
    break;
  }

  return -1;
}

/****************/
/* Miscelaneous */
/****************/

/**
 * tifiles_calctype2signature:
 * @model: a calculator model.
 *
 * Returns the signature used at the top of a TI file depending on the
 * calculator model.
 *
 * Return value: a string like "**TI89**".
 **/
TIEXPORT2 const char *TICALL tifiles_calctype2signature(CalcModel model)
{
  switch (model) 
  {
  case CALC_NONE:
    return "**TI??**";
  case CALC_NSPIRE:
    return "";
  default:
    tifiles_critical("%s: invalid model argument.", __FUNCTION__);
    break;
  }

  return NULL;
}

/**
 * tifiles_signature2calctype:
 * @s: a TI file signature like "**TI89**".
 *
 * Returns the calculator model contained in the signature.
 *
 * Return value: a calculator model.
 **/
TIEXPORT2 CalcModel TICALL tifiles_signature2calctype(const char *s)
{
  if (s != NULL)
  {
  }
  tifiles_critical("%s: invalid signature.", __FUNCTION__);
  return CALC_NONE;
}
