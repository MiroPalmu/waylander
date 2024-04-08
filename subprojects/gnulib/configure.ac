#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([gnulib-modules-to-meson], [0.0.1], [email@miropalmu.cc])

AC_CONFIG_SRCDIR([lib/full-read.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_FILES([Makefile lib/Makefile])
AC_CONFIG_MACRO_DIRS([m4])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# - gl = gnulib

# This should be right after AC_PROG_CC
gl_EARLY
gl_INIT

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT
