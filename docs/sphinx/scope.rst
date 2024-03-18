Scope of Guildander
===================

Guilander gloas
---------------

- C++ impelementation of Wayland protocol for clients.
- Be agnostic of:
    - async model
- Give example usecase:
    - print all globals
    - write ascii
    - draw ascii box
    - ascii grid

Guilander non-goals
-------------------

- Event loop implementation.
- Raii abstarction for Wayland objects.
- Anything that needs async library.
- Impelement any async library funcionalities in itself.
    - Slippery slope to add asyc features in sstd or similar.
- Abstract syscalls from different operating systems.
    - Only abstract just enough that spesific usecase is behind raii.
