// Generated from Wayland xml protocol: tablet_v2

/// @file
/// Wayland protocol for graphics tablets
///
/// This description provides a high-level overview of the interplay between
/// the interfaces defined this protocol. For details, see the protocol
/// specification.
///
/// More than one tablet may exist, and device-specifics matter. Tablets are
/// not represented by a single virtual device like wl_pointer. A client
/// binds to the tablet manager object which is just a proxy object. From
/// that, the client requests wp_tablet_manager.get_tablet_seat(wl_seat)
/// and that returns the actual interface that has all the tablets. With
/// this indirection, we can avoid merging wp_tablet into the actual Wayland
/// protocol, a long-term benefit.
///
/// The wp_tablet_seat sends a "tablet added" event for each tablet
/// connected. That event is followed by descriptive events about the
/// hardware; currently that includes events for name, vid/pid and
/// a wp_tablet.path event that describes a local path. This path can be
/// used to uniquely identify a tablet or get more information through
/// libwacom. Emulated or nested tablets can skip any of those, e.g. a
/// virtual tablet may not have a vid/pid. The sequence of descriptive
/// events is terminated by a wp_tablet.done event to signal that a client
/// may now finalize any initialization for that tablet.
///
/// Events from tablets require a tool in proximity. Tools are also managed
/// by the tablet seat; a "tool added" event is sent whenever a tool is new
/// to the compositor. That event is followed by a number of descriptive
/// events about the hardware; currently that includes capabilities,
/// hardware id and serial number, and tool type. Similar to the tablet
/// interface, a wp_tablet_tool.done event is sent to terminate that initial
/// sequence.
///
/// Any event from a tool happens on the wp_tablet_tool interface. When the
/// tool gets into proximity of the tablet, a proximity_in event is sent on
/// the wp_tablet_tool interface, listing the tablet and the surface. That
/// event is followed by a motion event with the coordinates. After that,
/// it's the usual motion, axis, button, etc. events. The protocol's
/// serialisation means events are grouped by wp_tablet_tool.frame events.
///
/// Two special events (that don't exist in X) are down and up. They signal
/// "tip touching the surface". For tablets without real proximity
/// detection, the sequence is: proximity_in, motion, down, frame.
///
/// When the tool leaves proximity, a proximity_out event is sent. If any
/// button is still down, a button release event is sent before this
/// proximity event. These button events are sent in the same frame as the
/// proximity event to signal to the client that the buttons were held when
/// the tool left proximity.
///
/// If the tool moves out of the surface but stays in proximity (i.e.
/// between windows), compositor-specific grab policies apply. This usually
/// means that the proximity-out is delayed until all buttons are released.
///
/// Moving a tool physically from one tablet to the other has no real effect
/// on the protocol, since we already have the tool object from the "tool
/// added" event. All the information is already there and the proximity
/// events on both tablets are all a client needs to reconstruct what
/// happened.
///
/// Some extra axes are normalized, i.e. the client knows the range as
/// specified in the protocol (e.g. [0, 65535]), the granularity however is
/// unknown. The current normalized axes are pressure, distance, and slider.
///
/// Other extra axes are in physical units as specified in the protocol.
/// The current extra axes with physical units are tilt, rotation and
/// wheel rotation.
///
/// Since tablets work independently of the pointer controlled by the mouse,
/// the focus handling is independent too and controlled by proximity.
/// The wp_tablet_tool.set_cursor request sets a tool-specific cursor.
/// This cursor surface may be the same as the mouse cursor, and it may be
/// the same across tools but it is possible to be more fine-grained. For
/// example, a client may set different cursors for the pen and eraser.
///
/// Tools are generally independent of tablets and it is
/// compositor-specific policy when a tool can be removed. Common approaches
/// will likely include some form of removing a tool when all tablets the
/// tool was used on are removed.
///
/// Copyright 2014 © Stephen "Lyude" Chandler Paul
/// Copyright 2015-2016 © Red Hat, Inc.
///
/// Permission is hereby granted, free of charge, to any person
/// obtaining a copy of this software and associated documentation files
/// (the "Software"), to deal in the Software without restriction,
/// including without limitation the rights to use, copy, modify, merge,
/// publish, distribute, sublicense, and/or sell copies of the Software,
/// and to permit persons to whom the Software is furnished to do so,
/// subject to the following conditions:
///
/// The above copyright notice and this permission notice (including the
/// next paragraph) shall be included in all copies or substantial
/// portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
/// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
/// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
/// NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
/// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
/// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
/// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#pragma once

#include "guilander/wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {
namespace protocols {

/// Declare everything before they might be used.

struct zwp_tablet_manager_v2 {
    struct request {
        struct get_tablet_seat;
        struct destroy;
    };

    struct event {};
};
struct zwp_tablet_seat_v2 {
    struct request {
        struct destroy;
    };

    struct event {
        struct tablet_added;
        struct tool_added;
        struct pad_added;
    };
};
struct zwp_tablet_tool_v2 {
    enum class type : Wint::integral_type;
    enum class capability : Wint::integral_type;
    enum class button_state : Wint::integral_type;
    enum class error : Wint::integral_type;

    struct request {
        struct set_cursor;
        struct destroy;
    };

    struct event {
        struct type;
        struct hardware_serial;
        struct hardware_id_wacom;
        struct capability;
        struct done;
        struct removed;
        struct proximity_in;
        struct proximity_out;
        struct down;
        struct up;
        struct motion;
        struct pressure;
        struct distance;
        struct tilt;
        struct rotation;
        struct slider;
        struct wheel;
        struct button;
        struct frame;
    };
};
struct zwp_tablet_v2 {
    struct request {
        struct destroy;
    };

