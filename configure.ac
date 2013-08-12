dnl Process this file with autoconf to produce a configure script.
dnl $Id: configure.ac 4422 2011-12-07 20:49:53Z debrouxl $

# Init.
AC_PREREQ(2.57)
AC_INIT(TILP2M, 1.17, [tilp-users@lists.sourceforge.net])

AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE
AC_CONFIG_MACRO_DIR([m4])

AC_PREFIX_DEFAULT(/usr/local)

# Files to configure.
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  build/Makefile
  build/mingw/Makefile
  builder/Makefile
  desktop/Makefile
  help/Makefile
  icons/Makefile
  man/Makefile
  pixmaps/Makefile
  src/Makefile
  wicons/Makefile
])

AC_MSG_CHECKING([for something to drink while compiling])
AC_MSG_RESULT([err: no fridge found!])

# Setup libtool.
AC_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LN_S
AC_ISC_POSIX
AC_PROG_AWK
AC_CHECK_PROG(GROFF, groff, yes, no)
AM_CONDITIONAL(USE_GROFF, test "$GROFF" != "no")
AC_CHECK_TOOL(RC, windres, windres)

# Translation
AC_PROG_INTLTOOL([0.40.0])

GETTEXT_PACKAGE=tilp2
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [tilp2])
AM_GLIB_GNU_GETTEXT

PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.6.0)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.12.0)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

# Check for zlib
AC_CHECK_HEADER(zlib.h)
AC_CHECK_LIB(z, compress)
LIBZ="$LIBS"
LIBS=""
AC_SUBST(LIBZ)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_STAT
AC_TYPE_UID_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_STRUCT_TM

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_CHECK_FUNCS([getcwd memset mkdir strcasecmp strchr strdup strrchr strstr strtol])

# USB cable support
AC_ARG_ENABLE(libusb,
  AC_HELP_STRING([--disable-libusb], [disable USB support through libusb 0.1.x]),
  [cab_cv_use_libusb=$enableval], [cab_cv_use_libusb=yes])
AC_CACHE_CHECK([whether to enable USB through libusb 0.1.x],
  [cab_cv_use_libusb], [cab_cv_use_libusb=no])

AC_ARG_ENABLE(libusb10,
  AC_HELP_STRING([--enable-libusb10], [enable EXPERIMENTAL USB support through libusb 1.0.x]),
  [cab_cv_use_libusb10=$enableval], [cab_cv_use_libusb10=no])
AC_CACHE_CHECK([whether to enable USB through libusb 1.0.x],
  [cab_cv_use_libusb10], [cab_cv_use_libusb10=no])

if [test "x$cab_cv_use_libusb" != xno]; then
  if [test "x$cab_cv_use_libusb10" != xno]; then
    PKG_CHECK_MODULES([LIBUSB10], [ libusb-1.0 >= 1.0.0 ],
      [HAVE_USB=yes
       AC_DEFINE([HAVE_LIBUSB_1_0], [1],
                 [Define to 1 to enable EXPERIMENTAL libusb 1.0.x support.])
       AC_CHECK_LIB([usb-1.0], [libusb_strerror],
                    [HAVE_LIBUSB10_STRERROR=yes], [HAVE_LIBUSB10_STRERROR=no])
      ])
  else
    PKG_CHECK_MODULES([LIBUSB], [ libusb >= 0.1.12 ],
      [HAVE_USB=yes
       AC_DEFINE([HAVE_LIBUSB], [1], [Define to 1 to enable libusb 0.1.x support.])
      ])
  fi
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_STRUCT_TM

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_STAT
AC_CHECK_FUNCS([memset strcasecmp strchr strdup strrchr])

AC_SUBST(LIBUSB_CFLAGS)
AC_SUBST(LIBUSB_LIBS)
AC_SUBST(LIBUSB10_CFLAGS)
AC_SUBST(LIBUSB10_LIBS)

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([stdint.h termios.h linux/parport.h linux/ppdev.h linux/serial.h] linux/ticable.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UID_T
AC_TYPE_MODE_T
AC_C_CONST
AC_HEADER_TIME

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT
AC_CHECK_FUNCS([cfmakeraw])


# Logging
AC_ARG_ENABLE(logging,
  AC_HELP_STRING([--enable-logging], [enable logging]),
  [cab_cv_enable_logging=$enableval], [])
AC_CACHE_CHECK([whether to enable logging],
  [cab_cv_enable_logging], [cab_cv_enable_logging=no])
if test x$cab_cv_enable_logging = xyes; then
  AC_DEFINE(ENABLE_LOGGING, 1, [Define to 1 to enable logging.])
fi


# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CONST

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_CHECK_FUNCS([bzero memmove memset strcasecmp strdup])


# Platform specific tests.
dnl AC_CANONICAL_HOST
case "$host" in
  *-*-mingw*) ARCH="-D__WIN32__ -D__MINGW32__" ;;
  *-*-cygwin) ARCH="-D__WIN32__ -D__CYGWIN__ -mno-cygwin" ;;
  *-*-*bsd*)  ARCH="-D__BSD__" ;;
  *)          ARCH="-D__LINUX__" ;;
