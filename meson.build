project(
    'guilander',
    'cpp',
    version: '0.0.1',
    default_options : ['warning_level=3', 'werror=true', 'optimization=3', 'cpp_std=c++26']
)

add_global_arguments('-fconcepts-diagnostics-depth=10', language : 'cpp')

gnulib_proj = subproject('gnulib')
gnulib_dep = gnulib_proj.get_variable('gnulib_dep')

guilander_dependencies = [
    gnulib_dep,
]

guilander_source_files = []

# Subdirectors
subdir('docs')
subdir('src')

guilander_lib = static_library(
    'guilander',
    guilander_source_files,
    include_directories : include_directories('include'),
    dependencies : guilander_dependencies,
)

guilander_dep = declare_dependency(
    include_directories : include_directories('include'),
    dependencies : gnulib_dep,
    link_with : guilander_lib,
)

# Tests has to come after guiladner_lib as they use it as meson dependency.
subdir('tests')
subdir('examples')

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

