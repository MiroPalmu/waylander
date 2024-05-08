# Test dependencies

subproject('ut')

test_dependencies = []
test_dependencies += dependency('ut')
test_dependencies += guilander_dep

# List of tests that can run in parallel
unit_tests = [
    'test_unit_test',
    'test_meson_guilander_dep',
    'test_meson_guilander_headers_do_not_leak_macros',
    'test_byte_buff',
    'test_linux_shm_memory',
    'test_wayland_connected_client',
    'test_wayland_primitive_types',
    'test_wayland_message_buffer',
    'test_wayland_message_parser',
    'test_gnu_pipe',
    'test_gnu_fd_handle',
    'test_gnu_local_stream_socket',
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
