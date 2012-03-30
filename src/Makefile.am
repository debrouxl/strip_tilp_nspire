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
	ticonv.c tokens.c filename.c \
	\
	minizip/*.h \
	comments.c \
	error.c \
	filesnsp.c filesxx.c \
	filetypes.c \
	grouped.c \
	intelhex.c \
	logging.c \
	misc.c \
	rwfile.c \
	tifiles.c \
	type2str.c \
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
	link_nul.c link_usb.c link_xxx.c \
	log_hex.c log_nsp.c \
	hex2nsp.c \
	probe.c \
	ticables.c \
	none.c \
	\
	calc_00.c calc_xx.c \
	calc_nsp.c \
	nsp_cmd.c \
	dirlist.c \
	nsp_rpkt.c nsp_vpkt.c \
	romdump.c \
	ticalcs.c

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
