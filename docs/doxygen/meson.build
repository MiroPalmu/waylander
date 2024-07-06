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


doxygen = find_program('doxygen', required : false)
if not doxygen.found()
    error('Skipping docs due to missing Doxygen')
    subdir_done()
endif

# Save this path so it can be used in sphinx/meson.build
doxygen_output_dir = meson.current_build_dir()

# Define variables used in Doxyfile.in
doxygen_config_data = configuration_data({
    'NAME' : meson.project_name(),
    'VERSION' : meson.project_version(),
    # In Doxygen.in:
    #   INPUT = "@INPUT@"
    # also:
    #   RECURSIVE = YES
    # so we just have to give different directories separated with ' '
    'INPUT' : '" "'.join([
        meson.project_source_root() + '/src',
        meson.project_source_root() + '/include',
    ]),
    'OUTPUT_DIRECTORY' : doxygen_output_dir
})

doxyfile = configure_file(
    input: 'Doxyfile.in',
    output: 'Doxyfile',
    configuration: doxygen_config_data,
    install: false
)


doxygen_html_target = custom_target(
    'doxygen_html_target',
    input: doxyfile,
    output: 'html',
    command: [doxygen, '@INPUT@'],
    install: false,
    build_always_stale: true
)

alias_target(
    'doxygen',
    doxygen_html_target
)
