/* Hey EMACS -*- linux-c -*- */
/* $Id: clock.c 4412 2011-11-12 12:46:08Z debrouxl $ */

/*  libticalcs - Ti Calculator library, a part of the TiLP project
 *  Copyright (C) 1999-2005  Romain Liévin
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
	Clock support for AMS >= 2.08.
*/

#include <string.h>

#include "ticalcs.h"
#include "logging.h"

#ifdef __WIN32__
#define strcasecmp _stricmp
#endif

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <string.h>
#include <time.h>

#include "support.h"
#include "clock.h"
#include "tilp_core.h"

static GladeXML *xml;
static gboolean modified;
static CalcClock tmp_clk;

static void update_fields(const CalcClock* clk)
{
	GtkWidget *data;

	data = glade_xml_get_widget(xml, "spinbutton1");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(data), clk->day);
	
	data = glade_xml_get_widget(xml, "combobox1");
	gtk_combo_box_set_active(GTK_COMBO_BOX(data), clk->month - 1);

	data = glade_xml_get_widget(xml, "spinbutton3");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(data), clk->year);

	data = glade_xml_get_widget(xml, "spinbutton4");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(data), clk->hours);
	
	data = glade_xml_get_widget(xml, "spinbutton5");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(data), clk->minutes);
	
	data = glade_xml_get_widget(xml, "spinbutton6");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(data), clk->seconds);
	
	data = glade_xml_get_widget(xml, "radiobutton1");
	if(clk->time_format == 12)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), TRUE);

	data = glade_xml_get_widget(xml, "radiobutton2");
	if(clk->time_format == 24)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), TRUE);

	data = glade_xml_get_widget(xml, "combobox2");
	gtk_combo_box_set_active(GTK_COMBO_BOX(data), clk->date_format - 1);
}

gint display_clock_dbox()
{
	GtkWidget *dbox;
	gint result;
	int err;

	if(!(ticalcs_calc_features(calc_handle) & OPS_CLOCK))
	{
		gif->msg_box1(_("Error"), _("No clock support!"));
		return -1;
	}

	err = ticalcs_calc_isready(calc_handle);
	if(err)
	{
		tilp_err(err);
		return -1;
	}

	if(tilp_calc_check_version("2.08") < 0)
		return -1;

	err = ticalcs_calc_get_clock(calc_handle, &tmp_clk);
	if(err)
	{
		tilp_err(err);
		return -1;
	}

	tilp_info("%02i/%02i/%02i %02i:%02i:%02i, %s, %s\n", tmp_clk.day,
		tmp_clk.month, tmp_clk.year, tmp_clk.hours,
		tmp_clk.minutes, tmp_clk.seconds,
		(tmp_clk.time_format == 12) ? "12" : "24",
		ticalcs_clock_format2date(options.calc_model, tmp_clk.date_format));

	xml = glade_xml_new(tilp_paths_build_glade("clock-2.glade"), "clock_dbox", PACKAGE);
	if(!xml)
		g_error(_("action.c: GUI loading failed !\n"));
	glade_xml_signal_autoconnect(xml);

	dbox = glade_xml_get_widget(xml, "clock_dbox");
	gtk_dialog_set_alternative_button_order(GTK_DIALOG(dbox), GTK_RESPONSE_OK,
	                                        GTK_RESPONSE_CANCEL,-1);
	update_fields(&tmp_clk);
	modified = FALSE;

	result = gtk_dialog_run(GTK_DIALOG(dbox));
	switch (result) 
	{
	case GTK_RESPONSE_OK:
		if(modified == FALSE)
			break;

		tilp_info("%02i/%02i/%02i %02i:%02i:%02i, %s, %s\n",
			tmp_clk.day, tmp_clk.month, tmp_clk.year,
			tmp_clk.hours, tmp_clk.minutes, tmp_clk.seconds,
			(tmp_clk.time_format == 12) ? "12" : "24",
			ticalcs_clock_format2date(options.calc_model, tmp_clk.date_format));

		err = ticalcs_calc_isready(calc_handle);
		if(err)
		{
			tilp_err(err);
			return -1;
		}

		err = ticalcs_calc_set_clock(calc_handle, &tmp_clk);
		if(err)
		{
			tilp_err(err);
			return -1;
		}

		modified = FALSE;
		break;
	case GTK_RESPONSE_HELP:
		break;
	default:
		break;
	}
	gtk_widget_destroy(dbox);

	return 0;
}

GLADE_CB void
clock_radiobutton3_toggled(GtkToggleButton * togglebutton,
			   gpointer user_data)
{
	tmp_clk.state = 1;
} 

GLADE_CB void
clock_radiobutton4_toggled(GtkToggleButton * togglebutton,
			   gpointer user_data)
{
	tmp_clk.state = 0;
} 

GLADE_CB void
clock_radiobutton1_toggled(GtkToggleButton * togglebutton,
			   gpointer user_data)
{
	tmp_clk.time_format = 12;
	modified = TRUE;
} 

GLADE_CB void
clock_radiobutton2_toggled(GtkToggleButton * togglebutton,
			   gpointer user_data)
{
	tmp_clk.time_format = 24;
	modified = TRUE;
} 

GLADE_CB void
on_clock_combobox1_changed             (GtkComboBox     *combobox,
                                        gpointer         user_data)
{
	gint nitem = gtk_combo_box_get_active(combobox);
	tmp_clk.month = nitem + 1;
}

