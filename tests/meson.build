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

subproject('ut')

test_dependencies = []
test_dependencies += dependency('ut')
test_dependencies += guilander_dep

# List of tests that can run in parallel
unit_tests = [
    'test_unit_test',
    'test_meson_guilander_dep',
    'test_meson_guilander_headers_do_not_leak_macros',
    'test_wayland_connected_client',
    'test_wayland_primitive_types',
    'test_wayland_message_visitor',
    'test_wayland_message_buffer',
    'test_wayland_message_parser',
    'test_wayland_message_utils',
    'test_wayland_system_utils',
    'test_gnu_pipe',
    'test_gnu_fd_handle',
    'test_gnu_memory_block',
    'test_gnu_local_stream_socket',
    'test_sstd_math',
    'test_sstd_type_list',
    'test_sstd_byte_array',
    'test_sstd_type_utils',
    'test_sstd_tuple_utils',
    'test_sstd_unique_handle',
    'test_sstd_throw_system_error',
    'test_sstd_construct_allocator_adapter',
]

test_names_and_exes = {}

# Create test executables:
foreach test_name : unit_tests
    test_names_and_exes += {test_name :
        executable(
            test_name,
            files(test_name + '.cpp'),
            dependencies: test_dependencies,
            override_options: ['optimization=0'],
        )
    }

endforeach

foreach test_name, test_exe : test_names_and_exes
    test(
        test_name,
        test_exe,
        is_parallel: true,
        timeout: 10,
    )
endforeach
