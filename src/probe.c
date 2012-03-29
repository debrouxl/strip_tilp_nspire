/* Hey EMACS -*- linux-c -*- */
/* $Id: probe.c 4412 2011-11-12 12:46:08Z debrouxl $ */

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
	Probing support.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ticalcs.h"
#include "logging.h"
#include "dbus_pkt.h"
#include "error.h"
#include "gettext.h"
#include "pause.h"
#include "calc_xx.h"
#include "dusb_vpkt.h"
#include "dusb_cmd.h"

#define DEAD_TIME	250

/* 
	Get the first byte sent by the calc (Machine ID)
*/
static int tixx_recv_ACK(CalcHandle* handle, uint8_t* mid)
{
	uint8_t host, cmd;
	uint16_t length;
	uint8_t buffer[5];

	TRYF(dbus_recv_2(handle, &host, &cmd, &length, buffer));
	ticalcs_info(" TI->PC: ACK");

	*mid = host;

	if (cmd == CMD_SKP)
		return ERR_VAR_REJECTED;

	return 0;
}

/**
 * ticalcs_probe_calc_2:
 * @handle: a previously allocated handle
 * @type: the calculator model
 *
 * This function tries and detect the calculator type for non-silent models
 * by requesting a screedump and analyzing the Machine ID. 
 * It supposes your calc is on and plugged.
 * 
 * PC: 08  6D 00 00		PC request a screen dump
 * TI: MId 56 00 00		TI reply OK
 *
 * Beware: the call sequence is very important: 86, 85, 73, 83, 82 !!!
 *
 * Return value: 0 if successful, an error code otherwise.
 **/
static int ticalcs_probe_calc_2(CalcHandle* handle, CalcModel* model)
{
	CalcHandle* h = handle;
	int err;
	uint8_t data;

	ticalcs_info(_("Probing calculator...\n"));
	*model = CALC_NONE;

	/* Test for a TI86 before a TI85 */
	ticalcs_info(_("Check for TI86... "));
	TRYF(dbus_send(h, PC_TI86, CMD_SCR, 2, NULL));
	err = tixx_recv_ACK(h, &data);

	ticalcs_info("<%02X-%02X> ", PC_TI86, data);

	if (!err && (data == TI86_PC)) 
	{
		ticalcs_info("OK !\n");
		*model = CALC_TI86;

		return 0;
	} 
	else 
	{
		ticalcs_info("NOK.\n");
		ticables_cable_reset(handle->cable);
		PAUSE(DEAD_TIME);
	}

	/* Test for a TI85 */
	ticalcs_info(_("Check for TI85... "));
	TRYF(dbus_send(h, PC_TI85, CMD_SCR, 2, NULL));
	err = tixx_recv_ACK(h, &data);

	ticalcs_info("<%02X-%02X> ", PC_TI85, data);

	if (!err && (data == TI85_PC)) 
	{
		ticalcs_info("OK !\n");
		*model = CALC_TI85;

		return 0;
	} 
	else 
	{
		ticalcs_info("NOK.\n");
		ticables_cable_reset(handle->cable);
		PAUSE(DEAD_TIME);
	}

	/* Test for a TI73 before a TI83 */
	ticalcs_info(_("Check for TI73... "));
	TRYF(dbus_send(h, PC_TI73, CMD_SCR, 2, NULL));
	err = tixx_recv_ACK(h, &data);

	ticalcs_info("<%02X-%02X> ", PC_TI73, data);

	if (!err && (data == TI73_PC)) 
	{
		ticalcs_info("OK !\n");
		*model = CALC_TI73;

		return 0;
	} 
	else 
	{
		ticalcs_info("NOK.\n");
		ticables_cable_reset(handle->cable);
		PAUSE(DEAD_TIME);
	}

	/* Test for a TI83 before a TI82 */
	ticalcs_info(_("Check for TI83... "));
	TRYF(dbus_send(h, PC_TI83, CMD_SCR, 2, NULL));
	err = tixx_recv_ACK(h, &data);

	ticalcs_info("<%02X-%02X> ", PC_TI83, data);

	if (!err && (data == TI83_PC)) 
	{
		ticalcs_info("OK !\n");
		*model = CALC_TI83;

		return 0;
	} 
	else 
	{
		ticalcs_info("NOK.\n");
		ticables_cable_reset(handle->cable);
		PAUSE(DEAD_TIME);
	}

	/* Test for a TI82 */
	ticalcs_info(_("Check for TI82... "));
	TRYF(dbus_send(h, PC_TI83, CMD_SCR, 2, NULL));
	err = tixx_recv_ACK(h, &data);

	ticalcs_info("<%02X-%02X> ", PC_TI82, data);

	if (!err && (data == TI82_PC)) 
	{
		ticalcs_info("OK !\n");
		*model = CALC_TI82;

		return 0;
	} 
	else 
	{
		ticalcs_info("NOK.\n");
		ticables_cable_reset(handle->cable);
		PAUSE(DEAD_TIME);
	}

	return (*model == CALC_NONE) ? ERR_NO_CALC : 0;
}

