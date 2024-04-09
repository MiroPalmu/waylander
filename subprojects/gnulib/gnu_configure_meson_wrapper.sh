#!/usr/bin/sh
#
# Bridge to make projects with GNU build system configure script buildable with meson.
#
# Assumptions:
#
#   A) Project X is build using meson.
#   B) Project X has subdirectory Y.
#   C) Y contains GNU build system configure script Y/configure.
#   D) Running Y/configure will generates GNU build files to $PWD,
#      s.t. running make in $PWD will build something successfully.
#   E) Positional arguments:
#        - $1 contains path to configure script
#        - $2 contains path to directory where configure script should be used.
#   F) meson.get_compiler('<lang>').cmd_array()[0] returns name of compiler for <lang>.
#   G) MESON_BUILD_ROOT/MESON_SUBDIR exists.

configure_script="$1"
dir_to_configure="$2"

if [ -x "$configure_script" ]; then
    echo "configuarion script found: '$configure_script'"
else
    echo "error: configuarion script is not executable: '$configure_script'"
    echo "       from pwd: '$PWD' as user: '$USER'"
    echo "       $(ls -l $configure_script)"
    exit 1
fi


cd "$dir_to_configure" || { echo "error: cd to '$dir_to_configure' failed!"; exit 1; }

exec $configure_script
