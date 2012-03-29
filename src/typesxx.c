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
#ifndef DISABLE_TI8X
  case CALC_TI73:
    return ti73_byte2type(data);
  case CALC_TI82:
    return ti82_byte2type(data);
  case CALC_TI83:
    return ti83_byte2type(data);
  case CALC_TI83P:
    return ti83p_byte2type(data);
  case CALC_TI84P:
  case CALC_TI84P_USB:
    return ti84p_byte2type(data);
  case CALC_TI85:
    return ti85_byte2type(data);
  case CALC_TI86:
    return ti86_byte2type(data);
#endif
#ifndef DISABLE_TI9X
  case CALC_TI89:
    return ti89_byte2type(data);
  case CALC_TI89T:
  case CALC_TI89T_USB:
    return ti89t_byte2type(data);
  case CALC_TI92:
    return ti92_byte2type(data);
  case CALC_TI92P:
    return ti92p_byte2type(data);
  case CALC_V200:
    return v200_byte2type(data);
  case CALC_NSPIRE:
    return nsp_byte2type(data);
#endif
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
#ifndef DISABLE_TI8X
  case CALC_TI73:
    return ti73_type2byte(s);
  case CALC_TI82:
    return ti82_type2byte(s);
  case CALC_TI83:
    return ti83_type2byte(s);
  case CALC_TI83P:
  case CALC_TI84P:
  case CALC_TI84P_USB:
    return ti83p_type2byte(s);
  case CALC_TI85:
    return ti85_type2byte(s);
  case CALC_TI86:
    return ti86_type2byte(s);
#endif
#ifndef DISABLE_TI9X
  case CALC_TI89:
  case CALC_TI89T:
  case CALC_TI89T_USB:
    return ti89_type2byte(s);
  case CALC_TI92:
    return ti92_type2byte(s);
  case CALC_TI92P:
    return ti92p_type2byte(s);
  case CALC_V200:
    return v200_type2byte(s);
  case CALC_NSPIRE:
    return nsp_type2byte(s);
#endif
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
#ifndef DISABLE_TI8X
  case CALC_TI73:
    return ti73_byte2fext(data);
  case CALC_TI82:
    return ti82_byte2fext(data);
  case CALC_TI83:
    return ti83_byte2fext(data);
  case CALC_TI83P:
  case CALC_TI84P:
  case CALC_TI84P_USB:
    return ti83p_byte2fext(data);
  case CALC_TI85:
    return ti85_byte2fext(data);
  case CALC_TI86:
    return ti86_byte2fext(data);
#endif
#ifndef DISABLE_TI9X
  case CALC_TI89:
  case CALC_TI89T:
  case CALC_TI89T_USB:
    return ti89_byte2fext(data);
  case CALC_TI92:
    return ti92_byte2fext(data);
  case CALC_TI92P:
    return ti92p_byte2fext(data);
  case CALC_V200:
    return v200_byte2fext(data);
  case CALC_NSPIRE:
    return nsp_byte2fext(data);
#endif
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
#ifndef DISABLE_TI8X
  case CALC_TI73:
    return ti73_fext2byte(s);
  case CALC_TI82:
    return ti82_fext2byte(s);
  case CALC_TI83:
    return ti83_fext2byte(s);
  case CALC_TI83P:
  case CALC_TI84P:
  case CALC_TI84P_USB:
    return ti83p_fext2byte(s);
  case CALC_TI85:
    return ti85_fext2byte(s);
  case CALC_TI86:
    return ti86_fext2byte(s);
#endif
#ifndef DISABLE_TI9X
  case CALC_TI89:
  case CALC_TI89T:
  case CALC_TI89T_USB:
    return ti89_fext2byte(s);
  case CALC_TI92:
    return ti92_fext2byte(s);
  case CALC_TI92P:
    return ti92p_fext2byte(s);
  case CALC_V200:
    return v200_fext2byte(s);
  case CALC_NSPIRE:
    return nsp_fext2byte(s);
