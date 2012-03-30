/* Hey EMACS -*- linux-c -*- */
/* $Id: error.c 4412 2011-11-12 12:46:08Z debrouxl $ */

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
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#ifdef __WIN32__
#include <windows.h>
#endif
#include <glib.h>
#include <errno.h>

#include "gettext.h"
#include "export1.h"
#include "export2.h"
#include "export3.h"
#include "error.h"
#include "logging.h"

/**
 * tifiles_get_error:
 * @number: error number (see file_err.h for list).
 * @message: a newly glib allocated string which contains corresponding error *message.
 *
 * Attempt to match the message corresponding to the error number. The returned
 * string must be freed when no longer needed.
 *
 * Return value: 0 if error has been caught, the error number otherwise (propagation).
 **/
TIEXPORT2 int TICALL tifiles_error_get(FileError number, char **message)
{
	if (message == NULL)
	{
		tifiles_critical("%s(NULL)", __FUNCTION__);
		return number;
	}

	switch (number) 
	{
	case ERR_MALLOC_2:
		*message = g_strconcat(
			_("Msg: unable to allocate memory (malloc)."),
			"\n",
			_("Cause: memory too low ?"), 
			NULL);
    break;

	case ERR_FILE_OPEN_2:
		*message = g_strconcat(
			_("Msg: unable to open file."),
			"\n",
			_("Cause: either the file does not exist, either there is no room."),
			NULL);
    break;

	case ERR_FILE_CLOSE:
		*message = g_strconcat(
			_("Msg: unable to close file."),
			"\n",
			_("Cause: either the file does not exist, either there is no room."),
			NULL);
    break;

	case ERR_GROUP_SIZE:
		*message = g_strconcat(
			_("Msg: the size of a group file cannot exceed 64 KB."),
			"\n",
			_("Cause: too many variables/data."),
			NULL);
    break;

	case ERR_BAD_CALC:
		*message = g_strconcat(
			_("Msg: unknown calculator type."),
			"\n",
			_("Cause: probably due to a bug, mail to: tilp-users@lists.sf.net."),
			NULL);
    break;

	case ERR_INVALID_FILE:
	case ERR_BAD_FILE:
		*message = g_strconcat(
			_("Msg: invalid file."),
			"\n",
			_("Cause: it's probably not a TI formatted file."),
			NULL);
    break;

	case ERR_FILE_CHECKSUM:
		*message = g_strconcat(
			_("Msg: checksum error."),
			"\n",
			_("Cause: the file has an incorrect checksum and may be corrupted."),
			NULL);
    break;

	case ERR_FILE_ZIP:
		*message = g_strconcat(
			_("Msg: (Un)Zip error."),
			"\n",
			_("Cause: unknown..."),
			NULL);
	break;

	case ERR_UNSUPPORTED_2:
		*message = g_strconcat(
			_("Msg: unsupported function."),
			"\n",
			_("Cause: the function you attempted to use is not available."),
			NULL);
		break;

	case ERR_FILE_IO:
		*message = g_strconcat(
			_("Msg: I/O file error."),
			"\n",
			_("Cause: file is corrupted or invalid."),
			NULL);
		break;


	default:
		// propagate error code
		return number;
    break;
	}

	return 0;
}

// extern helpers
extern void nsp_vtl_pkt_purge(void);

extern int nsp_reset;

/**
 * ticalcs_error_get:
 * @number: error number (see error.h for list).
 * @message: a newly glib allocated string which contains corresponding error *message.
 *
 * Attempt to match the message corresponding to the error number. The returned
 * string must be freed when no longer needed.
 * The error message has the following format:
 * - 1: the error message
 * - 2: the cause(s), explanations on how to fix it
 * - 3: the error returned by the system
 *
 * Return value: 0 if error has been caught, the error number otherwise (propagation).
 **/
