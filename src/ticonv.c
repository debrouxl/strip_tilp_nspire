/* Hey EMACS -*- linux-c -*- */
/* $Id: ticonv.c 4412 2011-11-12 12:46:08Z debrouxl $ */

/*  libticonv - charset library, a part of the TiLP project
 *  Copyright (C) 2006-2006 Romain Lievin and Kevin Kofler
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
  This unit contains the interface of the libticonv library.
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <glib.h>

#include "ticonv.h"

/***********/
/* Methods */
/***********/

/**
 * ticonv_version_get:
 *
 * This function returns the library version like "X.Y.Z".
 *
 * Return value: a constant string.
 **/
TIEXPORT4 const char *TICALL ticonv_version_get(void)
{
	return LIBCONV_VERSION;
}

/**
 * ticonv_utf16_strlen:
 * @str: null terminated UTF-16 string
 *
 * UTF-16 version of strlen (same as wcslen if wchar_t is UTF-16, but portable).
 *
 * Return value: number of characters. Surrogate pairs are counted as 2
 *               characters each.
 **/
TIEXPORT4 size_t TICALL ticonv_utf16_strlen(const unsigned short *str)
{
	size_t l = 0;
	if (str != NULL)
	{
		const unsigned short *p = str;
		while (*(p++)) l++;
	}
	else
	{
		g_critical("%s(NULL)", __FUNCTION__);
	}
	return l;
}

/**
 * ticonv_utf8_to_utf16:
 * @src: null terminated UTF-8 string
 *
 * UTF-8 to UTF-16 conversion.
 *
 * Return value: a newly allocated string or NULL if error.
 **/
TIEXPORT4 unsigned short* ticonv_utf8_to_utf16(const char *str)
{
	gunichar2* dst;
	const gchar* src = str;
	GError *error = NULL;

	dst = g_utf8_to_utf16(src, -1, NULL, NULL, &error);
	if(error) g_critical("%s", error->message);

	return dst;
}

/**
 * ticonv_utf16_to_utf8:
 * @src: null terminated UTF-16 string
 *
 * UTF-16 to UTF-8 conversion.
 *
 * Return value: a newly allocated string or NULL if error.
 **/
TIEXPORT4 char* ticonv_utf16_to_utf8(const unsigned short *str)
{
	const gunichar2* src = str;
	gchar* dst;
	GError *error = NULL;

	dst = g_utf16_to_utf8(src, -1, NULL, NULL, &error);
	if(error) g_critical("%s", error->message);

	return dst;
}

/**
 * ticonv_charset_utf16_to_ti_s:
 * @model: calculator model
 * @utf16: null terminated string (input)
 * @ti: null terminated string (output)
 *
 * UTF-16 to TI charset conversion.
 *
 * Return value: the %ti string or NULL if error.
 **/
TIEXPORT4 char* TICALL ticonv_charset_utf16_to_ti_s(CalcModel model, const unsigned short *utf16, char *ti)
{
	if (utf16 != NULL && ti != NULL)
	{
		switch(model)
		{
			case CALC_NSPIRE:
				{
				char *tmp = ticonv_utf16_to_utf8(utf16);
				strcpy(ti, tmp);
				g_free(tmp);
				return ti;
				}
				break;
			default: return strcpy(ti, "");
		}

		return strcpy(ti, "");
	}
	else
	{
		g_critical("%s: an argument is NULL", __FUNCTION__);
		return NULL;
	}
}

/**
 * ticonv_charset_utf16_to_ti:
 * @model: calculator model
 * @utf16: null terminated string (input)
 * @ti: null terminated string (output)
 *
 * UTF-16 to TI charset conversion.
 *
 * Return value: a newly allocated string or NULL if error.
 **/
