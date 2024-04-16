File descriptor abstraction design requirements
-----------------------------------------------

.. #) creates ordered list.

#) all file descriptors can...:
    #) ...allocate resource
    #) ...free resource
    #) ...write span
    #) ...read to span
    #) ...handle all error messages
#) sockets can...:
    #) ...connect
#) files can...:
    #) ...be opened without danger of overwriting anything
    #) ...can be closed

Options for file descriptor operation interfaces
------------------------------------------------

- member functions
- free functions

Options for file descriptor abstraction
---------------------------------------

Define:

- TA = type actually used in application code
- TBi = i:th type used in the impelemntation of TA

Trait system
^^^^^^^^^^^^

TA declare tags TBi inside TA which are used to to dispatch to right functions.

Static inheritance
^^^^^^^^^^^^^^^^^^

Copy what std::iostream is doing, i.e. TA inherits from TBi.
In std::iostream TBi are:

- :code:`std::ios_base` maintains data:

    - state information: stream status flags.

    - control information: flags that control formatting of both
      input and output sequences and the imbued locale.

    - private storage: indexed extensible data structure that
      allows both long and void* members, which may be implemented
      as two arbitrary-length arrays or a single array of two-element structs or another container.

    - callbacks: arbitrary number of user-defined functions
      to be called from :code:`imbue()`, :code:`std::basic_ios::copyfmt()`,
      and :code:`~ios_base()`.

- :code:`std::basic_ios` provides facilities for interfacing with objects
  that have :code:`std::basic_streambuf` interface.

- :code:`std::basic_istream` and :code:`std::basic_ostream` provides support for high level io,
  implemented in terms of the interface provided by the underlying :code:`basic_streambuf` class,
  accessed through  the :code:`basic_ios` base class.

- :code:`std::basic_iostrem` combindes :code:`std::basic_istream` and :code:`std::basic_ostream`.

:code:`ger::gnu` desing
-----------------------

system resource
    Resources which management needs to be negotiated with operating system.

Following graph shows which system resources can be converted to other.

.. graphviz:: system_resource_conversions.dot

Implicit conversion are transformations of the system resources types,
so they do not read, write or copy memory behind any file descriptor.
Note that mmap does not do any of these.
Because of paging memory operation happens accessing the mapped memory.

Explicit conversions have to, either explicitly or by asking the kernel,
read, write or copy memory behind any file descriptor.
Conversion function that consume the converted resource count as explicit conversion.

Data memory file descriptor does not include metadata of the file descriptor,
that kernel keeps up to date automatically.
Note that this allows opening and closing file descriptors themself.

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

See: https://www.gnu.org/software/gnulib/manual/html_node/Closed-standard-fds.html
