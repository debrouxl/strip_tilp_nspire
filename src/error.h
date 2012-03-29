/* Hey EMACS -*- linux-c -*- */
/* $Id: error.h 4392 2011-08-01 09:24:05Z debrouxl $ */

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

#ifndef __ERRCODES__
#define __ERRCODES__

/* Error codes must begin at 0 up to 255 */

typedef enum
{
    ERR_NO_ERROR = 0,		// No error (returns 0)

    ERR_BUSY_1,
    ERR_ILLEGAL_ARG,

    ERR_READ_ERROR,
    ERR_READ_TIMEOUT=4,		// exported in ticables.h
    ERR_WRITE_ERROR,
    ERR_WRITE_TIMEOUT=6,	// exported in ticables.h
    ERR_PROBE_FAILED,
    ERR_FLUSH_ERROR,

    ERR_GRY_CREATEFILE,
    ERR_GRY_SETUPCOMM,		//10
    ERR_GRY_GETCOMMSTATE, 
    ERR_GRY_SETCOMMSTATE,
    ERR_GRY_GETCOMMTIMEOUT,
    ERR_GRY_SETCOMMTIMEOUT,
    ERR_GRY_PURGECOMM,
    ERR_GRY_SETCOMMMASK,

    ERR_LIBUSBWIN32_NOT_PRESENT,
    ERR_FREE2,
    ERR_FREE3,
    ERR_DHA_NOT_FOUND,		//20
    ERR_RWP_NOT_FOUND, 
    ERR_FREE6,

    ERR_VTI_FINDWINDOW,	// open
    ERR_VTI_OPENFILEMAPPING,
    ERR_VTI_MAPVIEWOFFILE,

    ERR_TIE_OPENFILEMAPPING,	//open
    ERR_TIE_MAPVIEWOFFILE,

    ERR_VTI_IPCKEY,	//open
    ERR_VTI_SHMGET,
    ERR_VTI_SHMAT,			//30
    ERR_VTI_SHMDT, //close
    ERR_VTI_SHMCTL,

    ERR_TIE_OPEN,	//open
    ERR_TIE_CLOSE,

    ERR_GRY_OPEN,
    ERR_GRY_IOCTL,

    ERR_LIBUSB_OPEN,
    ERR_LIBUSB_CLAIM,
    ERR_LIBUSB_CONFIG,
    ERR_LIBUSB_RESET,		//40

    ERR_FREE7, 
    ERR_ROOT,    
    ERR_TTDEV,
    ERR_PPDEV,
    ERR_USBFS,

    ERR_TTY_OPEN,
    ERR_TTY_IOCTL,

    ERR_PPT_OPEN,
    ERR_PPT_IOCTL, 

    ERR_NO_CABLE_1 = 50,		//50
    ERR_NOT_OPEN,
    ERR_NO_LIBRARY,

} CableError;

/* Error codes must begin at 256 up to 511 */

typedef enum
{
	ERR_ABORT = 256,		// Operation aborted (exported in ticalcs.h)
	ERR_NOT_READY,			// Calculator is not ready (don't change it !)
	ERR_CHECKSUM,			// Checksum error
	ERR_INVALID_HOST,		// Invalid host ID
	ERR_INVALID_TARGET,		// Invalid target ID
	ERR_INVALID_CMD,		// Invalid command ID
	ERR_EOT,				// Not really an error (internal use)
	ERR_VAR_REJECTED,		// The variable has been rejected by calc
	ERR_CTS_ERROR,			// Invalid CTS packet
	ERR_NACK,				// NACK received
	ERR_INVALID_PACKET,		// Packet is invalid
	
	ERR_MALLOC_3,				// No memory ?!
	ERR_NO_CABLE_3,			// No cable attached
	ERR_BUSY_3,				// Device is busy (transfer in progress)
	ERR_FILE_OPEN_3,			// Can't open file
	ERR_UNSUPPORTED_3,		// This function does not exist for the calc
	
// ---
		
	ERR_VOID_FUNCTION,		// This function does not exist for the calc
	ERR_OUT_OF_MEMORY,		// Calc reply out of *memory
	ERR_OPEN_FILE,
	ERR_SAVE_FILE,			// Can not open (writing) file
	ERR_PENDING_TRANSFER,	// A transfer is in progress
	
	ERR_FATAL_ERROR,		// Fatal error (alloc, file, ...)
	ERR_MISSING_VAR,		// The requested var does not exist
	ERR_NO_VARS,			// No vars to backup

// ---

	ERR_ROM_ERROR,
	ERR_NO_CALC,			// No calc probed
	ERR_INVALID_HANDLE,		// Invalid handle

// --- 300 to 349 are reserved for hand-held status (DUSB)
	
	ERR_CALC_ERROR2 = 300,	// Hand-held returned an error code

// --- 350 to 399 are reserved for hand-held status (DBUS)
	
	ERR_CALC_ERROR1 = 350,	// Hand-held returned an error code

// --- 400 to 459 are reserved for hand-held status (NSpire)

	ERR_CALC_ERROR3 = 400,

// ---

} CalcError;

/* Error codes must begin at 512 up to 767 */

typedef enum {

	ERR_MALLOC_2=512,			// Error with malloc
	ERR_FILE_OPEN_2,			// Unable to open file
	ERR_FILE_CLOSE,			// Unable to close file
	ERR_GROUP_SIZE,			// Group size exceeded (>64KB)
	ERR_BAD_CALC,			// The function does not exist for this calc
	ERR_INVALID_FILE,		// Is not a TI file
	ERR_BAD_FILE,			// Same as above
	ERR_FILE_CHECKSUM,		// Checksum file error
	ERR_FILE_ZIP,			// (Un)Zip internal error
	ERR_UNSUPPORTED_2,		// Function not supported
	ERR_FILE_IO				// Stream error
} FileError;

#endif
