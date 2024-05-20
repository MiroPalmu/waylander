# See ../meson.build for more information.
gnulib_generated_headers_in_lib = custom_target(
    'gnulib-generated-headers',
    input : [gnulib_configured_makefile],
    output : [
        'fcntl.h',
        'alloca.h',
        'limits.h',
        'signal.h',
        'stddef.h',
        'time.h',
        'unistd.h',
    ],
    command: ['true'],
)
