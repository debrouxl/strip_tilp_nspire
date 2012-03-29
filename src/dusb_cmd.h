/* Hey EMACS -*- linux-c -*- */
/* $Id: cmd84p.h 2074 2006-03-31 08:36:06Z roms $ */

/*  libticalcs - Ti Calculator library, a part of the TiLP project
 *  Copyright (C) 1999-2005  Romain Li�vin
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

#ifndef __DUSB_CMDS__
#define __DUSB_CMDS__

#define DUSB_DFL_BUF_SIZE	1024

// Parameter IDs
#define PID_PRODUCT_NUMBER	0x0001
#define PID_PRODUCT_NAME	0x0002
#define PID_MAIN_PART_ID	0x0003
#define PID_HW_VERSION		0x0004
#define PID_FULL_ID			0x0005
#define PID_LANGUAGE_ID		0x0006
#define PID_SUBLANG_ID		0x0007
#define PID_DEVICE_TYPE		0x0008
#define PID_BOOT_VERSION	0x0009
#define PID_OS_MODE			0x000A
#define PID_OS_VERSION		0x000B
#define PID_PHYS_RAM		0x000C
#define PID_USER_RAM		0x000D
#define PID_FREE_RAM		0x000E
#define PID_PHYS_FLASH		0x000F
#define PID_USER_FLASH		0x0010
#define PID_FREE_FLASH		0x0011
#define PID_USER_PAGES		0x0012
#define PID_FREE_PAGES		0x0013
#define PID_UNKNOWN2		0x0019
#define PID_UNKNOWN3		0x001A
#define PID_UNKNOWN4		0x001B
#define PID_UNKNOWN5		0x001C
#define PID_UNKNOWN6		0x001D
#define PID_LCD_WIDTH		0x001E
#define PID_LCD_HEIGHT		0x001F
#define PID_SCREENSHOT		0x0022
#define PID_UNKNOWN7		0x0023
#define PID_CLK_ON			0x0024
#define PID_CLK_SEC			0x0025
#define PID_CLK_DATE_FMT	0x0027
#define PID_CLK_TIME_FMT	0x0028
#define PID_UNKNOWN8		0x0029
#define PID_BATTERY			0x002D
#define PID_UNKNOWN9		0x0030
#define PID_UNKNOWN10		0x0031
#define PID_UNKNOWN11		0x0032
#define PID_HOMESCREEN		0x0037
#define PID_SCREEN_SPLIT	0x0039 

// Attributes IDs
#define AID_VAR_SIZE		0x01
#define AID_VAR_TYPE		0x02
#define AID_ARCHIVED		0x03
#define AID_UNKNOWN_04		0x04
#define AID_4APPVAR			0x05
#define AID_VAR_VERSION		0x08
#define AID_VAR_TYPE2		0x11
#define AID_UNKNOWN_13		0x13
#define AID_LOCKED			0x41
#define AID_UNKNOWN_42		0x42

// Execute commands
#define EID_PRGM			0x00
#define EID_ASM				0x01
#define EID_APP				0x02
#define EID_KEY				0x03
#define EID_UNKNOWN			0x04

// Modes
#define MODE_STARTUP	{ 1, 1, 0, 0, 0x07d0 }
#define MODE_BASIC		{ 2, 1, 0, 0, 0x07d0 }
#define MODE_NORMAL		{ 3, 1, 0, 0, 0x07d0 }

// Structures
typedef struct
{
	uint16_t	arg1;
	uint16_t	arg2;
	uint16_t	arg3;
	uint16_t	arg4;
	uint16_t	arg5;
} ModeSet;

typedef struct
{
	uint16_t	id;
	uint8_t		ok;
	uint16_t	size;
	uint8_t*	data;
} CalcParam;

typedef struct
{
	uint16_t	id;
	uint8_t		ok;
	uint16_t	size;
	uint8_t*	data;
} CalcAttr;

// Helpers
CalcParam*	cp_new(uint16_t id, uint16_t size);
void		cp_del(CalcParam* cp);
CalcParam** cp_new_array(int size);
void		cp_del_array(int size, CalcParam **params);

CalcAttr*	ca_new(uint16_t id, uint16_t size);
void		ca_del(CalcAttr* cp);
CalcAttr**	ca_new_array(int size);
void		ca_del_array(int size, CalcAttr **attrs);

#define CA(x)   (const CalcAttr **)(x)
#define CP(x)   (const CalcParam **)(x)

// Command wrappers
int cmd_s_mode_set(CalcHandle *h, ModeSet mode);

int cmd_s_os_begin(CalcHandle *h, uint32_t size);
int cmd_r_os_ack(CalcHandle *h, uint32_t *size);

int cmd_s_os_header(CalcHandle *h, uint16_t addr, uint8_t page, uint8_t flag, uint32_t size, uint8_t *data);
int cmd_s_os_data(CalcHandle *h, uint16_t addr, uint8_t page, uint8_t flag, uint32_t size, uint8_t *data);

int cmd_s_os_header_89(CalcHandle *h, uint32_t size, uint8_t *data);
int cmd_s_os_data_89(CalcHandle *h, uint32_t size, uint8_t *data);

int cmd_r_eot_ack(CalcHandle *h);

int cmd_s_param_request(CalcHandle *h, int npids, uint16_t *pids);
int cmd_r_param_data(CalcHandle *h, int nparams, CalcParam **params);

int cmd_s_dirlist_request(CalcHandle *h, int naids, uint16_t *aids);
int cmd_r_var_header(CalcHandle *h, char *folder, char *name, CalcAttr **attr);

int cmd_s_rts(CalcHandle *h, const char *folder, const char *name, uint32_t size, int nattrs, const CalcAttr **attrs);
int cmd_s_var_request(CalcHandle *h, const char *folder, const char *name, int naids, uint16_t *aids, int nattrs, const CalcAttr **attrs);

int cmd_s_var_content(CalcHandle *h, uint32_t  size, uint8_t  *data);
int cmd_r_var_content(CalcHandle *h, uint32_t *size, uint8_t **data);

int cmd_s_param_set(CalcHandle *h, const CalcParam *param);

int cmd_s_var_delete(CalcHandle *h, const char *folder, const char *name, int nattrs, const CalcAttr **attrs);
int cmd_s_execute(CalcHandle *h, const char *folder, const char *name, uint8_t action, const char *args, uint16_t code);

int cmd_r_mode_ack(CalcHandle *h);
int cmd_r_data_ack(CalcHandle *h);

int cmd_r_delay_ack(CalcHandle *h);

int cmd_s_eot(CalcHandle *h);
int cmd_r_eot(CalcHandle *h);

int cmd_s_error(CalcHandle *h, uint16_t code);


#endif
