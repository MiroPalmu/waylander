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

doxygen = find_program('doxygen', version : '>=1.9.1', required : false)
dot = find_program('dot', required : false)

if not doxygen.found()
    message('Skipping doxygen and sphinx targets due to missing Doxygen')
    subdir_done()
endif

if not dot.found()
    message('Skipping doxygen and sphinx targets due to missing Dot')
    subdir_done()
endif

subdir('doxygen')

sphinx = find_program('sphinx-build', version : '>=4.3.2', required: false)

if not sphinx.found()
    message('Skipping sphinx target due to missing Sphinx')
    subdir_done()
endif

# Check if Breathe is installed using python module of meson
breathe = import('python').find_installation(
    'python3',
    modules : ['breathe'],
    required : false
)

if not breathe.found()
    message('Skipping sphinx target due to missing Breathe')
    subdir_done()
endif

subdir('sphinx')
