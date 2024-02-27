sphinx_configuration_data = configuration_data({
    'PROJECT' : meson.project_name(),
    'AUTHOR' : 'Miro Palmu',
    'VERSION' : meson.project_version(),
    'DOXYGEN_XML_OUTPUT_DIR' : doxygen_output_dir + '/xml',
    'THEME' : 'alabaster',
})

# Check if different themes is installed using python module of meson
py = import('python')

furo_theme = py.find_installation(
    'python3',
    modules : ['furo'],
    required : false
)

rtd_theme = py.find_installation(
    'python3',
    modules : ['sphinx_rtd_theme'],
    required : false
)

if furo_theme.found()
    sphinx_configuration_data.set('THEME', 'furo')
elif rtd_theme.found()
    sphinx_configuration_data.set('THEME', 'sphinx_rtd_theme')
endif

sphinx_config_file = configure_file(
    input: 'conf.py.in',
    output: 'conf.py',
    configuration: sphinx_configuration_data,
    install: false
)

sphinx_html_target = custom_target(
    'sphinx_html_target',
    output : 'build',
    depends : doxygen_html_target,
    depend_files : [ sphinx_config_file ],
    command : [
        sphinx, '-b', 'html',
        # Give folder of generated conf.py as -c argument to sphinx
        '-c', meson.current_build_dir(),
        # Source dir
        meson.current_source_dir(),
        # Output dir
        '@OUTPUT@',
    ],
    install : false,
    build_always_stale: true
)

alias_target(
    'sphinx',
    sphinx_html_target
)
