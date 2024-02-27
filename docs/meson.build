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
