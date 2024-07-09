# Description

Guilander is dependency free (*) C++26 Wayland client library
for writing Wayland client applications.

It encodes Wayland protocol to C++ type system by generating C++ headers from protocol XML,
in which Wayland interfaces and messages are represented by types, built form set of primitives.
Communication using the Wayland wire format is implemented
using bleeding edge C++ compile time techniques based on the generated types.

(*) Guilander uses some modules from source code library Gnulib,
which means that they are bundled with Guilander itself.

## Wayland XML to Guilander C++ protocol header

To get an idea of how Guilander maps Wayland XML to C++ here is a part of `wl_display` interface:

```xml
<interface name="wl_display" version="1">
  <request name="sync">
    <arg name="callback" type="new_id" interface="wl_callback"
     summary="callback object for the sync request"/>
  </request>

  <event name="error">
    <arg name="object_id" type="object" summary="object where the error occurred"/>
    <arg name="code" type="uint" summary="error code"/>
    <arg name="message" type="string" summary="error description"/>
  </event>

  <enum name="error">
    <entry name="invalid_object" value="0"
       summary="server couldn't find object"/>
    <entry name="invalid_method" value="1"
       summary="method doesn't exist on the specified interface or malformed request"/>
    <entry name="no_memory" value="2"
       summary="server is out of memory"/>
    <entry name="implementation" value="3"
       summary="implementation error in compositor"/>
  </enum>
</interface>
```

We can see that the interface `wl_display` contains a request `sync`, an event `error`
and an enum `error`. Guilander comes with XML `xml-protocol-tool` which generates
approximately following C++ code from the previous XML (`include/wayland/protocols/wayland_protocol.hpp`):

```cpp
struct wl_display {
    enum class error : Wint::integral_type {
        /// server couldn't find object
        Einvalid_object = 0,
        /// method doesn't exist on the specified interface or malformed request
        Einvalid_method = 1,
        /// server is out of memory
        Eno_memory = 2,
        /// implementation error in compositor
        Eimplementation = 3
    };

    struct request {
        struct sync {
            static constexpr Wopcode<wl_display> opcode{ 0 };
            /// callback object for the sync request
            Wnew_id<wl_callback> callback;
        };
    };

    struct event {
        struct error {
            static constexpr Wopcode<wl_display> opcode{ 0 };
            /// object where the error occurred
            Wobject<> object_id;
            /// error code
            Wuint code;
            /// error description
            Wstring message;
        };
    };
}
```

The types with `W` prefix are Guilander primitives,
which are defined in `include/wayland/protocol_primitives.hpp`.
There is a one for each of the Wayland data types:
`Wint`, `Wuint`, `Wfixed`, `Wstring`, `Wobject<interface>`,
`Wnew_id<interface>`, `Warray` and `Wfd`.
There is also three Guilander specific primitives:
`Wmessage_size`, `Wopcode<interface>` and `message_header<interface>`.

## Application examples

For simple listing of Wayland registry globals, see `examples/list_registry_globals.cpp`.

For more complex example, see `examples/wayland/display_static_picture.cpp`
which implements Wayland client to display static picture of a Sierpinski's Carpet.

# Status

Currently, Guilander is considered to be in an alpha stage,
which means that some features are missing, and it might go through large changes in the future.

## Todo

- [ ] Receiving events which contain file descriptors.
- [ ] Versioned protocols (atm version numbers from XML are ignored).
- [ ] `Wfixed` utilities.

# Building

Guilander uses Meson as build system and provides `guildander_dep` meson dependency
for other projects to consume. At the moment this is the only way to consume Guilander.

Because of the bleeding edge nature of the project currently only GCC 14.1.1 or newer
is required to build Guilander.

## Examples

```shell
meson setup build && ninja -C build examples/eg<N>_<example_name>
```

Where `N` is a ordinal given to example source file `examples/**/<example_name>.cpp`
in `examples/meson.build`.

## Tests

```shell
meson setup build && ninja -C build test
```

## Generated protocol headers

Guilander comes with all `stable` Wayland protocols (`include/wayland/protocols/*_protocol.hpp`),
which are generated using `xml-protocol-tool`.

# Author

Miro Palmu, email@miropalmu.cc