esac
CFLAGS="$CFLAGS $ARCH"

# Ensure MSVC-compatible struct packing convention is used when
# compiling for Win32 with gcc.
# What flag to depends on gcc version: gcc3 uses "-mms-bitfields", while
# gcc2 uses "-fnative-struct".
case $host_os in
*mingw*|*cygwin*)
  if test x$GCC = xyes; then
    msnative_struct=''
    AC_MSG_CHECKING([how to get MSVC-compatible struct packing])
    if test -z "$ac_cv_prog_CC"; then
      our_gcc="$CC"
    else
      our_gcc="$ac_cv_prog_CC"
    fi
    case `$our_gcc --version | sed -e 's,\..*,.,' -e q` in
      2.)
        if $our_gcc -v --help 2>/dev/null | grep fnative-struct >/dev/null; then
          msnative_struct='-fnative-struct'
        fi
        ;;
      *)
        if $our_gcc -v --help 2>/dev/null | grep ms-bitfields >/dev/null; then
          msnative_struct='-mms-bitfields'
        fi
        ;;
    esac
    if test x"$msnative_struct" = x ; then
      AC_MSG_RESULT([no way])
      AC_MSG_WARN([produced libraries might be incompatible with MSVC-compiled code])
    else
      CFLAGS="$CFLAGS $msnative_struct"
      AC_MSG_RESULT([${msnative_struct}])
    fi
  fi
  ;;
esac

AM_CONDITIONAL(OS_WIN32, test "$msnative_struct")

# Check for the new -fvisibility=hidden flag introduced in gcc 4.0
# Allow to reduce shared library size and avoid symbol clash
case $host_os in
*mingw*)
  ;;
*)
        if test x$GCC = xyes; then
           visibility_flag=''
           AC_MSG_CHECKING([whether gcc accepts -fvisibility])
           if test -z "$ac_cv_prog_CC"; then
              our_gcc="$CC"
           else
              our_gcc="$ac_cv_prog_CC"
           fi
           if $our_gcc -v --help 2>/dev/null | grep "fvisibility" >/dev/null; then
              visibility_flag='-fvisibility=hidden'
           fi
           if test x"$visibility_flag" = x ; then
              AC_MSG_RESULT([no])
           else
              CFLAGS="$CFLAGS $visibility_flag"
              AC_MSG_RESULT([${visibility_flag}])
              AC_DEFINE(HAVE_FVISIBILITY, 1, [Use -fvisibility=hidden flag])
            fi
        fi
        ;;
esac

case "$host" in
  *-*-linux*) EXFL="-Wl,--export-dynamic" ;;
  *-*-mingw*) EXFL="-lcomdlg32" ;;
  *)          EXFL="" ;;
esac
LDFLAGS="$LDFLAGS $EXFL"

# On Darwin, libglade chokes on stripped executables.
case "$host" in
  *-apple-darwin*)
    case "$CFLAGS $CXXFLAGS $LDFLAGS" in
      *" -s "*|"-s "*|*" -s"|"-s")
         echo "warning: cannot strip libglade executables on this host"
         CFLAGS=`echo "$CFLAGS"|sed 's/\( -s\)* / /g;s/^-s //g;s/ -s$//g;s/^-s$//g'`
         CXXFLAGS=`echo "$CXXFLAGS"|sed 's/\( -s\)* / /g;s/^-s //g;s/ -s$//g;s/^-s$//g'`
         LDFLAGS=`echo "$LDFLAGS"|sed 's/\( -s\)* / /g;s/^-s //g;s/ -s$//g;s/^-s$//g'`
         ;;
    esac
    ;;
esac

# some extra flags
CFLAGS="$CFLAGS -DGTK_DISABLE_DEPRECATED"
CXXFLAGS="$CFLAGS -fno-rtti -fno-exceptions"

AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(CXXFLAGS)

# Output.
AC_OUTPUT([
po/Makefile.in
])

echo "Now, you can type 'make' and 'make install'."
