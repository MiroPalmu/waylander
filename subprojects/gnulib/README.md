Guilander wants to use some modules from gnulib,
however gnulib assumes that projects that use it are build with gnu build system.

This subdirectory is for setting mock gnu build system for building libgnu.a,
in such way that meson can build it with custom target.

Every generated file in source tree should be tracked with git, as it is assumed that,
this directory is setup once and rarely updated and we do not want to meson to gnereate *any*
files to source tree. Only to build tree.

# Initial import process

At commit 4f3f6b3 build(gnulib): add subdir and configure.ac (hash potentially going to change),
following command was executed in guilander/gnulib:

```bash
$ /path/to/gnulib/gnulib-tool --aux-dir=build-aux --import full-read full-write

Module list with included dependencies (indented):
    absolute-header
    extensions
    extern-inline
  full-read
  full-write
    gen-header
    include_next
    msvc-inval
    msvc-nothrow
    raise
    read
    safe-read
    safe-write
    signal-h
    snippet/arg-nonnull
    snippet/c++defs
    snippet/warn-on-use
    ssize_t
    stddef
    sys_types
    unistd
    write
File list:
  lib/arg-nonnull.h
  lib/c++defs.h
  lib/full-read.c
  lib/full-read.h
  lib/full-write.c
  lib/full-write.h
  lib/msvc-inval.c
  lib/msvc-inval.h
  lib/msvc-nothrow.c
  lib/msvc-nothrow.h
  lib/raise.c
  lib/read.c
  lib/safe-read.c
  lib/safe-read.h
  lib/safe-write.c
  lib/safe-write.h
  lib/signal.in.h
  lib/stddef.in.h
  lib/sys-limits.h
  lib/sys_types.in.h
  lib/unistd.c
  lib/unistd.in.h
  lib/warn-on-use.h
  lib/write.c
  m4/00gnulib.m4
  m4/absolute-header.m4
  m4/extensions.m4
  m4/extern-inline.m4
  m4/gnulib-common.m4
  m4/include_next.m4
  m4/msvc-inval.m4
  m4/msvc-nothrow.m4
  m4/off_t.m4
  m4/pid_t.m4
  m4/raise.m4
  m4/read.m4
  m4/safe-read.m4
  m4/safe-write.m4
  m4/signal_h.m4
  m4/ssize_t.m4
  m4/stddef_h.m4
  m4/sys_types_h.m4
  m4/unistd_h.m4
  m4/warn-on-use.m4
  m4/wchar_t.m4
  m4/write.m4
  m4/zzgnulib.m4
Creating directory ./lib
Creating directory ./m4
Creating directory ./build-aux
Copying file lib/arg-nonnull.h
Copying file lib/c++defs.h
Copying file lib/full-read.c
Copying file lib/full-read.h
Copying file lib/full-write.c
Copying file lib/full-write.h
Copying file lib/msvc-inval.c
Copying file lib/msvc-inval.h
Copying file lib/msvc-nothrow.c
Copying file lib/msvc-nothrow.h
Copying file lib/raise.c
Copying file lib/read.c
Copying file lib/safe-read.c
Copying file lib/safe-read.h
Copying file lib/safe-write.c
Copying file lib/safe-write.h
Copying file lib/signal.in.h
Copying file lib/stddef.in.h
Copying file lib/sys-limits.h
Copying file lib/sys_types.in.h
Copying file lib/unistd.c
Copying file lib/unistd.in.h
Copying file lib/warn-on-use.h
Copying file lib/write.c
Copying file m4/00gnulib.m4
Copying file m4/absolute-header.m4
Copying file m4/extensions.m4
Copying file m4/extern-inline.m4
Copying file m4/gnulib-common.m4
Copying file m4/gnulib-tool.m4
Copying file m4/include_next.m4
Copying file m4/msvc-inval.m4
Copying file m4/msvc-nothrow.m4
Copying file m4/off_t.m4
Copying file m4/pid_t.m4
Copying file m4/raise.m4
Copying file m4/read.m4
Copying file m4/safe-read.m4
Copying file m4/safe-write.m4
Copying file m4/signal_h.m4
Copying file m4/ssize_t.m4
Copying file m4/stddef_h.m4
Copying file m4/sys_types_h.m4
Copying file m4/unistd_h.m4
Copying file m4/warn-on-use.m4
Copying file m4/wchar_t.m4
Copying file m4/write.m4
Copying file m4/zzgnulib.m4
Creating m4/gnulib-cache.m4
Creating m4/gnulib-comp.m4
Creating lib/Makefile.am
Finished.

You may need to add #include directives for the following .h files.
  #include "full-read.h"
  #include "full-write.h"

Don't forget to
  - add "lib/Makefile" to AC_CONFIG_FILES in ./configure.ac,
  - mention "lib" in SUBDIRS in Makefile.am,
  - mention "-I m4" in ACLOCAL_AMFLAGS in Makefile.am
    or add an AC_CONFIG_MACRO_DIRS([m4]) invocation in ./configure.ac,
  - mention "m4/gnulib-cache.m4" in EXTRA_DIST in Makefile.am,
  - invoke gl_EARLY in ./configure.ac, right after AC_PROG_CC,
  - invoke gl_INIT in ./configure.ac.
```
