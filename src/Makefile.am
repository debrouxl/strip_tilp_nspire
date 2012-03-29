## Process this file with automake to produce Makefile.in

AM_CFLAGS = -Wall

# programs and files to compile and install
bin_PROGRAMS = tilpm

# build instructions
tilpm_CPPFLAGS = -I$(top_srcdir)/intl \
	@GLIB_CFLAGS@ @GTK_CFLAGS@ @GLADE_CFLAGS@ \
	-DSHARE_DIR=\"$(pkgdatadir)\" \
	-DLOCALEDIR=\"$(datadir)/locale\" \
	-DSYSCONFDIR=\"$(sysconfdir)\" \
	@LIBUSB_CFLAGS@ @LIBUSB10_CFLAGS@ \
	-DGTK_DISABLE_DEPRECATED
tilpm_LDFLAGS = -export-dynamic
tilpm_LDADD = @GLIB_LIBS@ @GTK_LIBS@ @GLADE_LIBS@ \
	@LTLIBINTL@ @LIBZ@ @LIBUSB_LIBS@ @LIBUSB10_LIBS@
tilpm_SOURCES = *.h \
	tilp_calcs.c tilp_cmdline.c tilp_config.c tilp_error.c \
	tilp_files.c tilp_gif.c tilp_log.c tilp_main.c \
	tilp_misc.c tilp_paths.c tilp_screen.c tilp_slct.c \
	tilp_update.c tilp_vars.c tilp_device.c tilp.c main.c \
	\
	about.c action.c bookmark.c clist.c clist_rbm.c clock.c \
	ctree.c ctree_rbm.c dboxes.c device.c dnd.c filesel.c \
	gtk_gif.c gtk_update.c labels.c manpage.c options.c pbars.c \
	release.c screenshot.c scroptions.c splash.c support.c \
	toolbar.c fileprop.c \
	\
	ticonv.c charset.c tokens.c filename.c \
	\
	minizip/*.h \
	comments.c \
	error.c \
	files8x.c files9x.c filesnsp.c filesxx.c \
	filetypes.c \
	grouped.c \
	intelhex.c \
	logging.c \
	misc.c \
	rwfile.c \
	tifiles.c \
	tigroup.c \
	type2str.c \
	types73.c types82.c types83.c types83p.c types84p.c \
	types85.c types86.c \
	types89.c types89t.c types92.c types92p.c typesv2.c \
	typesnsp.c \
	typesxx.c \
	ve_fp.c \
	minizip/ioapi.c   \
	minizip/miniunz.c \
	minizip/minizip.c \
	minizip/unzip.c   \
	minizip/zip.c\
	\
	data_log.c \
	detect.c \
	ioports.c \
	link_gry.c link_nul.c link_par.c link_blk.c \
	link_usb.c link_tie.c link_vti.c link_xxx.c \
	log_dbus.c log_dusb.c log_hex.c log_nsp.c \
	hex2dbus.c hex2dusb.c hex2nsp.c \
	probe.c \
	ticables.c \
	none.c \
	\
	backup.c \
	calc_00.c calc_xx.c \
	calc_73.c calc_82.c calc_83.c calc_85.c calc_86.c \
	calc_89.c calc_92.c \
	calc_84p.c calc_89t.c \
	calc_nsp.c \
	cmd73.c cmd82.c cmd85.c cmd89.c cmd92.c dusb_cmd.c nsp_cmd.c \
	dirlist.c \
	keys73.c keys83.c keys83p.c keys86.c keys89.c keys92p.c \
	dbus_pkt.c \
	dusb_rpkt.c dusb_vpkt.c \
	nsp_rpkt.c nsp_vpkt.c \
	romdump.c \
	ticalcs.c \
	tikeys.c

EXTRA_DIST = bsd/* linux/* macos/* win32/*.c win32/*.h win32/dha/* win32/usb/* win64/*.c win64/*.h win64/rwp/*

if OS_WIN32
libusbdir = $(pkgdatadir)/libusb
dist_libusb_DATA = win32/usb/*.cat win32/usb/*.inf
else
EXTRA_DIST += win32/usb/*.cat win32/usb/*.inf
endif

if OS_WIN32
  tilpm_DEPENDENCIES = ../build/mingw/tilp-rc.o
  tilpm_LDFLAGS += -Wl,../build/mingw/tilp-rc.o
endif

dos2unix:
	dos2unix
	lowercase
	uppercase