    struct event {
        struct name;
        struct id;
        struct path;
        struct done;
        struct removed;
    };
};
struct zwp_tablet_pad_ring_v2 {
    enum class source : Wint::integral_type;

    struct request {
        struct set_feedback;
        struct destroy;
    };

    struct event {
        struct source;
        struct angle;
        struct stop;
        struct frame;
    };
};
struct zwp_tablet_pad_strip_v2 {
    enum class source : Wint::integral_type;

    struct request {
        struct set_feedback;
        struct destroy;
    };

    struct event {
        struct source;
        struct position;
        struct stop;
        struct frame;
    };
};
struct zwp_tablet_pad_group_v2 {
    struct request {
        struct destroy;
    };

    struct event {
        struct buttons;
        struct ring;
        struct strip;
        struct modes;
        struct done;
        struct mode_switch;
    };
};
struct zwp_tablet_pad_v2 {
    enum class button_state : Wint::integral_type;

    struct request {
        struct set_feedback;
        struct destroy;
    };

    struct event {
        struct group;
        struct path;
        struct buttons;
        struct done;
        struct button;
        struct enter;
        struct leave;
        struct removed;
    };
};
/// get the tablet seat
///
/// Get the wp_tablet_seat object for the given seat. This object
/// provides access to all graphics tablets in this seat.
struct zwp_tablet_manager_v2::request::get_tablet_seat {
    static constexpr Wopcode<zwp_tablet_manager_v2> opcode{ 0 };
    Wnew_id<zwp_tablet_seat_v2> tablet_seat;
    /// The wl_seat object to retrieve the tablets for
    Wobject<wl_seat> seat;
};

/// release the memory for the tablet manager object
///
/// Destroy the wp_tablet_manager object. Objects created from this
/// object are unaffected and should be destroyed separately.
struct zwp_tablet_manager_v2::request::destroy {
    static constexpr Wopcode<zwp_tablet_manager_v2> opcode{ 1 };
};

/// release the memory for the tablet seat object
///
/// Destroy the wp_tablet_seat object. Objects created from this
/// object are unaffected and should be destroyed separately.
struct zwp_tablet_seat_v2::request::destroy {
    static constexpr Wopcode<zwp_tablet_seat_v2> opcode{ 0 };
};

/// new device notification
///
/// This event is sent whenever a new tablet becomes available on this
/// seat. This event only provides the object id of the tablet, any
/// static information about the tablet (device name, vid/pid, etc.) is
/// sent through the wp_tablet interface.
struct zwp_tablet_seat_v2::event::tablet_added {
    static constexpr Wopcode<zwp_tablet_seat_v2> opcode{ 0 };
    /// the newly added graphics tablet
    Wnew_id<zwp_tablet_v2> id;
};

/// a new tool has been used with a tablet
///
/// This event is sent whenever a tool that has not previously been used
/// with a tablet comes into use. This event only provides the object id
/// of the tool; any static information about the tool (capabilities,
/// type, etc.) is sent through the wp_tablet_tool interface.
struct zwp_tablet_seat_v2::event::tool_added {
    static constexpr Wopcode<zwp_tablet_seat_v2> opcode{ 1 };
    /// the newly added tablet tool
    Wnew_id<zwp_tablet_tool_v2> id;
};

/// new pad notification
///
/// This event is sent whenever a new pad is known to the system. Typically,
/// pads are physically attached to tablets and a pad_added event is
/// sent immediately after the wp_tablet_seat.tablet_added.
/// However, some standalone pad devices logically attach to tablets at
/// runtime, and the client must wait for wp_tablet_pad.enter to know
/// the tablet a pad is attached to.
///
/// This event only provides the object id of the pad. All further
/// features (buttons, strips, rings) are sent through the wp_tablet_pad
/// interface.
struct zwp_tablet_seat_v2::event::pad_added {
    static constexpr Wopcode<zwp_tablet_seat_v2> opcode{ 2 };
    /// the newly added pad
    Wnew_id<zwp_tablet_pad_v2> id;
};

/// a physical tool type
///
/// Describes the physical type of a tool. The physical type of a tool
/// generally defines its base usage.
///
/// The mouse tool represents a mouse-shaped tool that is not a relative
/// device but bound to the tablet's surface, providing absolute
/// coordinates.
///
/// The lens tool is a mouse-shaped tool with an attached lens to
/// provide precision focus.
enum class zwp_tablet_tool_v2::type : Wint::integral_type {
    /// Pen
    Epen = 320,
    /// Eraser
    Eeraser = 321,
    /// Brush
    Ebrush = 322,
    /// Pencil
    Epencil = 323,
    /// Airbrush
    Eairbrush = 324,
    /// Finger
    Efinger = 325,
    /// Mouse
    Emouse = 326,
    /// Lens
    Elens = 327
};

/// capability flags for a tool
///
/// Describes extra capabilities on a tablet.
///
/// Any tool must provide x and y values, extra axes are
/// device-specific.
enum class zwp_tablet_tool_v2::capability : Wint::integral_type {
    /// Tilt axes
    Etilt = 1,
    /// Pressure axis
    Epressure = 2,
    /// Distance axis
    Edistance = 3,
    /// Z-rotation axis
    Erotation = 4,
    /// Slider axis
    Eslider = 5,
    /// Wheel axis
    Ewheel = 6
};

/// physical button state
///
/// Describes the physical state of a button that produced the button event.
enum class zwp_tablet_tool_v2::button_state : Wint::integral_type {
    /// button is not pressed
    Ereleased = 0,
    /// button is pressed
    Epressed = 1
};

enum class zwp_tablet_tool_v2::error : Wint::integral_type {
    /// given wl_surface has another role
    Erole = 0
};

/// set the tablet tool's surface
///
/// Sets the surface of the cursor used for this tool on the given
/// tablet. This request only takes effect if the tool is in proximity
/// of one of the requesting client's surfaces or the surface parameter
/// is the current pointer surface. If there was a previous surface set
/// with this request it is replaced. If surface is NULL, the cursor
/// image is hidden.
///
/// The parameters hotspot_x and hotspot_y define the position of the
/// pointer surface relative to the pointer location. Its top-left corner
/// is always at (x, y) - (hotspot_x, hotspot_y), where (x, y) are the
/// coordinates of the pointer location, in surface-local coordinates.
///
/// On surface.attach requests to the pointer surface, hotspot_x and
/// hotspot_y are decremented by the x and y parameters passed to the
/// request. Attach must be confirmed by wl_surface.commit as usual.
///
/// The hotspot can also be updated by passing the currently set pointer
/// surface to this request with new values for hotspot_x and hotspot_y.
///
/// The current and pending input regions of the wl_surface are cleared,
/// and wl_surface.set_input_region is ignored until the wl_surface is no
/// longer used as the cursor. When the use as a cursor ends, the current
/// and pending input regions become undefined, and the wl_surface is
/// unmapped.
///
/// This request gives the surface the role of a wp_tablet_tool cursor. A
/// surface may only ever be used as the cursor surface for one
/// wp_tablet_tool. If the surface already has another role or has
/// previously been used as cursor surface for a different tool, a
/// protocol error is raised.
struct zwp_tablet_tool_v2::request::set_cursor {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 0 };
    /// serial of the proximity_in event
    Wuint serial;
    Wobject<wl_surface> surface;
    /// surface-local x coordinate
    Wint hotspot_x;
    /// surface-local y coordinate
    Wint hotspot_y;
};