/**
 * ticalcs_probe_calc_1:
 * @handle: a previously allocated handle
 * @type: the calculator model
 *
 * Check if the calculator is ready and detect the type.
 * Works only on FLASH calculators with an AMS2.08 or OS2.00 by requesting the
 * version. A previous version was based on MID but TI83+/84+, TI89/TI89t, TI92+/V200 
 * could not be distinguished ;-(
 *
 * Return value: 0 if successful, an error code otherwise.
 **/
static int ticalcs_probe_calc_1(CalcHandle* handle, CalcModel* model)
{
	uint8_t host, cmd;
	uint16_t status;
	uint8_t buffer[256];
	int i, err;
	CalcInfos infos;

	// init value
	*model = CALC_NONE;

	// test for FLASH hand-helds (00 68 00 00 -> XX 56 00 00)
	// where XX is 0x98: TI89/89t, 0x88: TI92+/V200, 0x73: TI83+/84+, 0x74: TI73
	ticalcs_info(_("Check for TIXX... "));
	for(i = 0; i < 2; i++)
	{		
		ticalcs_info(" PC->TI: RDY?");
		err = dbus_send(handle, PC_TIXX, CMD_RDY, 2, NULL);
		if(err) continue;

		err = dbus_recv_2(handle, &host, &cmd, &status, buffer);
		ticalcs_info(" TI->PC: ACK");
		if(err) continue;

		break;
	}

	// test for TI73
	if(!err)
	{
		if(host == TI73_PC)
		{
			*model = CALC_TI73;
			return 0;
		}
		else if(host == TI92_PC)
		{
			*model = CALC_TI92;
			return 0;
		}
	}
	
	// test for TI92 (09 68 00 00 -> 89 56 00 00)
	else if(err)
	{
		ticalcs_info(_("Check for TI92... "));
		ticables_cable_reset(handle->cable);
		PAUSE(DEAD_TIME);	// needed !

		for(i = 0; i < 2; i++)
		{
			ticalcs_info(" PC->TI: RDY?");
			err = dbus_send(handle, PC_TI92, CMD_RDY, 2, NULL);
			if(err) continue;

			err = dbus_recv_2(handle, &host, &cmd, &status, buffer);
			ticalcs_info(" TI->PC: ACK");
			if(err) continue;

			break;
		}

		if(!err)
			*model = CALC_TI92;
	}

	if (cmd != CMD_ACK)
		return ERR_INVALID_CMD;

	if ((status & 1) != 0)
		return ERR_NOT_READY;
	
	// test for TI9x FLASH hand-helds again (request version and analyze HW_ID)
	if(!err && (host != TI73_PC) && (host != TI83p_PC))
	{
		ticalcs_info(_("Check for TI9X... "));

		handle->model = CALC_TI89;
		handle->calc = (CalcFncts *)&calc_89;

		memset(&infos, 0, sizeof(CalcInfos));		
		TRYF(ticalcs_calc_get_version(handle, &infos));
		*model = infos.model;
	}
	else
	{
		ticalcs_info(_("Check for TI8X... "));

		handle->model = CALC_TI83P;
		handle->calc = (CalcFncts *)&calc_83p;

		memset(&infos, 0, sizeof(CalcInfos));
		TRYF(ticalcs_calc_get_version(handle, &infos));
		*model = infos.model;
	}

	ticalcs_info(_("Calculator type: %s"), tifiles_model_to_string(*model));

	return (*model == CALC_NONE) ? ERR_NO_CALC : 0;
}

