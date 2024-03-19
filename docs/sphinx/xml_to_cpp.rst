Mapping xml protocol to C++
===========================

Wayland primitives
------------------

.. list-table:: Primitives
   :header-rows: 1

   * - Wayland data type
     - Guilander data type
   * - :code:`int`
     - :code:`ger::wl::Wint`
   * - :code:`int`
     - :code:`ger::wl::Wuint`
   * - :code:`fixed`
     - :code:`ger::wl::Wfixed`
   * - :code:`string`
     - :code:`ger::wl::Wstring`
   * - :code:`object`
     - :code:`ger::wl::Wobject<interface>`
   * - :code:`new_id`
     - :code:`ger::wl::Wnew_id<interface>`
   * - :code:`array`
     - :code:`ger::wl::Warray`
   * - :code:`fd`
     - :code:`ger::wl::Wfd`

Guilander also defines following primitive data types:

- :code:`ger::wl::Wmessage_size`
- :code:`ger::wl::Woptcode<interface>`
- :code:`ger::wl::message_header<interface>`

:code:`interface` template arguments are constrained s.t. it is required
that they are types with following nested types:

- :code:`interface::request`
- :code:`interface::event`

Then the actual requests and events of interface are defined inside these types.

Xml nodes
---------

.. list-table:: Nodes
   :header-rows: 1

   * - xml
     - C++
   * - protocol
     - header :code:`<name of protocol>_protocol.hpp`
   * - copyright
     - comment in a protocol header
   * - interface
     - struct :code:`ger::wl::protocols::<name of interface>`
   * - request
     - nested struct with args as data members
       :code:`ger::wl::protocols::<name of interface>::request::<name of request>`
   * - event
     - nested struct with args as data members
       :code:`ger::wl::protocols::<name of interface>::event::<name of event>`
   * - enum
     - nested enum class in interface struct
   * - entry
     - enum class entry which identifier is name of the entry with "E" prefix
   * - arg
     - data member of request or event
   * - description
     - sphinx comment

Requests and events are messages which all have static datamember of type
:code:`Wopcode<interface> opcode`.