/// destroy the tool object
///
/// This destroys the client's resource for this tool object.
struct zwp_tablet_tool_v2::request::destroy {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 1 };
};

/// tool type
///
/// The tool type is the high-level type of the tool and usually decides
/// the interaction expected from this tool.
///
/// This event is sent in the initial burst of events before the
/// wp_tablet_tool.done event.
struct zwp_tablet_tool_v2::event::type {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 0 };
    /// the physical tool type
    zwp_tablet_tool_v2::type tool_type;
};

/// unique hardware serial number of the tool
///
/// If the physical tool can be identified by a unique 64-bit serial
/// number, this event notifies the client of this serial number.
///
/// If multiple tablets are available in the same seat and the tool is
/// uniquely identifiable by the serial number, that tool may move
/// between tablets.
///
/// Otherwise, if the tool has no serial number and this event is
/// missing, the tool is tied to the tablet it first comes into
/// proximity with. Even if the physical tool is used on multiple
/// tablets, separate wp_tablet_tool objects will be created, one per
/// tablet.
///
/// This event is sent in the initial burst of events before the
/// wp_tablet_tool.done event.
struct zwp_tablet_tool_v2::event::hardware_serial {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 1 };
    /// the unique serial number of the tool, most significant bits
    Wuint hardware_serial_hi;
    /// the unique serial number of the tool, least significant bits
    Wuint hardware_serial_lo;
};

/// hardware id notification in Wacom's format
///
/// This event notifies the client of a hardware id available on this tool.
///
/// The hardware id is a device-specific 64-bit id that provides extra
/// information about the tool in use, beyond the wl_tool.type
/// enumeration. The format of the id is specific to tablets made by
/// Wacom Inc. For example, the hardware id of a Wacom Grip
/// Pen (a stylus) is 0x802.
///
/// This event is sent in the initial burst of events before the
/// wp_tablet_tool.done event.
struct zwp_tablet_tool_v2::event::hardware_id_wacom {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 2 };
    /// the hardware id, most significant bits
    Wuint hardware_id_hi;
    /// the hardware id, least significant bits
    Wuint hardware_id_lo;
};

/// tool capability notification
///
/// This event notifies the client of any capabilities of this tool,
/// beyond the main set of x/y axes and tip up/down detection.
///
/// One event is sent for each extra capability available on this tool.
///
/// This event is sent in the initial burst of events before the
/// wp_tablet_tool.done event.
struct zwp_tablet_tool_v2::event::capability {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 3 };
    /// the capability
    zwp_tablet_tool_v2::capability capability;
};

/// tool description events sequence complete
///
/// This event signals the end of the initial burst of descriptive
/// events. A client may consider the static description of the tool to
/// be complete and finalize initialization of the tool.
struct zwp_tablet_tool_v2::event::done {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 4 };
};

/// tool removed
///
/// This event is sent when the tool is removed from the system and will
/// send no further events. Should the physical tool come back into
/// proximity later, a new wp_tablet_tool object will be created.
///
/// It is compositor-dependent when a tool is removed. A compositor may
/// remove a tool on proximity out, tablet removal or any other reason.
/// A compositor may also keep a tool alive until shutdown.
///
/// If the tool is currently in proximity, a proximity_out event will be
/// sent before the removed event. See wp_tablet_tool.proximity_out for
/// the handling of any buttons logically down.
///
/// When this event is received, the client must wp_tablet_tool.destroy
/// the object.
struct zwp_tablet_tool_v2::event::removed {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 5 };
};