#endif
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
#ifndef DISABLE_TI8X
  case CALC_TI73:
    return ti73_byte2desc(vartype);
  case CALC_TI82:
    return ti82_byte2desc(vartype);
  case CALC_TI83:
    return ti83_byte2desc(vartype);
  case CALC_TI83P:
  case CALC_TI84P:
  case CALC_TI84P_USB:
    return ti83p_byte2desc(vartype);
  case CALC_TI85:
    return ti85_byte2desc(vartype);
  case CALC_TI86:
    return ti86_byte2desc(vartype);
#endif
#ifndef DISABLE_TI9X
  case CALC_TI89:
  case CALC_TI89T:
  case CALC_TI89T_USB:
    return ti89_byte2desc(vartype);
  case CALC_TI92:
    return ti92_byte2desc(vartype);
  case CALC_TI92P:
    return ti92p_byte2desc(vartype);
  case CALC_V200:
    return v200_byte2desc(vartype);
  case CALC_NSPIRE:
    return nsp_byte2desc(vartype);
#endif
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
#ifndef DISABLE_TI8X
  case CALC_TI73:
    return ti73_byte2icon(vartype);
  case CALC_TI82:
    return ti82_byte2icon(vartype);
  case CALC_TI83:
    return ti83_byte2icon(vartype);
  case CALC_TI83P:
  case CALC_TI84P:
  case CALC_TI84P_USB:
    return ti83p_byte2icon(vartype);
  case CALC_TI85:
    return ti85_byte2icon(vartype);
  case CALC_TI86:
    return ti86_byte2icon(vartype);
#endif
#ifndef DISABLE_TI9X
  case CALC_TI89:
  case CALC_TI89T:
  case CALC_TI89T_USB:
    return ti89_byte2icon(vartype);
  case CALC_TI92:
    return ti92_byte2icon(vartype);
  case CALC_TI92P:
    return ti92p_byte2icon(vartype);
  case CALC_V200:
    return v200_byte2icon(vartype);
  case CALC_NSPIRE:
    return nsp_byte2icon(vartype);
#endif
  default:
    tifiles_critical("%s: invalid model argument.", __FUNCTION__);
    return "";
  }
}

/****************/
/* Global types */
/****************/

static const char GROUP_FILE_EXT[CALC_MAX + 1][4] = 
{
	"XxX", 
	"73g", "82g", "83g", "8Xg", "8Xg", "85g", "86g", 
	"89g", "89g", "92g", "9Xg", "v2g", "???",
};

static const char BACKUP_FILE_EXT[CALC_MAX + 1][4] = 
{
	"XxX", 
	"73b", "82b", "83b", "8Xb", "8Xb", "85b", "86b", 
	"89g", "89g", "92b", "9Xg", "v2g", 
	"???",
};

static const char FLASH_APP_FILE_EXT[CALC_MAX + 1][4] = 
{
	"XxX", 
	"73k", "???", "???", "8Xk", "8Xk", "???", "???",
	"89k", "89k", "???", "9Xk", "v2k", 
	"???",
};

static const char FLASH_OS_FILE_EXT[CALC_MAX + 1][4] = 
{
	"XxX", 
	"73u", "???", "???", "8Xu", "8Xu", "???", "???",
	"89u", "89u", "???", "9Xu", "v2u", 
	"tno",
};

static const int TIXX_DIR[CALC_MAX + 1] = 
{
	-1, 
	TI73_DIR, -1, TI83_DIR, TI83p_DIR, TI84p_DIR, -1, TI86_DIR,
	TI89_DIR, TI89_DIR, TI92_DIR, V200_DIR, 
	NSP_DIR,
};

static const int TIXX_FLASH[CALC_MAX + 1] = 
{
	-1, 
	TI73_APPL, -1, -1, TI83p_APPL, TI84p_APPL, -1, -1,
	TI89_APPL, TI89t_APPL, -1, TI92p_APPL, V200_APPL, 
	-1,
};

