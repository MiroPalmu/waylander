Usage of Gnulib
===============

Replacement headers
-------------------

Let's say that :code:`foo.cpp` uses libc header :code:`<stddef.h>` either directly
or possibly indirectly trough C++ standard library which includes it.

If compilation flags contain :code:`-L <gnuilb/include/dir>`, then the C pre processor
will find :code:`-L <gnuilb/include/dir>/stddef.h`, which is replacement header for the system one.
This takes precedence and using :code:`#include_next` GNU extension the replaced header
will be included, but in controlled manner.

By default if target system of *gnulib* contains, by some definition of, broken libc function,
then the replacement header will use following macro technique to replace the broken function
with Gnulib one (Gnulib manual: 2.6.1 Support for ISO C or POSIX functions):

.. code-block:: c

    #if !HAVE_WORKING_FOO
    # define foo rpl_foo
    #endif

If :code:`GNULIB_NAMESPACE` is defined to be :code:`X` before including replacement header,
then insted of the above macro technique, :code:`rpl_foo` will be defined as :code:`X::foo`
(Gnulib manual: 6.5 A C++ namespace for gnulib).

Safer modules
-------------

Modules :code:`*-safer` implement safer replacements for corresponding libc functions.

Gnulib manual (17.10 Handling closed standard file descriptors) advocates use of headers
:code:`<libc-header>--.h`, which will include :code:`<libc-header>` and :code:`<libc-header>-safer.h`,
and then just define macros that replace the "unsafe" functions with the safer ones from
:code:`<libc-header-safer.h`.

.. note::

    Above is observation based on the current used modules in guilander.
    In these modules only :code:`<libc-header>--.h` header is :code:`unistd--.h`.

These macros might not not be desired and :code:`<libc-header>-safer.h` should be included directly.

.. note::

    Declarations in :code:`<libc-header>-safer.h` do not use :code:`GNULIB_NAMESPACE` mechanism.

Used Gnulib modules
-------------------

.. list-table:: libc gnulib replacements
    :widths: auto
    :header-rows: 1

    * - libc
      - gnulib module
    * - read
      - full-read
    * - write
      - full-write
    * - pipe
      - pipe2-safer
    * - mkfifo
      - mkfifo
    * - socket
      - socket
    * - connect
      - connect
    * - mmap
      - no subtitute module available
    * - open
      - open
    * - close
      - close
    * - fpathconf
      - no subtitue module available

For more information on :code:`*-safer` modules, see
Gnulib manual 17.10 Handling closed standard file descriptors.

Note that some modules uses :code:`*-safer` modules indirectly.

Guilander user POV requirements
-------------------------------

.. #) creates ordered list.

#) Guilander headers do not define any C preprocessor macros.
#) Application using guilander does not find any gnulib headers:
    #) :code:`config.h`
    #) libc replacement headers
    #) utility headers (e.g. :code:`safe-read.h` and :code:`unistd--.h`)

.. Saved for potential later use:

..     potential headers
..     │
..     ├── libc
..     │   ├── iso standard
..     │   │   ├── float.h
..     │   │   └── signal.h
..     │   └── posix extensions
..     │       ├── fcntl.h
..     │       └── unistd.h
..     ├── libgnu
..     │   ├── extensions
..     │   │   └── full_read.h
..     │   ├── libc header rpl
..     │   │   ├── fcntl.h
..     │   │   ├── float.h
..     │   │   ├── signal.h
..     │   │   └── unistd.h
..     │   └── libc safe rpl
..     │       ├── fcntl--.h
..     │       └── unistd--.h
..     └── libstdc++
..         ├── C compatibility
..         │   ├── float.h
..         │   └── signal.h
..         ├── libc from C++
..         │   ├── cfloat
..         │   └── csignal
..         └── C++
..             ├── cfloat
..             └── csignal