/// proximity in event
///
/// Notification that this tool is focused on a certain surface.
///
/// This event can be received when the tool has moved from one surface to
/// another, or when the tool has come back into proximity above the
/// surface.
///
/// If any button is logically down when the tool comes into proximity,
/// the respective button event is sent after the proximity_in event but
/// within the same frame as the proximity_in event.
struct zwp_tablet_tool_v2::event::proximity_in {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 6 };
    Wuint serial;
    /// The tablet the tool is in proximity of
    Wobject<zwp_tablet_v2> tablet;
    /// The current surface the tablet tool is over
    Wobject<wl_surface> surface;
};

/// proximity out event
///
/// Notification that this tool has either left proximity, or is no
/// longer focused on a certain surface.
///
/// When the tablet tool leaves proximity of the tablet, button release
/// events are sent for each button that was held down at the time of
/// leaving proximity. These events are sent before the proximity_out
/// event but within the same wp_tablet.frame.
///
/// If the tool stays within proximity of the tablet, but the focus
/// changes from one surface to another, a button release event may not
/// be sent until the button is actually released or the tool leaves the
/// proximity of the tablet.
struct zwp_tablet_tool_v2::event::proximity_out {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 7 };
};

/// tablet tool is making contact
///
/// Sent whenever the tablet tool comes in contact with the surface of the
/// tablet.
///
/// If the tool is already in contact with the tablet when entering the
/// input region, the client owning said region will receive a
/// wp_tablet.proximity_in event, followed by a wp_tablet.down
/// event and a wp_tablet.frame event.
///
/// Note that this event describes logical contact, not physical
/// contact. On some devices, a compositor may not consider a tool in
/// logical contact until a minimum physical pressure threshold is
/// exceeded.
struct zwp_tablet_tool_v2::event::down {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 8 };
    Wuint serial;
};

/// tablet tool is no longer making contact
///
/// Sent whenever the tablet tool stops making contact with the surface of
/// the tablet, or when the tablet tool moves out of the input region
/// and the compositor grab (if any) is dismissed.
///
/// If the tablet tool moves out of the input region while in contact
/// with the surface of the tablet and the compositor does not have an
/// ongoing grab on the surface, the client owning said region will
/// receive a wp_tablet.up event, followed by a wp_tablet.proximity_out
/// event and a wp_tablet.frame event. If the compositor has an ongoing
/// grab on this device, this event sequence is sent whenever the grab
/// is dismissed in the future.
///
/// Note that this event describes logical contact, not physical
/// contact. On some devices, a compositor may not consider a tool out
/// of logical contact until physical pressure falls below a specific
/// threshold.
struct zwp_tablet_tool_v2::event::up {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 9 };
};

/// motion event
///
/// Sent whenever a tablet tool moves.
struct zwp_tablet_tool_v2::event::motion {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 10 };
    /// surface-local x coordinate
    Wfixed x;
    /// surface-local y coordinate
    Wfixed y;
};

/// pressure change event
///
/// Sent whenever the pressure axis on a tool changes. The value of this
/// event is normalized to a value between 0 and 65535.
///
/// Note that pressure may be nonzero even when a tool is not in logical
/// contact. See the down and up events for more details.
struct zwp_tablet_tool_v2::event::pressure {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 11 };
    /// The current pressure value
    Wuint pressure;
};

/// distance change event
///
/// Sent whenever the distance axis on a tool changes. The value of this
/// event is normalized to a value between 0 and 65535.
///
/// Note that distance may be nonzero even when a tool is not in logical
/// contact. See the down and up events for more details.
struct zwp_tablet_tool_v2::event::distance {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 12 };
    /// The current distance value
    Wuint distance;
};

/// tilt change event
///
/// Sent whenever one or both of the tilt axes on a tool change. Each tilt
/// value is in degrees, relative to the z-axis of the tablet.
/// The angle is positive when the top of a tool tilts along the
/// positive x or y axis.
struct zwp_tablet_tool_v2::event::tilt {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 13 };
    /// The current value of the X tilt axis
    Wfixed tilt_x;
    /// The current value of the Y tilt axis
    Wfixed tilt_y;
};

/// z-rotation change event
///
/// Sent whenever the z-rotation axis on the tool changes. The
/// rotation value is in degrees clockwise from the tool's
/// logical neutral position.
struct zwp_tablet_tool_v2::event::rotation {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 14 };
    /// The current rotation of the Z axis
    Wfixed degrees;
};

/// Slider position change event
///
/// Sent whenever the slider position on the tool changes. The
/// value is normalized between -65535 and 65535, with 0 as the logical
/// neutral position of the slider.
///
/// The slider is available on e.g. the Wacom Airbrush tool.
struct zwp_tablet_tool_v2::event::slider {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 15 };
    /// The current position of slider
    Wint position;
};

/// Wheel delta event
///
/// Sent whenever the wheel on the tool emits an event. This event
/// contains two values for the same axis change. The degrees value is
/// in the same orientation as the wl_pointer.vertical_scroll axis. The
/// clicks value is in discrete logical clicks of the mouse wheel. This
/// value may be zero if the movement of the wheel was less
/// than one logical click.
///
/// Clients should choose either value and avoid mixing degrees and
/// clicks. The compositor may accumulate values smaller than a logical
/// click and emulate click events when a certain threshold is met.
/// Thus, wl_tablet_tool.wheel events with non-zero clicks values may
/// have different degrees values.
struct zwp_tablet_tool_v2::event::wheel {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 16 };
    /// The wheel delta in degrees
    Wfixed degrees;
    /// The wheel delta in discrete clicks
    Wint clicks;
};