TIEXPORT4 char* TICALL ticonv_charset_utf16_to_ti(CalcModel model, const unsigned short *utf16)
{
	if (utf16 != NULL)
	{
		char * ti = g_malloc0(4*ticonv_utf16_strlen(utf16) + 1);	// upper bound
		ticonv_charset_utf16_to_ti_s(model, utf16, ti);
		return ti;
	}
	else
	{
		g_critical("%s: utf16 is NULL", __FUNCTION__);
		return NULL;
	}
}

/**
 * ticonv_charset_ti_to_utf16_s:
 * @ti: null terminated string (input)
 * @utf16: null terminated string (output)
 *
 * TI charset to UTF-16 conversion.
 * The %utf16 destination buffer will hold the result. It must be big enough.
 *
 * Return value: the %utf16 string or NULL if error.
 **/
TIEXPORT4 unsigned short* TICALL ticonv_charset_ti_to_utf16_s(CalcModel model, const char *ti, unsigned short *utf16)
{
	if (ti != NULL && utf16 != NULL)
	{
		switch(model)
		{
			case CALC_NSPIRE:
				{
				unsigned short *tmp = ticonv_utf8_to_utf16(ti);
				memcpy(utf16, tmp, 2*ticonv_utf16_strlen(tmp));
				g_free(tmp);
				return utf16;
				}
				break;
			default: return memcpy(utf16, "\0", 2);
		}

		return memcpy(utf16, "\0", 2);
	}
	else
	{
		g_critical("%s: an argument is NULL", __FUNCTION__);
		return NULL;
	}
}

/**
 * ticonv_charset_ti_to_utf16:
 * @ti: null terminated string (input)
 * @utf16: null terminated string (output)
 *
 * TI charset to UTF-16 conversion.
 *
 * Return value: a newly allocated string or NULL if error.
 **/
TIEXPORT4 unsigned short* TICALL ticonv_charset_ti_to_utf16(CalcModel model, const char *ti)
{
	if (ti != NULL)
	{
		unsigned short * utf16 = g_malloc0(4 * strlen(ti) + 2);	// upper bound
		ticonv_charset_ti_to_utf16_s(model, ti, utf16);
		return utf16;
	}
	else
	{
		g_critical("%s: ti is NULL", __FUNCTION__);
		return NULL;
	}
}

/**
 * ticonv_varname_to_utf16:
 * @model: a calculator model.
 * @src: a name of variable to detokenize and translate (17 chars max).
 * @vartype: the type of variable.
 *
 * Some calculators (like TI73/82/83/83+/84+) does not return the real name of the 
 * variable (like L1) but uses a special encoded way. This functions expands the name 
 * and converts it to UTF-16.
 *
 * Dynamic version.
 *
 * Return value: a newly allocated string or NULL if error.
 **/
TIEXPORT4 unsigned short* TICALL ticonv_varname_to_utf16(CalcModel model, const char *src, unsigned char type)
{
	if (src != NULL)
	{
		char * tmp = ticonv_varname_detokenize(model, src, type);
		unsigned short * utf16 = ticonv_charset_ti_to_utf16(model, tmp);
		g_free(tmp);
		return utf16;
	}
	else
	{
		g_critical("%s: an argument is NULL", __FUNCTION__);
		return NULL;
	}
}

/**
 * ticonv_varname_to_utf16_s:
 * @model: a calculator model.
 * @src: a name of variable to detokenize and translate (17 chars max).
 * @dst: a buffer where to placed the result (big enough).
 * @vartype: the type of variable.
 *
 * Some calculators (like TI73/82/83/83+/84+) does not return the real name of the 
 * variable (like L1) but uses a special encoded way. This functions expands the name 
 * and converts it to UTF-16.
 * 
 * Static version.
 *
 * Return value: the %dst string or NULL if error.
 **/
TIEXPORT4 unsigned short* TICALL ticonv_varname_to_utf16_s(CalcModel model, const char *src, unsigned short *dst, unsigned char type)
{
	if (src != NULL && dst != NULL)
	{
		char * tmp = ticonv_varname_detokenize(model, src, type);
		ticonv_charset_ti_to_utf16_s(model, tmp, dst);
		g_free(tmp);
		return dst;
	}
	else
	{
		g_critical("%s: an argument is NULL", __FUNCTION__);
		return NULL;
	}
}