TIEXPORT3 int TICALL ticalcs_error_get(CalcError number, char **message)
{
	char error_msg[2048];

	// free memory
	nsp_vtl_pkt_purge();

	if (message == NULL)
	{
		ticalcs_critical("ticalcs_error_get(NULL)\n");
		return number;
	}

	switch(number)
	{
	case ERR_ABORT:
		*message = g_strconcat(
		_("Msg: transfer aborted."),
		"\n",
		_("Cause: the user!"),
		NULL);
		break;

	case ERR_NOT_READY:
		*message = g_strconcat(
		_("Msg: calculator is not ready."),
		"\n",
		_("Cause: the calculator is busy (Var-Link menu, Window, ...). Press HOME..."),
		NULL);
		break;

	case ERR_CHECKSUM:
		strcpy(error_msg, _("Msg: checksum error."));
		strcat(error_msg, "\n");
		strcat(error_msg, _("Cause: a transmission error. Try again."));
		*message = g_strdup(error_msg);
		break;

	case ERR_INVALID_HOST:
		strcpy(error_msg, _("Msg: invalid host ID."));
		strcat(error_msg, "\n");
		strcat(error_msg,
		_("Cause: received an unexpected Machine ID from calculator, probably due to a transmission error."));
		*message = g_strdup(error_msg);
		break;

	case ERR_INVALID_TARGET:
		strcpy(error_msg, _("Msg: invalid target ID."));
		strcat(error_msg, "\n");
		strcat(error_msg,
		_("Cause: received an unexpected Target ID from calculator, probably due to a transmission error."));
		*message = g_strdup(error_msg);
		break;

	case ERR_INVALID_CMD:
		strcpy(error_msg, _("Msg: invalid command ID."));
		strcat(error_msg, "\n");
		strcat(error_msg,
		_("Cause: received an unexpected Command ID from calculator, probably due to a transmission error."));
		*message = g_strdup(error_msg);
		break;

	case ERR_EOT:
		strcpy(error_msg, _("Msg: not really an error, should not appear!"));
		strcat(error_msg, "\n");
		strcat(error_msg, _("Cause: a bug. Drop us an email!"));
		*message = g_strdup(error_msg);
		break;

	case ERR_VAR_REJECTED:
		strcpy(error_msg,
		_("Msg: contents has been refused."));
		strcat(error_msg, "\n");
		strcat(error_msg,
		_("Cause: you attempted to send a variable which is locked/archived on the target calculator. The calculator may be out of memory. The certificate may be invalid or already installed."));
		*message = g_strdup(error_msg);
		break;

	case ERR_CTS_ERROR:
		strcpy(error_msg, _("Invalid CTS packet."));
		strcat(error_msg, "\n");
		strcat(error_msg, _("Cause: a transmission error."));
		*message = g_strdup(error_msg);
		break;

	case ERR_NACK:
		strcpy(error_msg, _("NACK received."));
		strcat(error_msg, "\n");
		strcat(error_msg, _("Cause: the command has been refused."));
		*message = g_strdup(error_msg);
		break;

	case ERR_INVALID_PACKET:
		strcpy(error_msg, _("Invalid packet."));
		strcat(error_msg, "\n");
		strcat(error_msg, _("Cause: a transmission error."));
		*message = g_strdup(error_msg);
		break;

	case ERR_MALLOC_3:
		break;

	case ERR_NO_CABLE_3:
		strcpy(error_msg, _("The cable cannot be used."));
		strcat(error_msg, "\n");
		strcat(error_msg, _("Cause: the cable has not been initialized due to a previous/current error."));
		*message = g_strdup(error_msg);
		break;

	case ERR_BUSY_3:
		break;

	case ERR_FILE_OPEN_3:
		break;

	case ERR_VOID_FUNCTION:
	case ERR_UNSUPPORTED_3:
		strcpy(error_msg,
		_("Msg: this function does not exist for this calculator."));
		strcat(error_msg, "\n");
		strcat(error_msg,
		_("Cause: the operation you attempted is not supported by your calculator model."));
		*message = g_strdup(error_msg);
		break;

	case ERR_OUT_OF_MEMORY:
		strcpy(error_msg, _("Msg: calculator out of memory."));
		strcat(error_msg, "\n");
		strcat(error_msg,
		_("Cause: the calculator does not have memory enough for doing the operation."));
		*message = g_strdup(error_msg);
		break;

	case ERR_OPEN_FILE:
		strcpy(error_msg, _("Msg: unable to open (reading) the file."));
		strcat(error_msg, "\n");
		strcat(error_msg, _("Cause: disk space or permissions."));
		*message = g_strdup(error_msg);
		break;

	case ERR_SAVE_FILE:
		strcpy(error_msg, _("Msg: unable to open (writing) the file."));
		strcat(error_msg, "\n");
		strcat(error_msg, _("Cause: disk space or permissions."));
		*message = g_strdup(error_msg);
		break;

	case ERR_PENDING_TRANSFER:
		strcpy(error_msg,
		_("A transfer is in progress. You cannot start a new one."));
		strcat(error_msg, "\n");
		strcat(error_msg,
		_("Cause: this should not happen. Probably a bug: drop us an email!"));
		*message = g_strdup(error_msg);
		break;

	case ERR_FATAL_ERROR:
		break;

	case ERR_MISSING_VAR:
		strcpy(error_msg, _("The requested var does not exist."));
		strcat(error_msg, "\n");
		strcat(error_msg,
		_("Cause: you attempted to download a variable which does not exist on the calculator any longer. Maybe you have manually deleted it ?"));
		*message = g_strdup(error_msg);
		break;

	case ERR_NO_VARS:
		strcpy(error_msg, _("There is nothing to backup."));
		strcat(error_msg, "\n");
		strcat(error_msg,
		_("Cause: 0 variables stored onto your calculator."));
		*message = g_strdup(error_msg);
		break;

	case ERR_INVALID_HANDLE:
		strcpy(error_msg, _("Invalid ticalcs handle."));
		strcat(error_msg, "\n");
		strcat(error_msg,
		_("Cause: bad cable or calc model."));
		*message = g_strdup(error_msg);
		break;

	case ERR_CALC_ERROR1:	// must be synchronized with cmd89.c (static uint8_t dbus_errors[])
		strcpy(error_msg, _("Msg: hand-held returned an error."));
		strcat(error_msg, "\n");
		strcat(error_msg,
		_("Cause: hand-held returned an uncaught error. Please report log."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR1+1:
		strcpy(error_msg, _("Msg: cannot delete application."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR1+2:
		strcpy(error_msg, _("Msg: invalid variable name."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR1+3:
		strcpy(error_msg, _("Msg: cannot overwrite variable (locked/archived)."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR1+4:
		strcpy(error_msg, _("Msg: cannot delete variable (locked/archived)."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR1+5:
		strcpy(error_msg, _("Msg: FLASH application rejected (bad target model)."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR1+6:
		strcpy(error_msg, _("Msg: malformed RTS packet (AMS version conflict)."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR1+7:
		strcpy(error_msg, _("Msg: FLASH application rejected (signature does not match)."));
		*message = g_strdup(error_msg);
		break;

	case ERR_CALC_ERROR2:	// must be synchronized with dusb_cmd.c (static uint16_t usb_errors[])
		strcpy(error_msg, _("Msg: hand-held returned an error (not caught)."));
		strcat(error_msg, "\n");
		strcat(error_msg,
		_("Cause: hand-held returned an uncaught error. Please report log."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+1:
		strcpy(error_msg, _("Msg: invalid argument or name."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+2:
		strcpy(error_msg, _("Msg: cannot delete var/app from archive."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+3:
		strcpy(error_msg, _("Msg: transmission error."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+4:
		strcpy(error_msg, _("Msg: using basic mode while being in boot mode."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+5:
		strcpy(error_msg, _("Msg: out of memory."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+6:
		strcpy(error_msg, _("Msg: invalid name."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+7:
		strcpy(error_msg, _("Msg: invalid name."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+8:
		strcpy(error_msg, _("Msg: busy?."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+9:
		strcpy(error_msg, _("Msg: can't overwrite, variable is locked."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+10:
		strcpy(error_msg, _("Msg: mode token too small."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+11:
		strcpy(error_msg, _("Msg: mode token too large."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+12:
		strcpy(error_msg, _("Msg: invalid parameter ID."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+13:
		strcpy(error_msg, _("Msg: remote control?"));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+14:
		strcpy(error_msg, _("Msg: battery low."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+15:
		strcpy(error_msg, _("Msg: FLASH application rejected (signature does not match)."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR2+16:
		strcpy(error_msg, _("Msg: hand-held is busy (set your calculator to HOME screen)."));
		*message = g_strdup(error_msg);
		break;

	case ERR_CALC_ERROR3+0:	// must be synchronized with nsp_cmd.c (static uint8_t usb_errors[])
		strcpy(error_msg, _("Msg: hand-held returned an error (not caught)."));
		strcat(error_msg, "\n");
		strcat(error_msg,
		_("Cause: hand-held returned an uncaught error. Please report log."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR3+1:
		strcpy(error_msg, _("Msg: out of memory."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR3+2:
		strcpy(error_msg, _("Msg: OS installation failed. File is corrupted/wrong."));
		*message = g_strdup(error_msg);
		break;
	// The following one is returned when the filename does not fit in a 254-byte packet.
	case ERR_CALC_ERROR3+3:
		strcpy(error_msg, _("Msg: packet too large."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR3+4:
		strcpy(error_msg, _("Msg: the variable or directory does not exist."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR3+5:
		strcpy(error_msg, _("Msg: the directory name is invalid."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR3+6:
		strcpy(error_msg, _("Msg: not currently listing a directory."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR3+7:
		strcpy(error_msg, _("Msg: no more files to list."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR3+8:
		strcpy(error_msg, _("Msg: the directory name already exists."));
		*message = g_strdup(error_msg);
		break;
	// The following one is returned with folder names >= 230 bytes long
	case ERR_CALC_ERROR3+9:
		strcpy(error_msg, _("Msg: the destination (folder or file) path is too long."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR3+10:
		strcpy(error_msg, _("Msg: the file name is invalid."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR3+11:
		strcpy(error_msg, _("Msg: no file extension."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR3+12:
		strcpy(error_msg, _("Msg: forbidden characters in folder name."));
		*message = g_strdup(error_msg);
		break;
	case ERR_CALC_ERROR3+13:
		strcpy(error_msg, _("Msg: anti-downgrade protection refuses this OS version."));
		*message = g_strdup(error_msg);
		break;

	default:
		// propagate error code
		return number;
	}

	return 0;
}

/**
 * ticables_error_get:
 * @number: error number (see error.h for list).
 * @message: a newly GLib allocated string which contains corresponding error *message.
 *
 * Attempt to match the message corresponding to the error number. The returned
 * string must be freed when no longer needed.
 * The error message has the following format:
 * - 1: the error message
 * - 2: the cause(s), explanations on how to fix it
 * - 3: the error returned by the system
 *
 * Return value: 0 if error has been caught, the error number otherwise (propagation).
 **/
TIEXPORT1 int TICALL ticables_error_get(CableError number, char **message)
{
	char *tmp;

	if (message == NULL)
	{
		ticables_critical("ticables_error_get(NULL)\n");
		return number;
	}

	switch(number)
	{
		case ERR_BUSY_1:
			*message = g_strconcat(
			_("Msg: link cable is busy."),
			"\n",
			_("Cause: a transfer is in progress."),
			NULL);
		break;

		case ERR_ROOT:
			*message = g_strconcat(
			_("Msg: unable to use parallel/serial port: access refused."),
			"\n",
			_("Cause: you are not running program as root or setuid root."),
			NULL);
		break;

		case ERR_TTDEV:
			*message = g_strconcat(
			_("Msg: unable to use serial (/dev/ttySx) device."),
			"\n",
			_("Cause: the /dev/ttySx node doesn't exist or you don't have required permissions. " \
				"How to grant permission to devices to the user depends on the " \
				"distribution, it may involve adding the user to a group such " \
				"as 'dialout' or 'uucp'. This can be done by editing /etc/group " \
				"and add the username after the ':'. Example: dialout:x:20:romain " \
				""
				),
			NULL);
		break;

		case ERR_PPDEV:
			*message = g_strconcat(
			_("Msg: unable to use parport (/dev/parportX) device."),
			"\n",
			_("Cause: the /dev/parportX node doesn't exist or you don't have required permissions. "
				"How to grant permission to devices to the user depends on the " \
				"distribution, it may involve adding the user to a group such " \
				"as 'lp'. This can be done by editing /etc/group " \
				"and add the username after the ':'. Example: lp:x:7:romain " \
				""
				),
			NULL);
		break;

		case ERR_USBFS:
			*message = g_strconcat(
			_("Msg: unable to use USB pseudo file system."),
			"\n",
			_("Cause: the USB filesystem is not mounted or " \
				"cannot access USB device because current user doesn't have the necessary " \
				"permissions. How to grant permission to devices to the user depends on the " \
				"distribution; it may involve adding udev and/or pam_console configuration " \
				"files, and you may in addition have to add your user to a group such " \
				"as 'usb' or set-up libusb mounting parameters." \
				"If packages of TiLP and its libraries for your distribution exist, " \
				"it is recommended to use those, as everything which can be configured " \
				"automatically will be set up for you. Otherwise, manual setup steps, if any, are " \
				"explained in the CONFIG file shipped with the ticables2 library. " \
				""
				),
			NULL);
		break;

		case ERR_ILLEGAL_ARG:
			*message = g_strconcat(
			_("Msg: illegal operation or argument."),
			"\n",
			_("Cause: the program which uses this library is buggy. Fire-up the developer!"),
			NULL);
		break;

		case ERR_DHA_NOT_FOUND:
			*message = g_strconcat(
			_("Msg: DhaHelper driver and/or library not found."),
			"\n",
			_("Cause: the driver may have been not started yet; it may have been uninstalled; the library has been installed without administrator privileges."),
			NULL);
		break;

		case ERR_RWP_NOT_FOUND:
			*message = g_strconcat(
			_("Msg: RwPorts driver and/or library not found."),
			"\n",
			_("Cause: the driver may have been not started yet; it may have been uninstalled; the library has been installed without administrator privileges."),
			NULL);
		break;

		case ERR_WRITE_ERROR:
			*message = g_strconcat(
			_("Msg: error occured while writing to the device."),
			NULL);
		break;

		case ERR_WRITE_TIMEOUT:
			*message = g_strconcat(
			_("Msg: timeout occured while writing to the device."),
			"\n",
			_("Cause: check that link cable is plugged and/or the calculator is ready."),
			NULL);
		break;

		case ERR_READ_ERROR:
			*message = g_strconcat(
			_("Msg: error occured while reading from the device."),
			NULL);
		break;

		case ERR_READ_TIMEOUT:
			*message = g_strconcat(
			_("Msg: timeout occured while reading from the device."),
			"\n",
			_("Cause: check that link cable is plugged and/or the calculator is ready."),
			NULL);
		break;

		case ERR_PROBE_FAILED:
			*message = g_strconcat(
			_("Msg: the probing of device failed."),
			"\n",
			_("Cause: internal error. If you get it, this is a bug!"),
			NULL);
		break;

		case ERR_GRY_CREATEFILE:
		case ERR_GRY_SETUPCOMM:
		case ERR_GRY_GETCOMMSTATE:
		case ERR_GRY_SETCOMMSTATE:
		case ERR_GRY_GETCOMMTIMEOUT:
		case ERR_GRY_SETCOMMTIMEOUT:
		case ERR_GRY_PURGECOMM:
		case ERR_GRY_SETCOMMMASK:
			*message = g_strconcat(
			_("Msg: unable to open serial port."),
			"\n",
			_("Cause: check that device is not used/locked by another application."),
			NULL);
		break;

		case ERR_VTI_FINDWINDOW:
		case ERR_VTI_OPENFILEMAPPING:
		case ERR_VTI_MAPVIEWOFFILE:
			*message = g_strconcat(
			_("Msg: failed to get VTI handle."),
			"\n",
			_("Cause: VTI may not be running. VTI must be launched before."),
			NULL);
		break;

		case ERR_VTI_IPCKEY:
		case ERR_VTI_SHMGET:
		case ERR_VTI_SHMAT:
		case ERR_VTI_SHMDT:
		case ERR_VTI_SHMCTL:
			*message = g_strconcat(
			_("Msg: unable to get a unique IPC (Inter Process Communication) key."),
			"\n",
			_("Cause: check that you have enough resources for allocating a shared memory segment."),
			NULL);
		break;

		case ERR_TIE_OPEN:
		case ERR_TIE_CLOSE:
		*message = g_strconcat(
			_("Msg: unable to open/close pipes for virtual linking."),
			"\n",
			_("Cause: check that you have permissions to create a pipe in the /tmp directory."),
			NULL);
		break;

		case ERR_TTY_OPEN:
		case ERR_GRY_OPEN:
			*message = g_strconcat(
			_("Msg: unable to open serial device."),
			"\n",
			_("Cause: check that you have permissions on /dev/ttySx device. Check that device is not locked."),
			NULL);
		break;

		case ERR_TTY_IOCTL:
		case ERR_GRY_IOCTL:
			*message = g_strconcat(
			_("Msg: unable to issue a specific command on serial device."),
			"\n",
			_("Cause: check that you have permissions on /dev/ttySx device. Check that device is not locked."),
			NULL);
		break;

		case ERR_PPT_OPEN:
			*message = g_strconcat(
			_("Msg: unable to open parallel device."),
			"\n",
			_("Cause: check that you have permissions on /dev/parportX device. Check that device is not locked."),
			NULL);
		break;

		case ERR_PPT_IOCTL:
			*message = g_strconcat(
			_("Msg: unable to issue a specific command on parallel device."),
			"\n",
			_("Cause: check that you have permissions on /dev/parportX device. Check that device is not locked."),
			NULL);
		break;

		case ERR_LIBUSBWIN32_NOT_PRESENT:
			*message = g_strconcat(
			_("Msg: unable to find the libusb-win32 driver."),
			"\n",
			_("Cause: the driver is not present and/or installed."),
			NULL);
		break;

		case ERR_LIBUSB_OPEN:
		case ERR_LIBUSB_CLAIM:
		case ERR_LIBUSB_CONFIG:
			*message = g_strconcat(
			_("Msg: failed to open the USB device."),
			"\n",
			_("Cause: Check that the USB cable is plugged in and that the calculator is turned ON! Also, check libusb and usbfs for valid permissions."),
			NULL);
		break;

		case ERR_LIBUSB_RESET:
			*message = g_strconcat(
			_("Msg: unable to reset USB device."),
			"\n",
			_("Cause: Check that cable is connected or not stalled. Try to unplug/plug it."),
			NULL);
		break;

		case ERR_NOT_OPEN:
			*message = g_strconcat(
			_("Msg: attempting to use a cable which has not been open before."),
			"\n",
			_("Cause: Internal error."),
			NULL);
		break;

		case ERR_NO_CABLE_1:
			*message = g_strconcat(
			_("Msg: no cable found."),
			"\n",
			_("Cause: probing failed or invalid parameters."),
			NULL);
		break;

		case ERR_NO_LIBRARY:
			*message = g_strconcat(
			_("Msg: DLL not found."),
			"\n",
			_("Cause: maybe you renamed the DLL ?"),
			NULL);
		break;

		default:
		// propagate error code
		return number;

	}

#ifndef __WIN32__
	if (errno != 0) 
	{
		gchar *str;
		tmp = *message;
		
		str = g_strdup_printf(" (errno = %i)", errno);
		*message = g_strconcat(tmp, "\n", "System: ", strerror(errno), str, "\n", NULL);
		g_free(tmp);
		g_free(str);
	}
#else
	if (GetLastError()) 
	{
		LPVOID lpMsgBuf;
		gchar *str;
		tmp = *message;

	FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, 
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 
			0, 
			NULL);

		str = g_strdup_printf("(%li) %s", (long)GetLastError, (char *)lpMsgBuf);
		*message = g_strconcat(tmp, "\n", "System: ", str, NULL);
		g_free(tmp);
		g_free(str);
	}
#endif

	return 0;
}