extern const CalcUpdate default_update;

/**
 * ticalcs_probe_calc:
 * @cable: a valid (=opened/attached) link cable handle
 * @model: the calculator model which have been detected
 *
 * This function attempts to detect the calculator model plugged onto the cable.
 * It works in a heuristic fashion.
 *
 * Return value: 0 if successful, an error code otherwise.
 **/
TIEXPORT3 int TICALL ticalcs_probe_calc (CableHandle* cable, CalcModel* model)
{
	CalcHandle calc;
	int err = 0;

	if (cable == NULL)
	{
		return ERR_INVALID_HANDLE;
	}
	if (model == NULL)
	{
		ticalcs_critical("ticalcs_probe_calc: model is NULL");
		return -1;
	}

	// Hack: we construct the structure here because we don't really need it.
	// I want to use ticalcs functions with a non-fixed calculator
	memset(&calc, 0, sizeof(CalcHandle));
	calc.model = *model = CALC_NONE;
	calc.updat = (CalcUpdate *)&default_update;
	calc.priv2 = (uint8_t *)g_malloc(65536 + 4);
	calc.cable = cable;
	calc.open = !0;

	// first: search for FLASH hand-helds (fast)
	err = ticalcs_probe_calc_1(&calc, model);
	if(!err && (*model != CALC_NONE))
	{
		g_free(calc.priv2);
		return 0;
	}

	// second: search for other calcs (slow)
	err = ticalcs_probe_calc_2(&calc, model);
	if(err)
	{
		g_free(calc.priv2);
		return err;
	}

	g_free(calc.priv2);
	return (*model == CALC_NONE) ? ERR_NO_CALC : 0;
}

/**
 * ticalcs_probe_usb_calc:
 * @cable: a valid (=opened/attached) link cable handle
 * @model: the calculator model which have been detected
 *
 * This function attempts to detect the calculator model plugged onto the cable.
 * It works in a heuristic fashion and with FLASH hand-helds only.
 *
 * Return value: 0 if successful, an error code otherwise.
 **/
TIEXPORT3 int TICALL ticalcs_probe_usb_calc(CableHandle* cable, CalcModel* model)
{
	CalcHandle calc;
	int err = 0;
	int ret = ERR_NO_CALC;

	if (cable == NULL)
	{
		return ERR_INVALID_HANDLE;
	}
	if (model == NULL)
	{
		ticalcs_critical("ticalcs_probe_calc: model is NULL");
		return -1;
	}

	// Hack: we construct the structure here because we don't really need it.
	// I want to use ticalcs functions with a non-fixed calculator
	memset(&calc, 0, sizeof(CalcHandle));
	calc.model = *model = CALC_NONE;
	calc.updat = (CalcUpdate *)&default_update;
	calc.priv2 = (uint8_t *)g_malloc(65536 + 4);
	calc.cable = cable;
	calc.open = !0;

	if(cable->model == CABLE_SLV)
	{
		err = ticalcs_probe_calc_1(&calc, model);
		if(!err && (*model != CALC_NONE))
			ret = 0;
	}
	else if(cable->model == CABLE_USB)
	{
		int n, *list;
		int i;

		ticables_get_usb_devices(&list, &n);
		i = (int)cable->port-1 > n ? n-1 : (int)cable->port-1;
		switch(list[i])
		{
		case PID_TI89TM:   *model = CALC_TI89T_USB; ret = 0; break;
		case PID_TI84P:    *model = CALC_TI84P_USB; ret = 0; break;
		case PID_TI84P_SE: *model = CALC_TI84P_USB; ret = 0; break;
		case PID_NSPIRE:   *model = CALC_NSPIRE; ret = 0; break;
		default: break;
		}
	}

	g_free(calc.priv2);
	return ret;
}

