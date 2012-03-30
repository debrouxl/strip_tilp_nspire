/* Hey EMACS -*- linux-c -*- */
/* $Id: type2str.c 4412 2011-11-12 12:46:08Z debrouxl $ */

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

#include <string.h>
#include <glib.h>

#include "gettext.h"
#include "tifiles.h"
#include "ticables.h"
#include "ticalcs.h"

#include "logging.h"

/**
 * tifiles_model_to_string:
 * @model: a calculator model.
 *
 * Do an integer to string conversion.
 *
 * Return value: a string like "TI92+".
 **/
TIEXPORT2 const char *TICALL tifiles_model_to_string(CalcModel model)
{
	switch (model)
	{
	case CALC_NONE:  return "none";
	case CALC_NSPIRE: return "NSpire";
	default: return "unknown";
	}
}

/**
 * tifiles_string_to_model:
 * @str: a calculator model as string like "TI92".
 *
 * Do a string to integer conversion.
 *
 * Return value: a calculator model.
 **/
TIEXPORT2 CalcModel TICALL tifiles_string_to_model(const char *str)
{
	if (str != NULL)
	{
		if(!g_ascii_strncasecmp(str, "TI NSpire", 9) || !g_ascii_strncasecmp(str, "NSpire", 6))
			return CALC_NSPIRE;
	}

	return CALC_NONE;
}

/**
 * tifiles_attribute_to_string:
 * @attrb: an attribute of variable.
 *
 * Do an integer to string conversion.
 *
 * Return value: a string like "archived".
 **/
TIEXPORT2 const char *TICALL tifiles_attribute_to_string(FileAttr attrb)
{
	switch (attrb)
	{
	case ATTRB_NONE:      return _("none     ");
	case ATTRB_LOCKED:    return _("locked   ");
	case ATTRB_ARCHIVED:  return _("archived ");
	case ATTRB_PROTECTED: return _("protected");
	default: return "unknown";
	}
}

/**
 * tifiles_string_to_attribute:
 * @str: a variable attribute string like "protected".
 *
 * Do a string to integer conversion.
 *
 * Return value: a variable attribute.
 **/
TIEXPORT2 FileAttr TICALL tifiles_string_to_attribute(const char *str)
{
	if (str != NULL)
	{
		if(!g_ascii_strcasecmp(str, _("none     ")))
			return ATTRB_NONE;
		else if(!g_ascii_strcasecmp(str, _("locked   ")))
			return ATTRB_LOCKED;
		else if(!g_ascii_strcasecmp(str, _("archived ")))
			return ATTRB_ARCHIVED;
		else if(!g_ascii_strcasecmp(str, _("protected")))
			return ATTRB_PROTECTED;
	}

	return ATTRB_NONE;
}

/**
 * tifiles_class_to_string:
 * @klass: a class of file.
 *
 * Do an integer to string conversion.
 *
 * Return value: a string like "backup".
 **/
TIEXPORT2 const char *TICALL tifiles_class_to_string(FileClass klass)
{
	switch (klass)
	{
	case TIFILE_SINGLE: return _("single");
	case TIFILE_GROUP:  return _("group");
	case TIFILE_REGULAR:return _("regular");
	case TIFILE_BACKUP: return _("backup");
	case TIFILE_TIGROUP:return _("tigroup");
	case TIFILE_OS:		return _("os");
	case TIFILE_APP:	return _("application");
	case TIFILE_FLASH:  return _("flash");
	default: return _("unknown");
	}
}

/**
 * tifiles_string_to_class:
 * @str: a file class string like "backup".
 *
 * Do a string to integer conversion.
 *
 * Return value: a file class.
 **/
TIEXPORT2 FileClass TICALL tifiles_string_to_class(const char *str)
{
	if (str != NULL)
	{
		if(!g_ascii_strcasecmp(str, _("single")))
			return TIFILE_SINGLE;
		else if(!g_ascii_strcasecmp(str, _("group")))
			return TIFILE_GROUP;
		else if(!g_ascii_strcasecmp(str, _("regular")))
			return TIFILE_REGULAR;
		else if(!g_ascii_strcasecmp(str, _("backup")))
			return TIFILE_BACKUP;
		else if(!g_ascii_strcasecmp(str, _("os")))
			return TIFILE_OS;
		else if(!g_ascii_strcasecmp(str, _("application")))
			return TIFILE_APP;
		else if(!g_ascii_strcasecmp(str, _("flash")))
			return TIFILE_FLASH;
		else if(!g_ascii_strcasecmp(str, _("tigroup")))
			return TIFILE_TIGROUP;
	}

	return 0;
}

/**
 * ticables_model_to_string:
 * @model: a cable model.
 *
 * Do an integer to string conversion.
 *
 * Return value: a string like "BlackLink".
 **/
TIEXPORT1 const char *TICALL ticables_model_to_string(CableModel model)
{
  	switch (model) 
	{
	case CABLE_NUL: return "null";
	case CABLE_USB: return "DirectLink";
	default: return "unknown";
	}
}

/**
 * ticables_string_to_model:
 * @str: a cable model as string like "BlackLink".
 *
 * Do a string to integer conversion.
 *
 * Return value: a cable model.
 **/
