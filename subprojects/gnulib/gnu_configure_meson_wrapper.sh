#!/usr/bin/sh

# Copyright (C) 2024 Miro Palmu.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this file.  If not, see <https://www.gnu.org/licenses/>.

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