/**
 * ticonv_varname_to_utf8:
 * @model: a calculator model.
 * @src: a name of variable to detokenize and translate.
 * @vartype: the type of variable.
 *
 * Some calculators (like TI73/82/83/83+/84+) does not return the real name of the 
 * variable (like L1) but uses a special encoded way. This functions expands the name 
 * and converts it to UTF-16.
 *
 * Dynamic version.
 *
 * Return value: a newly allocated string or NULL if error.
 **/
TIEXPORT4 char* TICALL ticonv_varname_to_utf8(CalcModel model, const char *src, unsigned char type)
{
	if (src != NULL)
	{
		unsigned short * utf16 = ticonv_varname_to_utf16(model, src, type);
		gchar * utf8 = ticonv_utf16_to_utf8(utf16);
		g_free(utf16);
		return utf8;
	}
	else
	{
		g_critical("%s: src is NULL", __FUNCTION__);
		return NULL;
	}
}

/**
 * ticonv_varname_to_utf8_s:
 * @model: a calculator model.
 * @src: a name of variable to detokenize and translate (17 chars max).
 * @dst: a buffer where to placed the result (big enough).
 * @vartype: the type of variable.
 *
 * Some calculators (like TI73/82/83/83+/84+) does not return the real name of the 
 * variable (like L1) but uses a special encoded way. This functions expands the name 
 * and converts it to UTF-16.
 *
 * Static version.
 *
 * Return value: the %dst string or NULL if error.
 **/
TIEXPORT4 char* TICALL ticonv_varname_to_utf8_s(CalcModel model, const char *src, char *dst, unsigned char type)
{
	if (src != NULL && dst != NULL)
	{
		char * tmp = ticonv_varname_to_utf8(model, src, type);
		strcpy(dst, tmp);
		g_free(tmp);
		return dst;
	}
	else
	{
		g_critical("%s: an argument is NULL", __FUNCTION__);
		return NULL;
	}
}

/**
 * ticonv_varname_to_filename:
 * @model: a calculator model taken in #CalcModel.
 * @src: the name of variable to convert (raw/binary name).
 *
 * This function converts a varname into a valid filename (depends on locale).
 * Example: 'foobar' => foobar, 'alpha' => _alpha_/alpha.
 * 
 * Greeks characters need conversion if the locale is not UTF-8 (Windows for sure, Linux
 * if locale is different of UTF-8) because greek characters are often missed or mis-converted
 * when converting to locale.
 *
 * Return value: %dst as a newly allocated string or NULL if error.
 **/ 
TIEXPORT4 char* TICALL ticonv_varname_to_filename(CalcModel model, const char *src, unsigned char type)
{
	if (src != NULL)
	{
		unsigned short * utf16 = ticonv_varname_to_utf16(model, src, type);
		char * gfe = ticonv_utf16_to_gfe(model, utf16);
		g_free(utf16);
		return gfe;
	}
	else
	{
		g_critical("%s: src is NULL", __FUNCTION__);
		return NULL;
	}
}

/**
 * ticonv_varname_to_filename_s:
 * @model: a calculator model taken in #CalcModel.
 * @src: the name of variable to convert (raw/binary name).
 * @dst: a buffer to place result in the GLib filename encoding (64 bytes max).
 *
 * This function converts a varname into a valid filename (depends on locale).
 * Example: 'foobar' => foobar, 'alpha' => _alpha_/alpha.
 * 
 * Greeks characters need conversion if the locale is not UTF-8 (Windows for sure, Linux
 * if locale is different of UTF-8) because greek characters are often missed or mis-converted
 * when converting to locale.
 *
 * Return value: %dst or NULL if error.
 **/