/**
 * ticalcs_probe:
 * @c_model: link cable model
 * @c_port: link cable port
 * @model: hand-held model detected/found [out]
 * @all: which hand-helds to detect (FLASH only or all)
 *
 * This function attempts to detect the calculator model plugged onto a
 * given link cable model/port. It works in a heuristic fashion.
 * This function handles device opening/closing for you.
 *
 * Return value: 0 if successful, an error code otherwise.
 **/
TIEXPORT3 int TICALL ticalcs_probe(CableModel c_model, CablePort c_port, CalcModel* model, int all)
{
	CableHandle *handle;
	int err = 0;
	CalcHandle calc;

	if (model == NULL)
	{
		ticalcs_critical("ticalcs_probe_calc: model is NULL");
		return -1;
	}

	// get handle
	handle = ticables_handle_new(c_model, c_port);
	ticables_options_set_timeout(handle, 10);

	// hack: we construct the structure here because we don't really need it.
	// I want to use ticalcs functions with a non-fixed calculator
	memset(&calc, 0, sizeof(CalcHandle));
	calc.model = *model = CALC_NONE;
	calc.updat = (CalcUpdate *)&default_update;
	calc.priv2 = (uint8_t *)g_malloc(65536 + 4);
	calc.cable = handle;
	calc.open = !0;

	// open cable
	err = ticables_cable_open(handle);
	if(err)
	{
		ticables_handle_del(handle);
		return err;
	}

	// probe
	if(c_model == CABLE_USB)
		err = ticalcs_probe_usb_calc(handle, model);
	else
	{
		if(all)
			err = ticalcs_probe_calc(handle, model);
		else
			err = ticalcs_probe_calc_1(&calc, model);
	}

	if(err)
	{
		ticables_cable_close(handle);
		ticables_handle_del(handle);
		return err;
	}

	// close
	ticables_cable_close(handle);
	ticables_handle_del(handle);

	return err;
}


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gettext.h"
#include "logging.h"
#include "ticables.h"
#include "error.h"

#include "linux/detect.h"
#include "win32/detect.h"
#include "bsd/detect.h"

static void ticables_probing_show(int **array)
{
	CableModel model;

	for(model = CABLE_NUL; model < CABLE_MAX; model++)
	{
		ticables_info(_(" %i: %i %i %i %i"), model, array[model][1], array[model][2], array[model][3], array[model][4]);
	}
}

/**
 * ticables_probing_do:
 * @result: address of an array of integers to put the result.
 * @timeout: timeout to set during probing
 * @method: defines which link cables you want to search for.
 *
 * Returns cables which have been detected. All cables should be closed before !
 * The array is like a matrix which contains 5 columns (PORT_0 to PORT_4) and 
 * 7 lines (CABLE_GRY to CABLE_USB).
 * The array must be freed by #ticables_probing_finish when no longer used.
 *
 * Return value: 0 if successful, ERR_NO_CABLE if no cables found.
 **/
