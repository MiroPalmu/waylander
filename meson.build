project(
    'meson_template',
    'cpp',
    version: '0.0.1',
    default_options : ['warning_level=3', 'werror=true', 'optimization=3', 'cpp_std=c++20']
)

# Directories where compiler is able to include from
project_include_directories = []
project_include_directories += include_directories('include')

# Subprojects (using meson wraps)
ut = subproject('ut')

project_dependencies = []

# Project sources
project_sources = []

# Subdirectors
subdir('docs')
subdir('include')
subdir('src')
subdir('tests')

executable(
    'temp_executable',
    project_sources,
    include_directories : project_include_directories,
    dependencies : project_dependencies
)


# compile_commands.json stuff:
compdb = find_program('compdb', required : false)

if compdb.found() and meson.backend() == 'ninja'
    message('Custom target compdb added for generating compile_commands.json entries for headers')
    custom_target(
        'compdb',
        output: 'compdb',
        command: [
            files('.generate_compile_commands_json_with_headers.sh'),
            meson.project_build_root(),
            meson.source_root(),
        ],
        install: false,
        build_always_stale: true,
    )
else
    message('Skipping custom target compdb for greating compile_commands.json entries for headers due to:')
    if not compdb.found()
        message('- missing compdb (compdb can be installed with: pip install compdb)')
    endif
    if meson.backend() != 'ninja'
        message('- ninja is not meson backend (only backend that generates compile_commands.json)')
    endif
endif