TIEXPORT4 char* TICALL ticonv_varname_to_filename_s(CalcModel model, const char *src, char *dst, unsigned char type)
{
	if (src != NULL && dst != NULL)
	{
		char * tmp = ticonv_varname_to_filename(model, src, type);
		strcpy(dst, tmp);
		g_free(tmp);
		return dst;
	}
	else
	{
		g_critical("%s: an argument is NULL", __FUNCTION__);
		return NULL;
	}
}

/**
 * ticonv_varname_to_tifile:
 * @model: a calculator model taken in #CalcModel.
 * @src: the name of variable to convert (raw/binary name).
 *
 * This function converts a raw varname into a TI file varname.
 * Needed because USB hand-helds use TI-UTF-8 while TI files are still encoded in
 * raw varname encoding.
 *
 * Return value: %dst as a newly allocated string or NULL if error.
 **/ 
TIEXPORT4 char* TICALL ticonv_varname_to_tifile(CalcModel model, const char *src, unsigned char type)
{
	unsigned short *utf16;
	char *ti;
	char *dst;

	if (src == NULL)
	{
		g_critical("%s: src is NULL", __FUNCTION__);
		return NULL;
	}

	dst = g_strdup(src);

	return dst;
}

/**
 * ticonv_varname_to_tifile_s:
 * @model: a calculator model taken in #CalcModel.
 * @src: the name of variable to convert (raw/binary name).
 * @dst: the location where to place the result (big enough).
 *
 * This function converts a raw varname into a TI file varname.
 * Needed because USB hand-helds use TI-UTF-8 while TI files are still encoded in
 * raw varname encoding.
 *
 * Return value: %dst as a newly allocated string or NULL if error.
 **/ 
TIEXPORT4 char* TICALL ticonv_varname_to_tifile_s(CalcModel model, const char *src, char *dst, unsigned char type)
{
	if (src != NULL && dst != NULL)
	{
		char * tmp = ticonv_varname_to_tifile(model, src, type);
		strcpy(dst, tmp);
		g_free(tmp);
		return dst;
	}
	else
	{
		g_critical("%s: an argument is NULL", __FUNCTION__);
		return NULL;
	}
}

/**
 * ticonv_varname_from_tifile:
 * @model: a calculator model taken in #CalcModel.
 * @src: the name of variable to convert (raw/binary name).
 *
 * This function converts a raw varname into a TI file varname.
 * Needed because USB hand-helds use TI-UTF-8 while TI files are still encoded in
 * raw varname encoding.
 *
 * Return value: %dst as a newly allocated string or NULL if error.
 **/ 
TIEXPORT4 char* TICALL ticonv_varname_from_tifile(CalcModel model, const char *src, unsigned char type)
{
	unsigned short *utf16;
	char *ti;
	char *dst;

	if (src == NULL)
	{
		g_critical("%s: src is NULL", __FUNCTION__);
		return NULL;
	}

	dst = g_strdup(src);

	return dst;
}

/**
 * ticonv_varname_from_tifile_s:
 * @model: a calculator model taken in #CalcModel.
 * @src: the name of variable to convert (raw/binary name).
 * @dst: the location where to place the result (big enough).
 *
 * This function converts a raw varname into a TI file varname.
 * Needed because USB hand-helds use TI-UTF-8 while TI files are still encoded in
 * raw varname encoding.
 * 
 * Return value: %dst as a newly allocated string or NULL if error.
 **/ 
TIEXPORT4 char* TICALL ticonv_varname_from_tifile_s(CalcModel model, const char *src, char *dst, unsigned char type)
{
	if (src != NULL && dst != NULL)
	{
		char * tmp = ticonv_varname_from_tifile(model, src, type);
		strcpy(dst, tmp);
		g_free(tmp);
		return dst;
	}
	else
	{
		g_critical("%s: an argument is NULL", __FUNCTION__);
		return NULL;
	}
}