TIEXPORT1 int TICALL ticables_probing_do(int ***result, int timeout, ProbingMethod method)
{
	CablePort port;
	CableModel model;
	int **array;
	int found = 0;

	if (result == NULL)
	{
		ticables_critical("%s: result is NULL", __FUNCTION__);
		return ERR_PROBE_FAILED;
	}

	ticables_info(_("Link cable probing:"));

	array = (int **)calloc(CABLE_MAX + 1, sizeof(int *));
	for(model = CABLE_NUL; model <= CABLE_MAX; model++)
	    array[model] = (int *)calloc(5, sizeof(int));

	// look for USB devices (faster)
	if(method & PROBE_USB)
	{
		int *list, n, i;

		ticables_get_usb_devices(&list, &n);

		for(i = 0; i < n; i++)
		{
			port = i+1;

			if(list[i] == PID_TIGLUSB)
				array[CABLE_SLV][port] = !0;

			if(list[i])
				array[CABLE_USB][port] = !0;

			if(list[i])
				found = !0;
		}
	}

	if((method & PROBE_FIRST) && found)
	{
		*result = array;
		return found ? 0 : ERR_NO_CABLE_1;
	}

	// look for DBUS devices (slower)
	if(method & PROBE_DBUS)
	{
		for(model = CABLE_GRY; model <= CABLE_PAR; model++)
		{
			for(port = PORT_1; port <= PORT_4; port++)
			{
				CableHandle* handle;
				int err, ret;

				handle = ticables_handle_new(model, port);
				if(handle)
				{
					ticables_options_set_timeout(handle, timeout);
					err = ticables_cable_probe(handle, &ret);
					array[model][port] = (ret && !err) ? 1: 0;
					if(array[model][port]) found = !0;

					if(found && (method & PROBE_FIRST))
					{
						ticables_handle_del(handle);
						break;
					}
				}
				ticables_handle_del(handle);
			}
		}
	}

	*result = array;
	return found ? 0 : ERR_NO_CABLE_1;
}

/**
 * ticables_probing_finish:
 * @result: address of an array of integers. 
 *
 * Free the array created by #ticables_probing_do.
 *
 * Return value: always 0.
 **/
TIEXPORT1 int TICALL ticables_probing_finish(int ***result)
{
	int i;

	if (result != NULL && *result != NULL)
	{
		for(i = CABLE_GRY; i <= CABLE_TIE; i++)
		{
			free((*result)[i]);
			(*result)[i] = NULL;
		}

		free(*result);
		*result = NULL;
	}
	else
	{
		ticables_critical("%s(NULL)", __FUNCTION__);
	}

	return 0;
}

/**
 * ticables_is_usb_enabled:
 *
 * Checks whether USB support is available. Can be called at any time.
 *
 * Return value: !0 if available, 0 otherwise.
 **/
TIEXPORT1 int TICALL ticables_is_usb_enabled(void)
{
#if defined(__WIN32__)
	return !win32_check_libusb();
#elif defined(__LINUX__) && (defined(HAVE_LIBUSB) || defined(HAVE_LIBUSB_1_0))
	return !linux_check_libusb();
#elif (defined(HAVE_LIBUSB) || defined(HAVE_LIBUSB_1_0))
	return 1;
#else
	return 0;
#endif

	return 0;
}

extern int usb_probe_devices(int **list);

/**
 * ticables_get_usb_devices:
 * @array: address of a NULL-terminated allocated array of integers (PIDs).
 * @length: number of detected USB devices.
 *
 * Returns the list of detected USB PIDs. Note that list is in the 
 * same order as PORT#x.
 * The array must be freed when no longer used.
 *
 * Return value: 0 if successful, an error code otherwise.
 **/
TIEXPORT1 int TICALL ticables_get_usb_devices(int **list, int *len)
{
	if (list != NULL)
	{
#if defined(__WIN32__) || (defined(HAVE_LIBUSB) || defined(HAVE_LIBUSB_1_0))
		int i, *p;
		int ret = 0;

		ret = usb_probe_devices(list);
		if(ret)
		{
			*list = calloc(1, sizeof(int));
			if(len) *len = 0;
			return ret;
		}

		for(p = *list, i = 0; *p; p++, i++);
			//printf("%i: %04x\n", i, (*list)[i]);

		if(len) *len = i;
#else
		*list = calloc(1, sizeof(int));
		if(len) *len = 0;
#endif
	}
	else
	{
		ticables_critical("%s: list is NULL", __FUNCTION__);
		return -1;
	}

	return 0;
}