static const int TIXX_IDLIST[CALC_MAX + 1] = 
{
	-1, 
	TI73_IDLIST, -1, -1, TI83p_IDLIST, TI84p_IDLIST, -1, -1,
	TI89_IDLIST, TI89t_IDLIST, -1, TI92p_IDLIST, V200_IDLIST,
	-1,
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
  case CALC_NONE:
    return -1;
  case CALC_TI73:
    return TI73_DIR;
  case CALC_TI82:
    return -1;
  case CALC_TI83:
    return TI83_DIR;
  case CALC_TI83P:
  case CALC_TI84P:
  case CALC_TI84P_USB:
    return TI83p_DIR;
  case CALC_TI85:
    return -1;
  case CALC_TI86:
    return TI86_DIR;
  case CALC_TI89:
  case CALC_TI89T:
  case CALC_TI89T_USB:
    return TI89_DIR;
  case CALC_TI92:
    return TI92_DIR;
  case CALC_TI92P:
    return TI92p_DIR;
  case CALC_V200:
    return V200_DIR;
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
  case CALC_NONE:
    return -1;
  case CALC_TI73:
    return TI73_APPL;
  case CALC_TI82:
    return -1;
  case CALC_TI83:
    return -1;
  case CALC_TI83P:
  case CALC_TI84P:
  case CALC_TI84P_USB:
    return TI83p_APPL;
  case CALC_TI85:
    return -1;
  case CALC_TI86:
    return -1;
  case CALC_TI89:
  case CALC_TI89T:
  case CALC_TI89T_USB:
    return TI89_APPL;
  case CALC_TI92:
    return -1;
  case CALC_TI92P:
    return TI92p_APPL;
  case CALC_V200:
    return V200_APPL;
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
  case CALC_NONE:
    return -1;
  case CALC_TI73:
    return TI73_IDLIST;
  case CALC_TI82:
    return -1;
  case CALC_TI83:
    return -1;
  case CALC_TI83P:
  case CALC_TI84P:
  case CALC_TI84P_USB:
    return TI83p_IDLIST;
  case CALC_TI85:
    return -1;
  case CALC_TI86:
    return -1;
  case CALC_TI89:
  case CALC_TI89T:
  case CALC_TI89T_USB:
    return TI89_IDLIST;
  case CALC_TI92:
    return -1;
  case CALC_TI92P:
    return TI92p_IDLIST;
  case CALC_V200:
    return V200_IDLIST;
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
  case CALC_TI73:
    return "**TI73**";
  case CALC_TI82:
    return "**TI82**";
  case CALC_TI83:
    return "**TI83**";
  case CALC_TI83P:
    return "**TI83F*";
  case CALC_TI84P:
  case CALC_TI84P_USB:
    return "**TI83F*";
  case CALC_TI85:
    return "**TI85**";
  case CALC_TI86:
    return "**TI86**";
  case CALC_TI89:
    return "**TI89**";
  case CALC_TI89T:
  case CALC_TI89T_USB:
    return "**TI89**";
  case CALC_TI92:
    return "**TI92**";
  case CALC_TI92P:
    return "**TI92P*";
  case CALC_V200:
    return "**TI92P*";
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
    if (!g_ascii_strcasecmp(s, "**TI73**"))
      return CALC_TI73;
    else if (!g_ascii_strcasecmp(s, "**TI82**"))
      return CALC_TI82;
    else if (!g_ascii_strcasecmp(s, "**TI83**"))
      return CALC_TI83;
    else if (!g_ascii_strcasecmp(s, "**TI83F*"))
      return CALC_TI83P;
    else if (!g_ascii_strcasecmp(s, "**TI85**"))
      return CALC_TI85;
    else if (!g_ascii_strcasecmp(s, "**TI86**"))
      return CALC_TI86;
    else if (!g_ascii_strcasecmp(s, "**TI89**"))
      return CALC_TI89;
    else if (!g_ascii_strcasecmp(s, "**TI92**"))
      return CALC_TI92;
    else if (!g_ascii_strcasecmp(s, "**TI92P*"))
      return CALC_TI92P;
    else if (!g_ascii_strcasecmp(s, "**V200**"))
      return CALC_V200;
  }
  tifiles_critical("%s: invalid signature.", __FUNCTION__);
  return CALC_NONE;
}
