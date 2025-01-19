Mapping xml protocol to C++
===========================

Wayland primitives
------------------

.. list-table:: Primitives
   :header-rows: 1

   * - Wayland data type
     - Waylander data type
   * - :code:`int`
     - :code:`waylander::wl::Wint`
   * - :code:`uint`
     - :code:`waylander::wl::Wuint`
   * - :code:`fixed`
     - :code:`waylander::wl::Wfixed`
   * - :code:`string`
     - :code:`waylander::wl::Wstring`
   * - :code:`object`
     - :code:`waylander::wl::Wobject<interface>`
   * - :code:`new_id`
     - :code:`waylander::wl::Wnew_id<interface>`
   * - :code:`array`
     - :code:`waylander::wl::Warray`
   * - :code:`fd`
     - :code:`waylander::wl::Wfd`

Waylander also defines following primitive data types:

- :code:`waylander::wl::Wmessage_size`
- :code:`waylander::wl::Woptcode<interface>`
- :code:`waylander::wl::message_header<interface>`

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
     - struct :code:`waylander::wl::protocols::<name of interface>`
   * - request
     - nested struct with args as data members
       :code:`waylander::wl::protocols::<name of interface>::request::<name of request>`
   * - event
     - nested struct with args as data members
       :code:`waylander::wl::protocols::<name of interface>::event::<name of event>`
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
