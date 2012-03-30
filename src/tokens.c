/* Hey EMACS -*- linux-c -*- */
/* $Id: tokens.c 4418 2011-11-22 20:21:59Z debrouxl $ */

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
	This unit contains raw varname to TI-charset conversion routines.
 
	The detokenization is used to translate some raw varnames into TI-charset 
	encoded varnames. Tokenization is the reverse way. 
	Many functions depends on the calculator model and the variable type ID.

	This is needed for the following calcs: 73/82/83/83+/84+.
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <glib.h>

#include "ticonv.h"

//---

#define MAXCHARS	((8+1+8+1) * 4)

//---

/**
 * ticonv_varname_detokenize:
 * @model: hand-held model
 * @src: binary string to detokenize
 *
 * This function translates a binary variable name (as used on TI8x) into a human readable one.
 *
 * Return value: a newly allocated string. Must be freed when no longer used.
 **/
TIEXPORT4 char* TICALL ticonv_varname_detokenize(CalcModel model, const char *src, unsigned char type)
{
	char *dst;

	if (src == NULL)
	{
		return g_strdup("________");
	}

	switch (model) 
	{
	case CALC_NSPIRE:
		return g_strdup(src);
	default:
		return g_strdup("________");
	}
}

//---

//FIXME: does not work with named list because we should pass the vartype, too
/**
 * ticonv_varname_tokenize:
 * @model: hand-held model
 * @src: binary string to tokenize
 *
 * This function tries andtranslates a human readable variable name into a binary name (as used on TI8x).
 *
 * Return value: a newly allocated string. Must be freed when no longer used.
 **/
TIEXPORT4 char* TICALL ticonv_varname_tokenize(CalcModel model, const char *src_, unsigned char type)
{
	const unsigned char *src = (const unsigned char *)src_;

	if (src == NULL)
	{
		return g_strdup("");
	}

	switch(model)
	{
		default: 
		break;
	}

	return g_strdup(src_);
}