GLADE_CB void
on_clock_combobox2_changed             (GtkComboBox     *combobox,
                                        gpointer         user_data)
{
	gint nitem = gtk_combo_box_get_active(combobox);
	tmp_clk.date_format = nitem + 1;
}

GLADE_CB void clock_spinbutton3_changed(GtkEditable * editable,
					gpointer user_data)
{
	tmp_clk.year =
	    gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(user_data));
	modified = TRUE;
} 

GLADE_CB void clock_spinbutton4_changed(GtkEditable * editable,
					  gpointer user_data)
{
	tmp_clk.hours =
	    gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(user_data));
	modified = TRUE;
} 

GLADE_CB void clock_spinbutton5_changed(GtkEditable * editable,
					  gpointer user_data)
{
	tmp_clk.minutes =
	    gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(user_data));
	modified = TRUE;
} 

GLADE_CB void clock_spinbutton6_changed(GtkEditable * editable,
					  gpointer user_data)
{
	tmp_clk.seconds =
	    gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(user_data));
	modified = TRUE;
} 

GLADE_CB void clock_spinbutton1_changed(GtkEditable * editable,
					  gpointer user_data)
{
	tmp_clk.day =
	    gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(user_data));
	modified = TRUE;
} 

GLADE_CB void clock_sync_button_clicked(GtkButton * button,
					  gpointer user_data)
{
	time_t tt;
	struct tm *lt;
	CalcClock tc;
	int err;

	time(&tt);
	lt = localtime(&tt);
	memcpy(&tc, &tmp_clk, sizeof(CalcClock));
	tc.day = lt->tm_mday;
	tc.month = lt->tm_mon + 1;
	tc.year = lt->tm_year + 1900;
	tc.hours = lt->tm_hour;
	tc.minutes = lt->tm_min;
	tc.seconds = lt->tm_sec;

	err = ticalcs_calc_isready(calc_handle);
	if(err)
	{
		tilp_err(err);
		return;
	}

	err = ticalcs_calc_set_clock(calc_handle, &tc);
	if(err)
	{
		tilp_err(err);
		return;
	}

	update_fields(&tc);
	modified = FALSE;
}


#define MAX_FORMAT_89	8
#define MAX_FORMAT_84	3

static const char *TI_CLOCK_89[] = 
{
  "",
  "MM/DD/YY",
  "DD/MM/YY",
  "MM.DD.YY",
  "DD.MM.YY",
  "YY.MM.DD",
  "MM-DD-YY",
  "DD-MM-YY",
  "YY-MM-DD",
  ""
};

static const char *TI_CLOCK_84[] = 
{
  "",
  "M/D/Y",
  "D/M/Y",
  "Y/M/D",
  ""
};

/**
 * ticalcs_clock_format2date:
 * @model: a calculator model
 * @value: a format type
 *
 * Convert a format type into a format string.
 * Example: 1 -> "MM/DD/YY"
 *
 * Return value: a format string.
 **/
TIEXPORT3 const char *TICALL ticalcs_clock_format2date(CalcModel model, int value)
{
	int v;

	if(tifiles_calc_is_ti9x(model))
	{
		if (value < 1)
			v = 1;
		else if (value > MAX_FORMAT_89)
			v = MAX_FORMAT_89;
		else
			v = value;

		return TI_CLOCK_89[v];
	}
	else if(tifiles_calc_is_ti8x(model))
	{
		if (value < 1)
			v = 1;
		else if (value > MAX_FORMAT_84)
			v = MAX_FORMAT_84;
		else
			v = value;

		return TI_CLOCK_84[v];
	}

	return "";
}

/**
 * ticalcs_clock_date2format:
 * @model: a calculator model
 * @format: a format string
 *
 * Convert a format string into a format type.
 * Example: "MM/DD/YY" -> 1
 *
 * Return value: a format string.
 **/
TIEXPORT3 int TICALL ticalcs_clock_date2format(CalcModel model, const char *format)
{
	int i = 1;

	if (format == NULL)
	{
		ticalcs_critical("ticalcs_clock_date2format: format is NULL");
		return 0;
	}

	if(tifiles_calc_is_ti9x(model))
	{
		for (i = 1; i <= MAX_FORMAT_89; i++) 
		{
		if (!strcasecmp(TI_CLOCK_89[i], format))
			break;
		}
		if (i > MAX_FORMAT_89)
			return 1;
	}
	else if(tifiles_calc_is_ti8x(model))
	{
		for (i = 1; i <= MAX_FORMAT_84; i++) 
		{
			if (!strcasecmp(TI_CLOCK_84[i], format))
			break;
		}
		if (i > MAX_FORMAT_84)
			return 1;
	}

	return i;
}

/**
 * ticalcs_clock_show:
 * @model: calc model
 * @clock: a #CalcClock structure
 *
 * Display to stdout the content of the structure.
 *
 * Return value: always 0.
 **/
TIEXPORT3 int TICALL ticalcs_clock_show(CalcModel model, CalcClock* s)
{
	if (s != NULL)
	{
		ticalcs_info("Date: %02i/%02i/%02i", s->day, s->month, s->year);
		ticalcs_info("Time: %02i/%02i/%02i", s->hours, s->minutes, s->seconds);
		ticalcs_info("Time format: %02i", s->time_format);
		ticalcs_info("Date format: %s", ticalcs_clock_format2date(model, s->date_format));
	}

	return 0;
}