/// button event
///
/// Sent whenever a button on the tool is pressed or released.
///
/// If a button is held down when the tool moves in or out of proximity,
/// button events are generated by the compositor. See
/// wp_tablet_tool.proximity_in and wp_tablet_tool.proximity_out for
/// details.
struct zwp_tablet_tool_v2::event::button {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 17 };
    Wuint serial;
    /// The button whose state has changed
    Wuint button;
    /// Whether the button was pressed or released
    zwp_tablet_tool_v2::button_state state;
};

/// frame event
///
/// Marks the end of a series of axis and/or button updates from the
/// tablet. The Wayland protocol requires axis updates to be sent
/// sequentially, however all events within a frame should be considered
/// one hardware event.
struct zwp_tablet_tool_v2::event::frame {
    static constexpr Wopcode<zwp_tablet_tool_v2> opcode{ 18 };
    /// The time of the event with millisecond granularity
    Wuint time;
};

/// destroy the tablet object
///
/// This destroys the client's resource for this tablet object.
struct zwp_tablet_v2::request::destroy {
    static constexpr Wopcode<zwp_tablet_v2> opcode{ 0 };
};

/// tablet device name
///
/// A descriptive name for the tablet device.
///
/// If the device has no descriptive name, this event is not sent.
///
/// This event is sent in the initial burst of events before the
/// wp_tablet.done event.
struct zwp_tablet_v2::event::name {
    static constexpr Wopcode<zwp_tablet_v2> opcode{ 0 };
    /// the device name
    Wstring name;
};

/// tablet device USB vendor/product id
///
/// The USB vendor and product IDs for the tablet device.
///
/// If the device has no USB vendor/product ID, this event is not sent.
/// This can happen for virtual devices or non-USB devices, for instance.
///
/// This event is sent in the initial burst of events before the
/// wp_tablet.done event.
struct zwp_tablet_v2::event::id {
    static constexpr Wopcode<zwp_tablet_v2> opcode{ 1 };
    /// USB vendor id
    Wuint vid;
    /// USB product id
    Wuint pid;
};

/// path to the device
///
/// A system-specific device path that indicates which device is behind
/// this wp_tablet. This information may be used to gather additional
/// information about the device, e.g. through libwacom.
///
/// A device may have more than one device path. If so, multiple
/// wp_tablet.path events are sent. A device may be emulated and not
/// have a device path, and in that case this event will not be sent.
///
/// The format of the path is unspecified, it may be a device node, a
/// sysfs path, or some other identifier. It is up to the client to
/// identify the string provided.
///
/// This event is sent in the initial burst of events before the
/// wp_tablet.done event.
struct zwp_tablet_v2::event::path {
    static constexpr Wopcode<zwp_tablet_v2> opcode{ 2 };
    /// path to local device
    Wstring path;
};

/// tablet description events sequence complete
///
/// This event is sent immediately to signal the end of the initial
/// burst of descriptive events. A client may consider the static
/// description of the tablet to be complete and finalize initialization
/// of the tablet.
struct zwp_tablet_v2::event::done {
    static constexpr Wopcode<zwp_tablet_v2> opcode{ 3 };
};

/// tablet removed event
///
/// Sent when the tablet has been removed from the system. When a tablet
/// is removed, some tools may be removed.
///
/// When this event is received, the client must wp_tablet.destroy
/// the object.
struct zwp_tablet_v2::event::removed {
    static constexpr Wopcode<zwp_tablet_v2> opcode{ 4 };
};

/// ring axis source
///
/// Describes the source types for ring events. This indicates to the
/// client how a ring event was physically generated; a client may
/// adjust the user interface accordingly. For example, events
/// from a "finger" source may trigger kinetic scrolling.
enum class zwp_tablet_pad_ring_v2::source : Wint::integral_type {
    /// finger
    Efinger = 1
};

/// set compositor feedback
///
/// Request that the compositor use the provided feedback string
/// associated with this ring. This request should be issued immediately
/// after a wp_tablet_pad_group.mode_switch event from the corresponding
/// group is received, or whenever the ring is mapped to a different
/// action. See wp_tablet_pad_group.mode_switch for more details.
///
/// Clients are encouraged to provide context-aware descriptions for
/// the actions associated with the ring; compositors may use this
/// information to offer visual feedback about the button layout
/// (eg. on-screen displays).
///
/// The provided string 'description' is a UTF-8 encoded string to be
/// associated with this ring, and is considered user-visible; general
/// internationalization rules apply.
///
/// The serial argument will be that of the last
/// wp_tablet_pad_group.mode_switch event received for the group of this
/// ring. Requests providing other serials than the most recent one will be
/// ignored.
struct zwp_tablet_pad_ring_v2::request::set_feedback {
    static constexpr Wopcode<zwp_tablet_pad_ring_v2> opcode{ 0 };
    /// ring description
    Wstring description;
    /// serial of the mode switch event
    Wuint serial;
};

/// destroy the ring object
///
/// This destroys the client's resource for this ring object.
struct zwp_tablet_pad_ring_v2::request::destroy {
    static constexpr Wopcode<zwp_tablet_pad_ring_v2> opcode{ 1 };
};

/// ring event source
///
/// Source information for ring events.
///
/// This event does not occur on its own. It is sent before a
/// wp_tablet_pad_ring.frame event and carries the source information
/// for all events within that frame.
///
/// The source specifies how this event was generated. If the source is
/// wp_tablet_pad_ring.source.finger, a wp_tablet_pad_ring.stop event
/// will be sent when the user lifts the finger off the device.
///
/// This event is optional. If the source is unknown for an interaction,
/// no event is sent.
struct zwp_tablet_pad_ring_v2::event::source {
    static constexpr Wopcode<zwp_tablet_pad_ring_v2> opcode{ 0 };
    /// the event source
    zwp_tablet_pad_ring_v2::source source;
};

