Developing guidelines
=====================

Doxygen documentation style is followed.

Sphinx pages can interact with Doxygen using
Breathe plugin through Doxygen xml output.

To generate Doxygen documentation
(``<build_dir>/docs/doxygen/html/index.html``)
build **doxygen** target.


To generate Sphinx documentation
(``<build_dir>/docs/sphinx/build/index.html``)
build **sphinx** target.

comiple_commands.json
---------------------

Meson ninja backend generates automatically
``<build_dir>/compile_commands.json``

However this does not contain entries for header
files, only for source files (c++). This means
that tools utilizing compile_commands.json might
not work for header files.

To generate ``<project_root>/compile_comands.json``
containing entries also for header files build
**compdb** target.

**compdb** target requires that ``compdb`` cli tool
is found and that ninja backend for meson is used.

``compdb`` can be installed with ``pip install compdb``

Commit messages
---------------

Commit messages follow `Conventional Commits v1.0.0`_.

Following types of commits are allowed:

* **build**: Changes that affect the build system or external dependencies (example scopes: version, dep)
* **ci**: Changes to our CI configuration files and scripts
* **docs**: Documentation only changes (example scopes: readme, doxygen, sphinx)
* **feat**: A new feature
* **fix**: A bug fix
* **perf**: A code change that improves performance
* **refactor**: A code change that neither fixes a bug nor adds a feature
* **style**: Changes that do not affect the meaning of the code (white-space, formatting, missing semi-colons, etc)
* **test**: Adding missing tests or correcting existing tests
* **examples**: A commit related to examples.

Types are borrowed from `Angular`_.

.. _Conventional Commits v1.0.0: https://www.conventionalcommits.org/en/v1.0.0/
.. _Angular: https://github.com/angular/angular/blob/22b96b9/CONTRIBUTING.md#-commit-message-guidelines
