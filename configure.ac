# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([buh],[0])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign 1.11 -Wall -Wno-portability subdir-objects silent-rules color-tests dist-xz tar-ustar])

AM_SILENT_RULES([yes])

### checks ###

# CC

CFLAGS="-O0 -g"
AC_PROG_CC
AC_GNU_SOURCE

# pkg-config

#PKG_PROG_PKG_CONFIG

#### compiler flags ####

AX_APPEND_COMPILE_FLAGS(
        [-Wall -W -Wextra -pipe -Wno-long-long -Wno-overlength-strings -Wunsafe-loop-optimizations -Wundef -Wlogical-op -Wsign-compare -Wmissing-include-dirs -Wold-style-definition -Wpointer-arith -Winit-self -Wfloat-equal -Wmissing-prototypes -Wstrict-prototypes -Wredundant-decls -Wmissing-declarations -Wmissing-noreturn -Wshadow -Wendif-labels -Wcast-align -Wstrict-aliasing -Wwrite-strings -Wno-unused-parameter -Wno-error=unused-function -ffast-math -fno-common -fdiagnostics-show-option],
        [], [-pedantic -pedantic-errors -Werror])

#### libtool ####

LT_PREREQ(2.4)
LT_INIT([dlopen disable-static])

#### environment ####

AC_HEADER_STDC
AC_CHECK_HEADERS([sys/socket.h sys/epoll.h])

# unit tests

PKG_CHECK_MODULES(LIBCHECK, [check])

#

AC_CONFIG_FILES([
Makefile
lib/Makefile
tests/Makefile
])

AC_OUTPUT