/// angle changed
///
/// Sent whenever the angle on a ring changes.
///
/// The angle is provided in degrees clockwise from the logical
/// north of the ring in the pad's current rotation.
struct zwp_tablet_pad_ring_v2::event::angle {
    static constexpr Wopcode<zwp_tablet_pad_ring_v2> opcode{ 1 };
    /// the current angle in degrees
    Wfixed degrees;
};

/// interaction stopped
///
/// Stop notification for ring events.
///
/// For some wp_tablet_pad_ring.source types, a wp_tablet_pad_ring.stop
/// event is sent to notify a client that the interaction with the ring
/// has terminated. This enables the client to implement kinetic scrolling.
/// See the wp_tablet_pad_ring.source documentation for information on
/// when this event may be generated.
///
/// Any wp_tablet_pad_ring.angle events with the same source after this
/// event should be considered as the start of a new interaction.
struct zwp_tablet_pad_ring_v2::event::stop {
    static constexpr Wopcode<zwp_tablet_pad_ring_v2> opcode{ 2 };
};

/// end of a ring event sequence
///
/// Indicates the end of a set of ring events that logically belong
/// together. A client is expected to accumulate the data in all events
/// within the frame before proceeding.
///
/// All wp_tablet_pad_ring events before a wp_tablet_pad_ring.frame event belong
/// logically together. For example, on termination of a finger interaction
/// on a ring the compositor will send a wp_tablet_pad_ring.source event,
/// a wp_tablet_pad_ring.stop event and a wp_tablet_pad_ring.frame event.
///
/// A wp_tablet_pad_ring.frame event is sent for every logical event
/// group, even if the group only contains a single wp_tablet_pad_ring
/// event. Specifically, a client may get a sequence: angle, frame,
/// angle, frame, etc.
struct zwp_tablet_pad_ring_v2::event::frame {
    static constexpr Wopcode<zwp_tablet_pad_ring_v2> opcode{ 3 };
    /// timestamp with millisecond granularity
    Wuint time;
};

/// strip axis source
///
/// Describes the source types for strip events. This indicates to the
/// client how a strip event was physically generated; a client may
/// adjust the user interface accordingly. For example, events
/// from a "finger" source may trigger kinetic scrolling.
enum class zwp_tablet_pad_strip_v2::source : Wint::integral_type {
    /// finger
    Efinger = 1
};

/// set compositor feedback
///
/// Requests the compositor to use the provided feedback string
/// associated with this strip. This request should be issued immediately
/// after a wp_tablet_pad_group.mode_switch event from the corresponding
/// group is received, or whenever the strip is mapped to a different
/// action. See wp_tablet_pad_group.mode_switch for more details.
///
/// Clients are encouraged to provide context-aware descriptions for
/// the actions associated with the strip, and compositors may use this
/// information to offer visual feedback about the button layout
/// (eg. on-screen displays).
///
/// The provided string 'description' is a UTF-8 encoded string to be
/// associated with this ring, and is considered user-visible; general
/// internationalization rules apply.
///
/// The serial argument will be that of the last
/// wp_tablet_pad_group.mode_switch event received for the group of this
/// strip. Requests providing other serials than the most recent one will be
/// ignored.
struct zwp_tablet_pad_strip_v2::request::set_feedback {
    static constexpr Wopcode<zwp_tablet_pad_strip_v2> opcode{ 0 };
    /// strip description
    Wstring description;
    /// serial of the mode switch event
    Wuint serial;
};

/// destroy the strip object
///
/// This destroys the client's resource for this strip object.
struct zwp_tablet_pad_strip_v2::request::destroy {
    static constexpr Wopcode<zwp_tablet_pad_strip_v2> opcode{ 1 };
};

/// strip event source
///
/// Source information for strip events.
///
/// This event does not occur on its own. It is sent before a
/// wp_tablet_pad_strip.frame event and carries the source information
/// for all events within that frame.
///
/// The source specifies how this event was generated. If the source is
/// wp_tablet_pad_strip.source.finger, a wp_tablet_pad_strip.stop event
/// will be sent when the user lifts their finger off the device.
///
/// This event is optional. If the source is unknown for an interaction,
/// no event is sent.
struct zwp_tablet_pad_strip_v2::event::source {
    static constexpr Wopcode<zwp_tablet_pad_strip_v2> opcode{ 0 };
    /// the event source
    zwp_tablet_pad_strip_v2::source source;
};

/// position changed
///
/// Sent whenever the position on a strip changes.
///
/// The position is normalized to a range of [0, 65535], the 0-value
/// represents the top-most and/or left-most position of the strip in
/// the pad's current rotation.
struct zwp_tablet_pad_strip_v2::event::position {
    static constexpr Wopcode<zwp_tablet_pad_strip_v2> opcode{ 1 };
    /// the current position
    Wuint position;
};

/// interaction stopped
///
/// Stop notification for strip events.
///
/// For some wp_tablet_pad_strip.source types, a wp_tablet_pad_strip.stop
/// event is sent to notify a client that the interaction with the strip
/// has terminated. This enables the client to implement kinetic
/// scrolling. See the wp_tablet_pad_strip.source documentation for
/// information on when this event may be generated.
///
/// Any wp_tablet_pad_strip.position events with the same source after this
/// event should be considered as the start of a new interaction.
struct zwp_tablet_pad_strip_v2::event::stop {
    static constexpr Wopcode<zwp_tablet_pad_strip_v2> opcode{ 2 };
};