TIEXPORT1 CableModel TICALL ticables_string_to_model(const char *str)
{
	if (str == NULL)
	{
		ticables_critical("%s(NULL)", __FUNCTION__);
		return CABLE_NUL;
	}

	if(!g_ascii_strcasecmp(str, "null"))
		return CABLE_NUL;
	else if(!g_ascii_strcasecmp(str, "DirectLink"))
		return CABLE_USB;

	return CABLE_NUL;
}

/**
 * ticables_port_to_string:
 * @port: a link port.
 *
 * Do an integer to string conversion.
 *
 * Return value: a string like "#1".
 **/
TIEXPORT1 const char *TICALL ticables_port_to_string(CablePort port)
{
	switch (port)
	{
	case PORT_0: return "null";
	case PORT_1: return "#1";
	case PORT_2: return "#2";
	case PORT_3: return "#3";
	case PORT_4: return "#4";
	default: return "unknown";
	}
}

/**
 * ticables_string_to_port:
 * @str: a link port as string like "#1".
 *
 * Do a string to integer conversion.
 *
 * Return value: a link port.
 **/
TIEXPORT1 CablePort TICALL ticables_string_to_port(const char *str)
{
	if (str == NULL)
	{
		ticables_critical("%s(NULL)", __FUNCTION__);
		return PORT_0;
	}

	if(!g_ascii_strcasecmp(str, "null"))
		return PORT_0;
	else if(!strcmp(str, "#1"))
		return PORT_1;
	else if(!strcmp(str, "#2"))
		return PORT_2;
	else if(!strcmp(str, "#3"))
		return PORT_3;
	else if(!strcmp(str, "#4"))
		return PORT_4;

	return PORT_0;
}

/**
 * ticables_usbpid_to_string:
 * @pid: a Usb Product Id.
 *
 * Do an integer to string conversion.
 *
 * Return value: a string like "SilverLink".
 **/
TIEXPORT1 const char *TICALL ticables_usbpid_to_string(UsbPid pid)
{
	switch (pid)
	{
	case PID_NSPIRE: return "NSpire";		// must match tifiles name
	default: return "unknown";
	}
}

/**
 * ticables_usbpid_to_port:
 * @str: a Usb Product Id as string like "SilverLink".
 *
 * Do a string to integer conversion.
 *
 * Return value: a Usb Product Id.
 **/
TIEXPORT1 UsbPid TICALL ticables_string_to_usbpid(const char *str)
{
	if (str == NULL)
	{
		ticables_critical("%s(NULL)", __FUNCTION__);
		return PID_UNKNOWN;
	}

	if(!strcmp(str, "NSpire"))
		return PID_NSPIRE;

	return PID_UNKNOWN;
}

/**
 * ticalcs_model_to_string:
 * @model: a calculator model.
 *
 * Do an integer to string conversion.
 *
 * Return value: a string like "TI92+".
 **/
TIEXPORT3 const char *TICALL ticalcs_model_to_string(CalcModel model)
{
	return tifiles_model_to_string(model);
}

/**
 * ticalcs_string_to_model:
 * @str: a calculator model as string like "TI92".
 *
 * Do a string to integer conversion.
 *
 * Return value: a calculator model.
 **/
TIEXPORT3 CalcModel TICALL ticalcs_string_to_model(const char *str)
{
	return tifiles_string_to_model(str);
}


TIEXPORT3 const char *TICALL ticalcs_scrfmt_to_string(CalcScreenFormat format)
{
	switch (format)
	{
	case SCREEN_FULL: return _("full");
	case SCREEN_CLIPPED: return _("clipped");
	default: return _("unknown");
	}
}

TIEXPORT3 CalcScreenFormat TICALL ticalcs_string_to_scrfmt(const char *str)
{
	if (str != NULL)
	{
		if(!strcmp(str, _("full")))
			return SCREEN_FULL;
		else if(!strcmp(str, _("clipped")))
			return SCREEN_CLIPPED;
	}
	else
	{
		ticalcs_critical("ticalcs_string_to_scrfmt(NULL)");
	}

	return SCREEN_CLIPPED;
}


TIEXPORT3 const char *TICALL ticalcs_pathtype_to_string(CalcPathType type)
{
	switch (type)
	{
	case PATH_FULL: return _("full");
	case PATH_LOCAL: return _("local");
	default: return _("unknown");
	}
}

TIEXPORT3 CalcPathType TICALL ticalcs_string_to_pathtype(const char *str)
{
	if (str != NULL)
	{
		if(!strcmp(str, _("full")))
			return PATH_FULL;
		else if(!strcmp(str, _("local")))
			return PATH_LOCAL;
	}
	else
	{
		ticalcs_critical("ticalcs_string_to_pathtype(NULL)");
	}

	return PATH_FULL;
}


TIEXPORT3 const char *TICALL ticalcs_memtype_to_string(CalcMemType type)
{
	switch (type)
	{
	case MEMORY_FREE: return _("free");
	case MEMORY_USED: return _("used");
	default: return _("unknown");
	}
}

TIEXPORT3 CalcMemType TICALL ticalcs_string_to_memtype(const char *str)
{
	if (str != NULL)
	{
		if(!strcmp(str, _("free")))
			return MEMORY_FREE;
		else if(!strcmp(str, _("used")))
			return MEMORY_USED;
	}

	return MEMORY_NONE;
}
