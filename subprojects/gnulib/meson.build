project('gnulib', 'c')

# Tools:
make = find_program('make', required : true)
c_compiler = meson.get_compiler('c')


# This wrapper is needes,
# as ./configure has to be executed in spesific directory.
gnu_configure_meson_wrapper = files('gnu_configure_meson_wrapper.sh')

gnulib_configuration_env = environment({
    'CC' : c_compiler.cmd_array()[0],
})

gnulib_configured_makefile = custom_target(
    'gnulib-configuration',
    output : 'Makefile',
    install : false,
    env : gnulib_configuration_env,
    command : [
        gnu_configure_meson_wrapper,
        meson.current_source_dir() + '/configure',
        meson.current_build_dir()
    ],
)


# From gnulib manual:
#
#   > Finally, you have to add compiler and linker flags in the appropriate source directories,
#   > so that you can make use of the gnulib library. Since some modules (‘getopt’, for example)
#   > may copy files into the build directory, top_builddir/lib is needed as well as top_srcdir/lib.
#
# Apparently include_directories('foo') will make both sourcedir/.../foo and builddir/.../foo
# include directories.
gnulib_include_dirs = [
    include_directories('.'),
    include_directories('lib'),
]

# Generated headers in <gnulib_builddir>/lib have to be collected to custom_target,
# to be abel to give them to gnulib_dep.sources. This is required as having only
# the gnulib_include_dirs is unreliable. Following links are potentially related:
#
# - https://github.com/mesonbuild/meson/issues/1855
# - https://github.com/mesonbuild/meson/pull/225
#
# lib/meson.build will declare gnulib_generated_headers_in_lib custom_target:
subdir('lib')

# Assume that building this handle with make is going to also build lib/libgnu.a.
gnulib_meson_handle = custom_target(
    'gnulib-meson-handle',
    input : [
        gnulib_configured_makefile,
        gnulib_generated_headers_in_lib,
        files('handle_for_meson.c'),
    ],
    output : 'handle_for_meson',
    install : false,
    env : gnulib_configuration_env,
    command : [make, '-C', meson.current_build_dir()],
)

gnulib_link_args = [
    '-L' + meson.current_build_dir() + '/lib',
    '-lgnu',
]

gnulib_dep = declare_dependency(
    sources : gnulib_meson_handle,
    include_directories : gnulib_include_dirs,
    link_args : gnulib_link_args,
)