/// end of a strip event sequence
///
/// Indicates the end of a set of events that represent one logical
/// hardware strip event. A client is expected to accumulate the data
/// in all events within the frame before proceeding.
///
/// All wp_tablet_pad_strip events before a wp_tablet_pad_strip.frame event belong
/// logically together. For example, on termination of a finger interaction
/// on a strip the compositor will send a wp_tablet_pad_strip.source event,
/// a wp_tablet_pad_strip.stop event and a wp_tablet_pad_strip.frame
/// event.
///
/// A wp_tablet_pad_strip.frame event is sent for every logical event
/// group, even if the group only contains a single wp_tablet_pad_strip
/// event. Specifically, a client may get a sequence: position, frame,
/// position, frame, etc.
struct zwp_tablet_pad_strip_v2::event::frame {
    static constexpr Wopcode<zwp_tablet_pad_strip_v2> opcode{ 3 };
    /// timestamp with millisecond granularity
    Wuint time;
};

/// destroy the pad object
///
/// Destroy the wp_tablet_pad_group object. Objects created from this object
/// are unaffected and should be destroyed separately.
struct zwp_tablet_pad_group_v2::request::destroy {
    static constexpr Wopcode<zwp_tablet_pad_group_v2> opcode{ 0 };
};

/// buttons announced
///
/// Sent on wp_tablet_pad_group initialization to announce the available
/// buttons in the group. Button indices start at 0, a button may only be
/// in one group at a time.
///
/// This event is first sent in the initial burst of events before the
/// wp_tablet_pad_group.done event.
///
/// Some buttons are reserved by the compositor. These buttons may not be
/// assigned to any wp_tablet_pad_group. Compositors may broadcast this
/// event in the case of changes to the mapping of these reserved buttons.
/// If the compositor happens to reserve all buttons in a group, this event
/// will be sent with an empty array.
struct zwp_tablet_pad_group_v2::event::buttons {
    static constexpr Wopcode<zwp_tablet_pad_group_v2> opcode{ 0 };
    /// buttons in this group
    Warray buttons;
};

/// ring announced
///
/// Sent on wp_tablet_pad_group initialization to announce available rings.
/// One event is sent for each ring available on this pad group.
///
/// This event is sent in the initial burst of events before the
/// wp_tablet_pad_group.done event.
struct zwp_tablet_pad_group_v2::event::ring {
    static constexpr Wopcode<zwp_tablet_pad_group_v2> opcode{ 1 };
    Wnew_id<zwp_tablet_pad_ring_v2> ring;
};

/// strip announced
///
/// Sent on wp_tablet_pad initialization to announce available strips.
/// One event is sent for each strip available on this pad group.
///
/// This event is sent in the initial burst of events before the
/// wp_tablet_pad_group.done event.
struct zwp_tablet_pad_group_v2::event::strip {
    static constexpr Wopcode<zwp_tablet_pad_group_v2> opcode{ 2 };
    Wnew_id<zwp_tablet_pad_strip_v2> strip;
};

/// mode-switch ability announced
///
/// Sent on wp_tablet_pad_group initialization to announce that the pad
/// group may switch between modes. A client may use a mode to store a
/// specific configuration for buttons, rings and strips and use the
/// wl_tablet_pad_group.mode_switch event to toggle between these
/// configurations. Mode indices start at 0.
///
/// Switching modes is compositor-dependent. See the
/// wp_tablet_pad_group.mode_switch event for more details.
///
/// This event is sent in the initial burst of events before the
/// wp_tablet_pad_group.done event. This event is only sent when more than
/// more than one mode is available.
struct zwp_tablet_pad_group_v2::event::modes {
    static constexpr Wopcode<zwp_tablet_pad_group_v2> opcode{ 3 };
    /// the number of modes
    Wuint modes;
};

/// tablet group description events sequence complete
///
/// This event is sent immediately to signal the end of the initial
/// burst of descriptive events. A client may consider the static
/// description of the tablet to be complete and finalize initialization
/// of the tablet group.
struct zwp_tablet_pad_group_v2::event::done {
    static constexpr Wopcode<zwp_tablet_pad_group_v2> opcode{ 4 };
};

/// mode switch event
///
/// Notification that the mode was switched.
///
/// A mode applies to all buttons, rings and strips in a group
/// simultaneously, but a client is not required to assign different actions
/// for each mode. For example, a client may have mode-specific button
/// mappings but map the ring to vertical scrolling in all modes. Mode
/// indices start at 0.
///
/// Switching modes is compositor-dependent. The compositor may provide
/// visual cues to the user about the mode, e.g. by toggling LEDs on
/// the tablet device. Mode-switching may be software-controlled or
/// controlled by one or more physical buttons. For example, on a Wacom
/// Intuos Pro, the button inside the ring may be assigned to switch
/// between modes.
///
/// The compositor will also send this event after wp_tablet_pad.enter on
/// each group in order to notify of the current mode. Groups that only
/// feature one mode will use mode=0 when emitting this event.
///
/// If a button action in the new mode differs from the action in the
/// previous mode, the client should immediately issue a
/// wp_tablet_pad.set_feedback request for each changed button.
///
/// If a ring or strip action in the new mode differs from the action
/// in the previous mode, the client should immediately issue a
/// wp_tablet_ring.set_feedback or wp_tablet_strip.set_feedback request
/// for each changed ring or strip.
struct zwp_tablet_pad_group_v2::event::mode_switch {
    static constexpr Wopcode<zwp_tablet_pad_group_v2> opcode{ 5 };
    /// the time of the event with millisecond granularity
    Wuint time;
    Wuint serial;
    /// the new mode of the pad
    Wuint mode;
};

