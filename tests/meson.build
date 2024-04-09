# Test dependencies

subproject('ut')

test_dependencies = []
test_dependencies += dependency('ut')
test_dependencies += guilander_dep

# List of tests that can run in parallel
single_threaded_unit_tests = [
    'test_unit_test',
    'test_byte_buff',
    'test_linux_shm_memory',
    'test_wayland_connected_client',
    'test_wayland_primitive_types',
    'test_gnu_pipe',
]

single_threaded_test_names_and_exes = {}

# Create test executables:
foreach test_name : single_threaded_unit_tests
    single_threaded_test_names_and_exes += {test_name :
        executable(
            test_name,
            files(test_name + '.cpp'),
            dependencies: test_dependencies,
            override_options: ['optimization=0'],
        )
    }

endforeach

foreach test_name, test_exe : single_threaded_test_names_and_exes
    test(
        test_name,
        test_exe,
        is_parallel: true
    )
endforeach