/// physical button state
///
/// Describes the physical state of a button that caused the button
/// event.
enum class zwp_tablet_pad_v2::button_state : Wint::integral_type {
    /// the button is not pressed
    Ereleased = 0,
    /// the button is pressed
    Epressed = 1
};

/// set compositor feedback
///
/// Requests the compositor to use the provided feedback string
/// associated with this button. This request should be issued immediately
/// after a wp_tablet_pad_group.mode_switch event from the corresponding
/// group is received, or whenever a button is mapped to a different
/// action. See wp_tablet_pad_group.mode_switch for more details.
///
/// Clients are encouraged to provide context-aware descriptions for
/// the actions associated with each button, and compositors may use
/// this information to offer visual feedback on the button layout
/// (e.g. on-screen displays).
///
/// Button indices start at 0. Setting the feedback string on a button
/// that is reserved by the compositor (i.e. not belonging to any
/// wp_tablet_pad_group) does not generate an error but the compositor
/// is free to ignore the request.
///
/// The provided string 'description' is a UTF-8 encoded string to be
/// associated with this ring, and is considered user-visible; general
/// internationalization rules apply.
///
/// The serial argument will be that of the last
/// wp_tablet_pad_group.mode_switch event received for the group of this
/// button. Requests providing other serials than the most recent one will
/// be ignored.
struct zwp_tablet_pad_v2::request::set_feedback {
    static constexpr Wopcode<zwp_tablet_pad_v2> opcode{ 0 };
    /// button index
    Wuint button;
    /// button description
    Wstring description;
    /// serial of the mode switch event
    Wuint serial;
};

/// destroy the pad object
///
/// Destroy the wp_tablet_pad object. Objects created from this object
/// are unaffected and should be destroyed separately.
struct zwp_tablet_pad_v2::request::destroy {
    static constexpr Wopcode<zwp_tablet_pad_v2> opcode{ 1 };
};

/// group announced
///
/// Sent on wp_tablet_pad initialization to announce available groups.
/// One event is sent for each pad group available.
///
/// This event is sent in the initial burst of events before the
/// wp_tablet_pad.done event. At least one group will be announced.
struct zwp_tablet_pad_v2::event::group {
    static constexpr Wopcode<zwp_tablet_pad_v2> opcode{ 0 };
    Wnew_id<zwp_tablet_pad_group_v2> pad_group;
};

/// path to the device
///
/// A system-specific device path that indicates which device is behind
/// this wp_tablet_pad. This information may be used to gather additional
/// information about the device, e.g. through libwacom.
///
/// The format of the path is unspecified, it may be a device node, a
/// sysfs path, or some other identifier. It is up to the client to
/// identify the string provided.
///
/// This event is sent in the initial burst of events before the
/// wp_tablet_pad.done event.
struct zwp_tablet_pad_v2::event::path {
    static constexpr Wopcode<zwp_tablet_pad_v2> opcode{ 1 };
    /// path to local device
    Wstring path;
};

/// buttons announced
///
/// Sent on wp_tablet_pad initialization to announce the available
/// buttons.
///
/// This event is sent in the initial burst of events before the
/// wp_tablet_pad.done event. This event is only sent when at least one
/// button is available.
struct zwp_tablet_pad_v2::event::buttons {
    static constexpr Wopcode<zwp_tablet_pad_v2> opcode{ 2 };
    /// the number of buttons
    Wuint buttons;
};

/// pad description event sequence complete
///
/// This event signals the end of the initial burst of descriptive
/// events. A client may consider the static description of the pad to
/// be complete and finalize initialization of the pad.
struct zwp_tablet_pad_v2::event::done {
    static constexpr Wopcode<zwp_tablet_pad_v2> opcode{ 3 };
};

/// physical button state
///
/// Sent whenever the physical state of a button changes.
struct zwp_tablet_pad_v2::event::button {
    static constexpr Wopcode<zwp_tablet_pad_v2> opcode{ 4 };
    /// the time of the event with millisecond granularity
    Wuint time;
    /// the index of the button that changed state
    Wuint button;
    zwp_tablet_pad_v2::button_state state;
};

/// enter event
///
/// Notification that this pad is focused on the specified surface.
struct zwp_tablet_pad_v2::event::enter {
    static constexpr Wopcode<zwp_tablet_pad_v2> opcode{ 5 };
    /// serial number of the enter event
    Wuint serial;
    /// the tablet the pad is attached to
    Wobject<zwp_tablet_v2> tablet;
    /// surface the pad is focused on
    Wobject<wl_surface> surface;
};

/// leave event
///
/// Notification that this pad is no longer focused on the specified
/// surface.
struct zwp_tablet_pad_v2::event::leave {
    static constexpr Wopcode<zwp_tablet_pad_v2> opcode{ 6 };
    /// serial number of the leave event
    Wuint serial;
    /// surface the pad is no longer focused on
    Wobject<wl_surface> surface;
};

/// pad removed event
///
/// Sent when the pad has been removed from the system. When a tablet
/// is removed its pad(s) will be removed too.
///
/// When this event is received, the client must destroy all rings, strips
/// and groups that were offered by this pad, and issue wp_tablet_pad.destroy
/// the pad itself.
struct zwp_tablet_pad_v2::event::removed {
    static constexpr Wopcode<zwp_tablet_pad_v2> opcode{ 7 };
};

} // namespace protocols
} // namespace wl
} // namespace ger
