// Generated from Wayland xml protocol: xdg_shell

/// @file
/// xdg_shell xml protocol
///
/// Copyright © 2008-2013 Kristian Høgsberg
/// Copyright © 2013      Rafael Antognolli
/// Copyright © 2013      Jasper St. Pierre
/// Copyright © 2010-2013 Intel Corporation
/// Copyright © 2015-2017 Samsung Electronics Co., Ltd
/// Copyright © 2015-2017 Red Hat Inc.
///
/// Permission is hereby granted, free of charge, to any person obtaining a
/// copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense,
/// and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice (including the next
/// paragraph) shall be included in all copies or substantial portions of the
/// Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.

#pragma once

#include "guilander/wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {
namespace protocols {

/// Declare everything before they might be used.

struct xdg_wm_base {
    enum class error : Wint::integral_type;

    struct request {
        struct destroy;
        struct create_positioner;
        struct get_xdg_surface;
        struct pong;
    };

    struct event {
        struct ping;
    };
};
struct xdg_positioner {
    enum class error : Wint::integral_type;
    enum class anchor : Wint::integral_type;
    enum class gravity : Wint::integral_type;
    enum class constraint_adjustment : Wuint::integral_type;

    struct request {
        struct destroy;
        struct set_size;
        struct set_anchor_rect;
        struct set_anchor;
        struct set_gravity;
        struct set_constraint_adjustment;
        struct set_offset;
        struct set_reactive;
        struct set_parent_size;
        struct set_parent_configure;
    };

    struct event {
    };
};
struct xdg_surface {
    enum class error : Wint::integral_type;

    struct request {
        struct destroy;
        struct get_toplevel;
        struct get_popup;
        struct set_window_geometry;
        struct ack_configure;
    };

    struct event {
        struct configure;
    };
};
struct xdg_toplevel {
    enum class error : Wint::integral_type;
    enum class resize_edge : Wint::integral_type;
    enum class state : Wint::integral_type;
    enum class wm_capabilities : Wint::integral_type;

    struct request {
        struct destroy;
        struct set_parent;
        struct set_title;
        struct set_app_id;
        struct show_window_menu;
        struct move;
        struct resize;
        struct set_max_size;
        struct set_min_size;
        struct set_maximized;
        struct unset_maximized;
        struct set_fullscreen;
        struct unset_fullscreen;
        struct set_minimized;
    };

    struct event {
        struct configure;
        struct close;
        struct configure_bounds;
        struct wm_capabilities;
    };
};
struct xdg_popup {
    enum class error : Wint::integral_type;

    struct request {
        struct destroy;
        struct grab;
        struct reposition;
    };

    struct event {
        struct configure;
        struct popup_done;
        struct repositioned;
    };
};
enum class xdg_wm_base::error : Wint::integral_type {
    /// given wl_surface has another role
    Erole = 0,
    /// xdg_wm_base was destroyed before children
    Edefunct_surfaces = 1,
    /// the client tried to map or destroy a non-topmost popup
    Enot_the_topmost_popup = 2,
    /// the client specified an invalid popup parent surface
    Einvalid_popup_parent = 3,
    /// the client provided an invalid surface state
    Einvalid_surface_state = 4,
    /// the client provided an invalid positioner
    Einvalid_positioner = 5,
    /// the client didn’t respond to a ping event in time
    Eunresponsive = 6
};

/// destroy xdg_wm_base
///
/// Destroy this xdg_wm_base object.
///
/// Destroying a bound xdg_wm_base object while there are surfaces
/// still alive created by this xdg_wm_base object instance is illegal
/// and will result in a defunct_surfaces error.
struct xdg_wm_base::request::destroy {
    static constexpr Wopcode<xdg_wm_base> opcode{ 0 };
};

/// create a positioner object
///
/// Create a positioner object. A positioner object is used to position
/// surfaces relative to some parent surface. See the interface description
/// and xdg_surface.get_popup for details.
struct xdg_wm_base::request::create_positioner {
    static constexpr Wopcode<xdg_wm_base> opcode{ 1 };
    Wnew_id<xdg_positioner> id;
};

/// create a shell surface from a surface
///
/// This creates an xdg_surface for the given surface. While xdg_surface
/// itself is not a role, the corresponding surface may only be assigned
/// a role extending xdg_surface, such as xdg_toplevel or xdg_popup. It is
/// illegal to create an xdg_surface for a wl_surface which already has an
/// assigned role and this will result in a role error.
///
/// This creates an xdg_surface for the given surface. An xdg_surface is
/// used as basis to define a role to a given surface, such as xdg_toplevel
/// or xdg_popup. It also manages functionality shared between xdg_surface
/// based surface roles.
///
/// See the documentation of xdg_surface for more details about what an
/// xdg_surface is and how it is used.
struct xdg_wm_base::request::get_xdg_surface {
    static constexpr Wopcode<xdg_wm_base> opcode{ 2 };
    Wnew_id<xdg_surface> id;
    Wobject<wl_surface> surface;
};

/// respond to a ping event
///
/// A client must respond to a ping event with a pong request or
/// the client may be deemed unresponsive. See xdg_wm_base.ping
/// and xdg_wm_base.error.unresponsive.
struct xdg_wm_base::request::pong {
    static constexpr Wopcode<xdg_wm_base> opcode{ 3 };
    /// serial of the ping event
    Wuint serial;
};

/// check if the client is alive
///
/// The ping event asks the client if it's still alive. Pass the
/// serial specified in the event back to the compositor by sending
/// a "pong" request back with the specified serial. See xdg_wm_base.pong.
///
/// Compositors can use this to determine if the client is still
/// alive. It's unspecified what will happen if the client doesn't
/// respond to the ping request, or in what timeframe. Clients should
/// try to respond in a reasonable amount of time. The “unresponsive”
/// error is provided for compositors that wish to disconnect unresponsive
/// clients.
///
/// A compositor is free to ping in any way it wants, but a client must
/// always respond to any xdg_wm_base object it created.
struct xdg_wm_base::event::ping {
    static constexpr Wopcode<xdg_wm_base> opcode{ 0 };
    /// pass this to the pong request
    Wuint serial;
};

enum class xdg_positioner::error : Wint::integral_type {
    /// invalid input provided
    Einvalid_input = 0
};

enum class xdg_positioner::anchor : Wint::integral_type {
    Enone = 0,
    Etop = 1,
    Ebottom = 2,
    Eleft = 3,
    Eright = 4,
    Etop_left = 5,
    Ebottom_left = 6,
    Etop_right = 7,
    Ebottom_right = 8
};

enum class xdg_positioner::gravity : Wint::integral_type {
    Enone = 0,
    Etop = 1,
    Ebottom = 2,
    Eleft = 3,
    Eright = 4,
    Etop_left = 5,
    Ebottom_left = 6,
    Etop_right = 7,
    Ebottom_right = 8
};

/// constraint adjustments
///
/// The constraint adjustment value define ways the compositor will adjust
/// the position of the surface, if the unadjusted position would result
/// in the surface being partly constrained.
///
/// Whether a surface is considered 'constrained' is left to the compositor
/// to determine. For example, the surface may be partly outside the
/// compositor's defined 'work area', thus necessitating the child surface's
/// position be adjusted until it is entirely inside the work area.
///
/// The adjustments can be combined, according to a defined precedence: 1)
/// Flip, 2) Slide, 3) Resize.
enum class xdg_positioner::constraint_adjustment : Wuint::integral_type {
    /// don't move the child surface when constrained
    ///
    /// Don't alter the surface position even if it is constrained on some
    /// axis, for example partially outside the edge of an output.

    Enone = 0,
    /// move along the x axis until unconstrained
    ///
    /// Slide the surface along the x axis until it is no longer constrained.
    ///
    /// First try to slide towards the direction of the gravity on the x axis
    /// until either the edge in the opposite direction of the gravity is
    /// unconstrained or the edge in the direction of the gravity is
    /// constrained.
    ///
    /// Then try to slide towards the opposite direction of the gravity on the
    /// x axis until either the edge in the direction of the gravity is
    /// unconstrained or the edge in the opposite direction of the gravity is
    /// constrained.

    Eslide_x = 1,
    /// move along the y axis until unconstrained
    ///
    /// Slide the surface along the y axis until it is no longer constrained.
    ///
    /// First try to slide towards the direction of the gravity on the y axis
    /// until either the edge in the opposite direction of the gravity is
    /// unconstrained or the edge in the direction of the gravity is
    /// constrained.
    ///
    /// Then try to slide towards the opposite direction of the gravity on the
    /// y axis until either the edge in the direction of the gravity is
    /// unconstrained or the edge in the opposite direction of the gravity is
    /// constrained.

    Eslide_y = 2,
    /// invert the anchor and gravity on the x axis
    ///
    /// Invert the anchor and gravity on the x axis if the surface is
    /// constrained on the x axis. For example, if the left edge of the
    /// surface is constrained, the gravity is 'left' and the anchor is
    /// 'left', change the gravity to 'right' and the anchor to 'right'.
    ///
    /// If the adjusted position also ends up being constrained, the resulting
    /// position of the flip_x adjustment will be the one before the
    /// adjustment.

    Eflip_x = 4,
    /// invert the anchor and gravity on the y axis
    ///
    /// Invert the anchor and gravity on the y axis if the surface is
    /// constrained on the y axis. For example, if the bottom edge of the
    /// surface is constrained, the gravity is 'bottom' and the anchor is
    /// 'bottom', change the gravity to 'top' and the anchor to 'top'.
    ///
    /// The adjusted position is calculated given the original anchor
    /// rectangle and offset, but with the new flipped anchor and gravity
    /// values.
    ///
    /// If the adjusted position also ends up being constrained, the resulting
    /// position of the flip_y adjustment will be the one before the
    /// adjustment.

    Eflip_y = 8,
    /// horizontally resize the surface
    ///
    /// Resize the surface horizontally so that it is completely
    /// unconstrained.

    Eresize_x = 16,
    /// vertically resize the surface
    ///
    /// Resize the surface vertically so that it is completely unconstrained.

    Eresize_y = 32
};

/// destroy the xdg_positioner object
///
/// Notify the compositor that the xdg_positioner will no longer be used.
struct xdg_positioner::request::destroy {
    static constexpr Wopcode<xdg_positioner> opcode{ 0 };
};

/// set the size of the to-be positioned rectangle
///
/// Set the size of the surface that is to be positioned with the positioner
/// object. The size is in surface-local coordinates and corresponds to the
/// window geometry. See xdg_surface.set_window_geometry.
///
/// If a zero or negative size is set the invalid_input error is raised.
struct xdg_positioner::request::set_size {
    static constexpr Wopcode<xdg_positioner> opcode{ 1 };
    /// width of positioned rectangle
    Wint width;
    /// height of positioned rectangle
    Wint height;
};

/// set the anchor rectangle within the parent surface
///
/// Specify the anchor rectangle within the parent surface that the child
/// surface will be placed relative to. The rectangle is relative to the
/// window geometry as defined by xdg_surface.set_window_geometry of the
/// parent surface.
///
/// When the xdg_positioner object is used to position a child surface, the
/// anchor rectangle may not extend outside the window geometry of the
/// positioned child's parent surface.
///
/// If a negative size is set the invalid_input error is raised.
struct xdg_positioner::request::set_anchor_rect {
    static constexpr Wopcode<xdg_positioner> opcode{ 2 };
    /// x position of anchor rectangle
    Wint x;
    /// y position of anchor rectangle
    Wint y;
    /// width of anchor rectangle
    Wint width;
    /// height of anchor rectangle
    Wint height;
};

/// set anchor rectangle anchor
///
/// Defines the anchor point for the anchor rectangle. The specified anchor
/// is used derive an anchor point that the child surface will be
/// positioned relative to. If a corner anchor is set (e.g. 'top_left' or
/// 'bottom_right'), the anchor point will be at the specified corner;
/// otherwise, the derived anchor point will be centered on the specified
/// edge, or in the center of the anchor rectangle if no edge is specified.
struct xdg_positioner::request::set_anchor {
    static constexpr Wopcode<xdg_positioner> opcode{ 3 };
    /// anchor
    xdg_positioner::anchor anchor;
};

/// set child surface gravity
///
/// Defines in what direction a surface should be positioned, relative to
/// the anchor point of the parent surface. If a corner gravity is
/// specified (e.g. 'bottom_right' or 'top_left'), then the child surface
/// will be placed towards the specified gravity; otherwise, the child
/// surface will be centered over the anchor point on any axis that had no
/// gravity specified. If the gravity is not in the ‘gravity’ enum, an
/// invalid_input error is raised.
struct xdg_positioner::request::set_gravity {
    static constexpr Wopcode<xdg_positioner> opcode{ 4 };
    /// gravity direction
    xdg_positioner::gravity gravity;
};

/// set the adjustment to be done when constrained
///
/// Specify how the window should be positioned if the originally intended
/// position caused the surface to be constrained, meaning at least
/// partially outside positioning boundaries set by the compositor. The
/// adjustment is set by constructing a bitmask describing the adjustment to
/// be made when the surface is constrained on that axis.
///
/// If no bit for one axis is set, the compositor will assume that the child
/// surface should not change its position on that axis when constrained.
///
/// If more than one bit for one axis is set, the order of how adjustments
/// are applied is specified in the corresponding adjustment descriptions.
///
/// The default adjustment is none.
struct xdg_positioner::request::set_constraint_adjustment {
    static constexpr Wopcode<xdg_positioner> opcode{ 5 };
    /// bit mask of constraint adjustments
    xdg_positioner::constraint_adjustment constraint_adjustment;
};

/// set surface position offset
///
/// Specify the surface position offset relative to the position of the
/// anchor on the anchor rectangle and the anchor on the surface. For
/// example if the anchor of the anchor rectangle is at (x, y), the surface
/// has the gravity bottom|right, and the offset is (ox, oy), the calculated
/// surface position will be (x + ox, y + oy). The offset position of the
/// surface is the one used for constraint testing. See
/// set_constraint_adjustment.
///
/// An example use case is placing a popup menu on top of a user interface
/// element, while aligning the user interface element of the parent surface
/// with some user interface element placed somewhere in the popup surface.
struct xdg_positioner::request::set_offset {
    static constexpr Wopcode<xdg_positioner> opcode{ 6 };
    /// surface position x offset
    Wint x;
    /// surface position y offset
    Wint y;
};

/// continuously reconstrain the surface
///
/// When set reactive, the surface is reconstrained if the conditions used
/// for constraining changed, e.g. the parent window moved.
///
/// If the conditions changed and the popup was reconstrained, an
/// xdg_popup.configure event is sent with updated geometry, followed by an
/// xdg_surface.configure event.
struct xdg_positioner::request::set_reactive {
    static constexpr Wopcode<xdg_positioner> opcode{ 7 };
};

/// 
///
/// Set the parent window geometry the compositor should use when
/// positioning the popup. The compositor may use this information to
/// determine the future state the popup should be constrained using. If
/// this doesn't match the dimension of the parent the popup is eventually
/// positioned against, the behavior is undefined.
///
/// The arguments are given in the surface-local coordinate space.
struct xdg_positioner::request::set_parent_size {
    static constexpr Wopcode<xdg_positioner> opcode{ 8 };
    /// future window geometry width of parent
    Wint parent_width;
    /// future window geometry height of parent
    Wint parent_height;
};

/// set parent configure this is a response to
///
/// Set the serial of an xdg_surface.configure event this positioner will be
/// used in response to. The compositor may use this information together
/// with set_parent_size to determine what future state the popup should be
/// constrained using.
struct xdg_positioner::request::set_parent_configure {
    static constexpr Wopcode<xdg_positioner> opcode{ 9 };
    /// serial of parent configure event
    Wuint serial;
};

enum class xdg_surface::error : Wint::integral_type {
    /// Surface was not fully constructed
    Enot_constructed = 1,
    /// Surface was already constructed
    Ealready_constructed = 2,
    /// Attaching a buffer to an unconfigured surface
    Eunconfigured_buffer = 3,
    /// Invalid serial number when acking a configure event
    Einvalid_serial = 4,
    /// Width or height was zero or negative
    Einvalid_size = 5,
    /// Surface was destroyed before its role object
    Edefunct_role_object = 6
};

/// destroy the xdg_surface
///
/// Destroy the xdg_surface object. An xdg_surface must only be destroyed
/// after its role object has been destroyed, otherwise
/// a defunct_role_object error is raised.
struct xdg_surface::request::destroy {
    static constexpr Wopcode<xdg_surface> opcode{ 0 };
};

/// assign the xdg_toplevel surface role
///
/// This creates an xdg_toplevel object for the given xdg_surface and gives
/// the associated wl_surface the xdg_toplevel role.
///
/// See the documentation of xdg_toplevel for more details about what an
/// xdg_toplevel is and how it is used.
struct xdg_surface::request::get_toplevel {
    static constexpr Wopcode<xdg_surface> opcode{ 1 };
    Wnew_id<xdg_toplevel> id;
};

/// assign the xdg_popup surface role
///
/// This creates an xdg_popup object for the given xdg_surface and gives
/// the associated wl_surface the xdg_popup role.
///
/// If null is passed as a parent, a parent surface must be specified using
/// some other protocol, before committing the initial state.
///
/// See the documentation of xdg_popup for more details about what an
/// xdg_popup is and how it is used.
struct xdg_surface::request::get_popup {
    static constexpr Wopcode<xdg_surface> opcode{ 2 };
    Wnew_id<xdg_popup> id;
    Wobject<xdg_surface> parent;
    Wobject<xdg_positioner> positioner;
};

/// set the new window geometry
///
/// The window geometry of a surface is its "visible bounds" from the
/// user's perspective. Client-side decorations often have invisible
/// portions like drop-shadows which should be ignored for the
/// purposes of aligning, placing and constraining windows.
///
/// The window geometry is double-buffered state, see wl_surface.commit.
///
/// When maintaining a position, the compositor should treat the (x, y)
/// coordinate of the window geometry as the top left corner of the window.
/// A client changing the (x, y) window geometry coordinate should in
/// general not alter the position of the window.
///
/// Once the window geometry of the surface is set, it is not possible to
/// unset it, and it will remain the same until set_window_geometry is
/// called again, even if a new subsurface or buffer is attached.
///
/// If never set, the value is the full bounds of the surface,
/// including any subsurfaces. This updates dynamically on every
/// commit. This unset is meant for extremely simple clients.
///
/// The arguments are given in the surface-local coordinate space of
/// the wl_surface associated with this xdg_surface, and may extend outside
/// of the wl_surface itself to mark parts of the subsurface tree as part of
/// the window geometry.
///
/// When applied, the effective window geometry will be the set window
/// geometry clamped to the bounding rectangle of the combined
/// geometry of the surface of the xdg_surface and the associated
/// subsurfaces.
///
/// The effective geometry will not be recalculated unless a new call to
/// set_window_geometry is done and the new pending surface state is
/// subsequently applied.
///
/// The width and height of the effective window geometry must be
/// greater than zero. Setting an invalid size will raise an
/// invalid_size error.
struct xdg_surface::request::set_window_geometry {
    static constexpr Wopcode<xdg_surface> opcode{ 3 };
    Wint x;
    Wint y;
    Wint width;
    Wint height;
};

/// ack a configure event
///
/// When a configure event is received, if a client commits the
/// surface in response to the configure event, then the client
/// must make an ack_configure request sometime before the commit
/// request, passing along the serial of the configure event.
///
/// For instance, for toplevel surfaces the compositor might use this
/// information to move a surface to the top left only when the client has
/// drawn itself for the maximized or fullscreen state.
///
/// If the client receives multiple configure events before it
/// can respond to one, it only has to ack the last configure event.
/// Acking a configure event that was never sent raises an invalid_serial
/// error.
///
/// A client is not required to commit immediately after sending
/// an ack_configure request - it may even ack_configure several times
/// before its next surface commit.
///
/// A client may send multiple ack_configure requests before committing, but
/// only the last request sent before a commit indicates which configure
/// event the client really is responding to.
///
/// Sending an ack_configure request consumes the serial number sent with
/// the request, as well as serial numbers sent by all configure events
/// sent on this xdg_surface prior to the configure event referenced by
/// the committed serial.
///
/// It is an error to issue multiple ack_configure requests referencing a
/// serial from the same configure event, or to issue an ack_configure
/// request referencing a serial from a configure event issued before the
/// event identified by the last ack_configure request for the same
/// xdg_surface. Doing so will raise an invalid_serial error.
struct xdg_surface::request::ack_configure {
    static constexpr Wopcode<xdg_surface> opcode{ 4 };
    /// the serial from the configure event
    Wuint serial;
};

/// suggest a surface change
///
/// The configure event marks the end of a configure sequence. A configure
/// sequence is a set of one or more events configuring the state of the
/// xdg_surface, including the final xdg_surface.configure event.
///
/// Where applicable, xdg_surface surface roles will during a configure
/// sequence extend this event as a latched state sent as events before the
/// xdg_surface.configure event. Such events should be considered to make up
/// a set of atomically applied configuration states, where the
/// xdg_surface.configure commits the accumulated state.
///
/// Clients should arrange their surface for the new states, and then send
/// an ack_configure request with the serial sent in this configure event at
/// some point before committing the new surface.
///
/// If the client receives multiple configure events before it can respond
/// to one, it is free to discard all but the last event it received.
struct xdg_surface::event::configure {
    static constexpr Wopcode<xdg_surface> opcode{ 0 };
    /// serial of the configure event
    Wuint serial;
};

enum class xdg_toplevel::error : Wint::integral_type {
    /// provided value is         not a valid variant of the resize_edge enum
    Einvalid_resize_edge = 0,
    /// invalid parent toplevel
    Einvalid_parent = 1,
    /// client provided an invalid min or max size
    Einvalid_size = 2
};

/// edge values for resizing
///
/// These values are used to indicate which edge of a surface
/// is being dragged in a resize operation.
enum class xdg_toplevel::resize_edge : Wint::integral_type {
    Enone = 0,
    Etop = 1,
    Ebottom = 2,
    Eleft = 4,
    Etop_left = 5,
    Ebottom_left = 6,
    Eright = 8,
    Etop_right = 9,
    Ebottom_right = 10
};

/// types of state on the surface
///
/// The different state values used on the surface. This is designed for
/// state values like maximized, fullscreen. It is paired with the
/// configure event to ensure that both the client and the compositor
/// setting the state can be synchronized.
///
/// States set in this way are double-buffered, see wl_surface.commit.
enum class xdg_toplevel::state : Wint::integral_type {
    /// the surface is maximized
    ///
    /// The surface is maximized. The window geometry specified in the configure
    /// event must be obeyed by the client, or the xdg_wm_base.invalid_surface_state
    /// error is raised.
    ///
    /// The client should draw without shadow or other
    /// decoration outside of the window geometry.

    Emaximized = 1,
    /// the surface is fullscreen
    ///
    /// The surface is fullscreen. The window geometry specified in the
    /// configure event is a maximum; the client cannot resize beyond it. For
    /// a surface to cover the whole fullscreened area, the geometry
    /// dimensions must be obeyed by the client. For more details, see
    /// xdg_toplevel.set_fullscreen.

    Efullscreen = 2,
    /// the surface is being resized
    ///
    /// The surface is being resized. The window geometry specified in the
    /// configure event is a maximum; the client cannot resize beyond it.
    /// Clients that have aspect ratio or cell sizing configuration can use
    /// a smaller size, however.

    Eresizing = 3,
    /// the surface is now activated
    ///
    /// Client window decorations should be painted as if the window is
    /// active. Do not assume this means that the window actually has
    /// keyboard or pointer focus.

    Eactivated = 4,
    /// the surface’s left edge is tiled
    ///
    /// The window is currently in a tiled layout and the left edge is
    /// considered to be adjacent to another part of the tiling grid.
    ///
    /// The client should draw without shadow or other decoration outside of
    /// the window geometry on the left edge.

    Etiled_left = 5,
    /// the surface’s right edge is tiled
    ///
    /// The window is currently in a tiled layout and the right edge is
    /// considered to be adjacent to another part of the tiling grid.
    ///
    /// The client should draw without shadow or other decoration outside of
    /// the window geometry on the right edge.

    Etiled_right = 6,
    /// the surface’s top edge is tiled
    ///
    /// The window is currently in a tiled layout and the top edge is
    /// considered to be adjacent to another part of the tiling grid.
    ///
    /// The client should draw without shadow or other decoration outside of
    /// the window geometry on the top edge.

    Etiled_top = 7,
    /// the surface’s bottom edge is tiled
    ///
    /// The window is currently in a tiled layout and the bottom edge is
    /// considered to be adjacent to another part of the tiling grid.
    ///
    /// The client should draw without shadow or other decoration outside of
    /// the window geometry on the bottom edge.

    Etiled_bottom = 8,
    /// surface repaint is suspended
    ///
    /// The surface is currently not ordinarily being repainted; for
    /// example because its content is occluded by another window, or its
    /// outputs are switched off due to screen locking.

    Esuspended = 9
};

enum class xdg_toplevel::wm_capabilities : Wint::integral_type {
    /// show_window_menu is available
    Ewindow_menu = 1,
    /// set_maximized and unset_maximized are available
    Emaximize = 2,
    /// set_fullscreen and unset_fullscreen are available
    Efullscreen = 3,
    /// set_minimized is available
    Eminimize = 4
};

/// destroy the xdg_toplevel
///
/// This request destroys the role surface and unmaps the surface;
/// see "Unmapping" behavior in interface section for details.
struct xdg_toplevel::request::destroy {
    static constexpr Wopcode<xdg_toplevel> opcode{ 0 };
};

/// set the parent of this surface
///
/// Set the "parent" of this surface. This surface should be stacked
/// above the parent surface and all other ancestor surfaces.
///
/// Parent surfaces should be set on dialogs, toolboxes, or other
/// "auxiliary" surfaces, so that the parent is raised when the dialog
/// is raised.
///
/// Setting a null parent for a child surface unsets its parent. Setting
/// a null parent for a surface which currently has no parent is a no-op.
///
/// Only mapped surfaces can have child surfaces. Setting a parent which
/// is not mapped is equivalent to setting a null parent. If a surface
/// becomes unmapped, its children's parent is set to the parent of
/// the now-unmapped surface. If the now-unmapped surface has no parent,
/// its children's parent is unset. If the now-unmapped surface becomes
/// mapped again, its parent-child relationship is not restored.
///
/// The parent toplevel must not be one of the child toplevel's
/// descendants, and the parent must be different from the child toplevel,
/// otherwise the invalid_parent protocol error is raised.
struct xdg_toplevel::request::set_parent {
    static constexpr Wopcode<xdg_toplevel> opcode{ 1 };
    Wobject<xdg_toplevel> parent;
};

/// set surface title
///
/// Set a short title for the surface.
///
/// This string may be used to identify the surface in a task bar,
/// window list, or other user interface elements provided by the
/// compositor.
///
/// The string must be encoded in UTF-8.
struct xdg_toplevel::request::set_title {
    static constexpr Wopcode<xdg_toplevel> opcode{ 2 };
    Wstring title;
};

/// set application ID
///
/// Set an application identifier for the surface.
///
/// The app ID identifies the general class of applications to which
/// the surface belongs. The compositor can use this to group multiple
/// surfaces together, or to determine how to launch a new application.
///
/// For D-Bus activatable applications, the app ID is used as the D-Bus
/// service name.
///
/// The compositor shell will try to group application surfaces together
/// by their app ID. As a best practice, it is suggested to select app
/// ID's that match the basename of the application's .desktop file.
/// For example, "org.freedesktop.FooViewer" where the .desktop file is
/// "org.freedesktop.FooViewer.desktop".
///
/// Like other properties, a set_app_id request can be sent after the
/// xdg_toplevel has been mapped to update the property.
///
/// See the desktop-entry specification [0] for more details on
/// application identifiers and how they relate to well-known D-Bus
/// names and .desktop files.
///
/// [0] https://standards.freedesktop.org/desktop-entry-spec/
struct xdg_toplevel::request::set_app_id {
    static constexpr Wopcode<xdg_toplevel> opcode{ 3 };
    Wstring app_id;
};

/// show the window menu
///
/// Clients implementing client-side decorations might want to show
/// a context menu when right-clicking on the decorations, giving the
/// user a menu that they can use to maximize or minimize the window.
///
/// This request asks the compositor to pop up such a window menu at
/// the given position, relative to the local surface coordinates of
/// the parent surface. There are no guarantees as to what menu items
/// the window menu contains, or even if a window menu will be drawn
/// at all.
///
/// This request must be used in response to some sort of user action
/// like a button press, key press, or touch down event.
struct xdg_toplevel::request::show_window_menu {
    static constexpr Wopcode<xdg_toplevel> opcode{ 4 };
    /// the wl_seat of the user event
    Wobject<wl_seat> seat;
    /// the serial of the user event
    Wuint serial;
    /// the x position to pop up the window menu at
    Wint x;
    /// the y position to pop up the window menu at
    Wint y;
};

/// start an interactive move
///
/// Start an interactive, user-driven move of the surface.
///
/// This request must be used in response to some sort of user action
/// like a button press, key press, or touch down event. The passed
/// serial is used to determine the type of interactive move (touch,
/// pointer, etc).
///
/// The server may ignore move requests depending on the state of
/// the surface (e.g. fullscreen or maximized), or if the passed serial
/// is no longer valid.
///
/// If triggered, the surface will lose the focus of the device
/// (wl_pointer, wl_touch, etc) used for the move. It is up to the
/// compositor to visually indicate that the move is taking place, such as
/// updating a pointer cursor, during the move. There is no guarantee
/// that the device focus will return when the move is completed.
struct xdg_toplevel::request::move {
    static constexpr Wopcode<xdg_toplevel> opcode{ 5 };
    /// the wl_seat of the user event
    Wobject<wl_seat> seat;
    /// the serial of the user event
    Wuint serial;
};

/// start an interactive resize
///
/// Start a user-driven, interactive resize of the surface.
///
/// This request must be used in response to some sort of user action
/// like a button press, key press, or touch down event. The passed
/// serial is used to determine the type of interactive resize (touch,
/// pointer, etc).
///
/// The server may ignore resize requests depending on the state of
/// the surface (e.g. fullscreen or maximized).
///
/// If triggered, the client will receive configure events with the
/// "resize" state enum value and the expected sizes. See the "resize"
/// enum value for more details about what is required. The client
/// must also acknowledge configure events using "ack_configure". After
/// the resize is completed, the client will receive another "configure"
/// event without the resize state.
///
/// If triggered, the surface also will lose the focus of the device
/// (wl_pointer, wl_touch, etc) used for the resize. It is up to the
/// compositor to visually indicate that the resize is taking place,
/// such as updating a pointer cursor, during the resize. There is no
/// guarantee that the device focus will return when the resize is
/// completed.
///
/// The edges parameter specifies how the surface should be resized, and
/// is one of the values of the resize_edge enum. Values not matching
/// a variant of the enum will cause the invalid_resize_edge protocol error.
/// The compositor may use this information to update the surface position
/// for example when dragging the top left corner. The compositor may also
/// use this information to adapt its behavior, e.g. choose an appropriate
/// cursor image.
struct xdg_toplevel::request::resize {
    static constexpr Wopcode<xdg_toplevel> opcode{ 6 };
    /// the wl_seat of the user event
    Wobject<wl_seat> seat;
    /// the serial of the user event
    Wuint serial;
    /// which edge or corner is being dragged
    xdg_toplevel::resize_edge edges;
};

/// set the maximum size
///
/// Set a maximum size for the window.
///
/// The client can specify a maximum size so that the compositor does
/// not try to configure the window beyond this size.
///
/// The width and height arguments are in window geometry coordinates.
/// See xdg_surface.set_window_geometry.
///
/// Values set in this way are double-buffered, see wl_surface.commit.
///
/// The compositor can use this information to allow or disallow
/// different states like maximize or fullscreen and draw accurate
/// animations.
///
/// Similarly, a tiling window manager may use this information to
/// place and resize client windows in a more effective way.
///
/// The client should not rely on the compositor to obey the maximum
/// size. The compositor may decide to ignore the values set by the
/// client and request a larger size.
///
/// If never set, or a value of zero in the request, means that the
/// client has no expected maximum size in the given dimension.
/// As a result, a client wishing to reset the maximum size
/// to an unspecified state can use zero for width and height in the
/// request.
///
/// Requesting a maximum size to be smaller than the minimum size of
/// a surface is illegal and will result in an invalid_size error.
///
/// The width and height must be greater than or equal to zero. Using
/// strictly negative values for width or height will result in a
/// invalid_size error.
struct xdg_toplevel::request::set_max_size {
    static constexpr Wopcode<xdg_toplevel> opcode{ 7 };
    Wint width;
    Wint height;
};

/// set the minimum size
///
/// Set a minimum size for the window.
///
/// The client can specify a minimum size so that the compositor does
/// not try to configure the window below this size.
///
/// The width and height arguments are in window geometry coordinates.
/// See xdg_surface.set_window_geometry.
///
/// Values set in this way are double-buffered, see wl_surface.commit.
///
/// The compositor can use this information to allow or disallow
/// different states like maximize or fullscreen and draw accurate
/// animations.
///
/// Similarly, a tiling window manager may use this information to
/// place and resize client windows in a more effective way.
///
/// The client should not rely on the compositor to obey the minimum
/// size. The compositor may decide to ignore the values set by the
/// client and request a smaller size.
///
/// If never set, or a value of zero in the request, means that the
/// client has no expected minimum size in the given dimension.
/// As a result, a client wishing to reset the minimum size
/// to an unspecified state can use zero for width and height in the
/// request.
///
/// Requesting a minimum size to be larger than the maximum size of
/// a surface is illegal and will result in an invalid_size error.
///
/// The width and height must be greater than or equal to zero. Using
/// strictly negative values for width and height will result in a
/// invalid_size error.
struct xdg_toplevel::request::set_min_size {
    static constexpr Wopcode<xdg_toplevel> opcode{ 8 };
    Wint width;
    Wint height;
};

/// maximize the window
///
/// Maximize the surface.
///
/// After requesting that the surface should be maximized, the compositor
/// will respond by emitting a configure event. Whether this configure
/// actually sets the window maximized is subject to compositor policies.
/// The client must then update its content, drawing in the configured
/// state. The client must also acknowledge the configure when committing
/// the new content (see ack_configure).
///
/// It is up to the compositor to decide how and where to maximize the
/// surface, for example which output and what region of the screen should
/// be used.
///
/// If the surface was already maximized, the compositor will still emit
/// a configure event with the "maximized" state.
///
/// If the surface is in a fullscreen state, this request has no direct
/// effect. It may alter the state the surface is returned to when
/// unmaximized unless overridden by the compositor.
struct xdg_toplevel::request::set_maximized {
    static constexpr Wopcode<xdg_toplevel> opcode{ 9 };
};

/// unmaximize the window
///
/// Unmaximize the surface.
///
/// After requesting that the surface should be unmaximized, the compositor
/// will respond by emitting a configure event. Whether this actually
/// un-maximizes the window is subject to compositor policies.
/// If available and applicable, the compositor will include the window
/// geometry dimensions the window had prior to being maximized in the
/// configure event. The client must then update its content, drawing it in
/// the configured state. The client must also acknowledge the configure
/// when committing the new content (see ack_configure).
///
/// It is up to the compositor to position the surface after it was
/// unmaximized; usually the position the surface had before maximizing, if
/// applicable.
///
/// If the surface was already not maximized, the compositor will still
/// emit a configure event without the "maximized" state.
///
/// If the surface is in a fullscreen state, this request has no direct
/// effect. It may alter the state the surface is returned to when
/// unmaximized unless overridden by the compositor.
struct xdg_toplevel::request::unset_maximized {
    static constexpr Wopcode<xdg_toplevel> opcode{ 10 };
};

/// set the window as fullscreen on an output
///
/// Make the surface fullscreen.
///
/// After requesting that the surface should be fullscreened, the
/// compositor will respond by emitting a configure event. Whether the
/// client is actually put into a fullscreen state is subject to compositor
/// policies. The client must also acknowledge the configure when
/// committing the new content (see ack_configure).
///
/// The output passed by the request indicates the client's preference as
/// to which display it should be set fullscreen on. If this value is NULL,
/// it's up to the compositor to choose which display will be used to map
/// this surface.
///
/// If the surface doesn't cover the whole output, the compositor will
/// position the surface in the center of the output and compensate with
/// with border fill covering the rest of the output. The content of the
/// border fill is undefined, but should be assumed to be in some way that
/// attempts to blend into the surrounding area (e.g. solid black).
///
/// If the fullscreened surface is not opaque, the compositor must make
/// sure that other screen content not part of the same surface tree (made
/// up of subsurfaces, popups or similarly coupled surfaces) are not
/// visible below the fullscreened surface.
struct xdg_toplevel::request::set_fullscreen {
    static constexpr Wopcode<xdg_toplevel> opcode{ 11 };
    Wobject<wl_output> output;
};

/// unset the window as fullscreen
///
/// Make the surface no longer fullscreen.
///
/// After requesting that the surface should be unfullscreened, the
/// compositor will respond by emitting a configure event.
/// Whether this actually removes the fullscreen state of the client is
/// subject to compositor policies.
///
/// Making a surface unfullscreen sets states for the surface based on the following:
/// * the state(s) it may have had before becoming fullscreen
/// * any state(s) decided by the compositor
/// * any state(s) requested by the client while the surface was fullscreen
///
/// The compositor may include the previous window geometry dimensions in
/// the configure event, if applicable.
///
/// The client must also acknowledge the configure when committing the new
/// content (see ack_configure).
struct xdg_toplevel::request::unset_fullscreen {
    static constexpr Wopcode<xdg_toplevel> opcode{ 12 };
};

/// set the window as minimized
///
/// Request that the compositor minimize your surface. There is no
/// way to know if the surface is currently minimized, nor is there
/// any way to unset minimization on this surface.
///
/// If you are looking to throttle redrawing when minimized, please
/// instead use the wl_surface.frame event for this, as this will
/// also work with live previews on windows in Alt-Tab, Expose or
/// similar compositor features.
struct xdg_toplevel::request::set_minimized {
    static constexpr Wopcode<xdg_toplevel> opcode{ 13 };
};

/// suggest a surface change
///
/// This configure event asks the client to resize its toplevel surface or
/// to change its state. The configured state should not be applied
/// immediately. See xdg_surface.configure for details.
///
/// The width and height arguments specify a hint to the window
/// about how its surface should be resized in window geometry
/// coordinates. See set_window_geometry.
///
/// If the width or height arguments are zero, it means the client
/// should decide its own window dimension. This may happen when the
/// compositor needs to configure the state of the surface but doesn't
/// have any information about any previous or expected dimension.
///
/// The states listed in the event specify how the width/height
/// arguments should be interpreted, and possibly how it should be
/// drawn.
///
/// Clients must send an ack_configure in response to this event. See
/// xdg_surface.configure and xdg_surface.ack_configure for details.
struct xdg_toplevel::event::configure {
    static constexpr Wopcode<xdg_toplevel> opcode{ 0 };
    Wint width;
    Wint height;
    Warray states;
};

/// surface wants to be closed
///
/// The close event is sent by the compositor when the user
/// wants the surface to be closed. This should be equivalent to
/// the user clicking the close button in client-side decorations,
/// if your application has any.
///
/// This is only a request that the user intends to close the
/// window. The client may choose to ignore this request, or show
/// a dialog to ask the user to save their data, etc.
struct xdg_toplevel::event::close {
    static constexpr Wopcode<xdg_toplevel> opcode{ 1 };
};

/// recommended window geometry bounds
///
/// The configure_bounds event may be sent prior to a xdg_toplevel.configure
/// event to communicate the bounds a window geometry size is recommended
/// to constrain to.
///
/// The passed width and height are in surface coordinate space. If width
/// and height are 0, it means bounds is unknown and equivalent to as if no
/// configure_bounds event was ever sent for this surface.
///
/// The bounds can for example correspond to the size of a monitor excluding
/// any panels or other shell components, so that a surface isn't created in
/// a way that it cannot fit.
///
/// The bounds may change at any point, and in such a case, a new
/// xdg_toplevel.configure_bounds will be sent, followed by
/// xdg_toplevel.configure and xdg_surface.configure.
struct xdg_toplevel::event::configure_bounds {
    static constexpr Wopcode<xdg_toplevel> opcode{ 2 };
    Wint width;
    Wint height;
};

/// compositor capabilities
///
/// This event advertises the capabilities supported by the compositor. If
/// a capability isn't supported, clients should hide or disable the UI
/// elements that expose this functionality. For instance, if the
/// compositor doesn't advertise support for minimized toplevels, a button
/// triggering the set_minimized request should not be displayed.
///
/// The compositor will ignore requests it doesn't support. For instance,
/// a compositor which doesn't advertise support for minimized will ignore
/// set_minimized requests.
///
/// Compositors must send this event once before the first
/// xdg_surface.configure event. When the capabilities change, compositors
/// must send this event again and then send an xdg_surface.configure
/// event.
///
/// The configured state should not be applied immediately. See
/// xdg_surface.configure for details.
///
/// The capabilities are sent as an array of 32-bit unsigned integers in
/// native endianness.
struct xdg_toplevel::event::wm_capabilities {
    static constexpr Wopcode<xdg_toplevel> opcode{ 3 };
    /// array of 32-bit capabilities
    Warray capabilities;
};

enum class xdg_popup::error : Wint::integral_type {
    /// tried to grab after being mapped
    Einvalid_grab = 0
};

/// remove xdg_popup interface
///
/// This destroys the popup. Explicitly destroying the xdg_popup
/// object will also dismiss the popup, and unmap the surface.
///
/// If this xdg_popup is not the "topmost" popup, the
/// xdg_wm_base.not_the_topmost_popup protocol error will be sent.
struct xdg_popup::request::destroy {
    static constexpr Wopcode<xdg_popup> opcode{ 0 };
};

/// make the popup take an explicit grab
///
/// This request makes the created popup take an explicit grab. An explicit
/// grab will be dismissed when the user dismisses the popup, or when the
/// client destroys the xdg_popup. This can be done by the user clicking
/// outside the surface, using the keyboard, or even locking the screen
/// through closing the lid or a timeout.
///
/// If the compositor denies the grab, the popup will be immediately
/// dismissed.
///
/// This request must be used in response to some sort of user action like a
/// button press, key press, or touch down event. The serial number of the
/// event should be passed as 'serial'.
///
/// The parent of a grabbing popup must either be an xdg_toplevel surface or
/// another xdg_popup with an explicit grab. If the parent is another
/// xdg_popup it means that the popups are nested, with this popup now being
/// the topmost popup.
///
/// Nested popups must be destroyed in the reverse order they were created
/// in, e.g. the only popup you are allowed to destroy at all times is the
/// topmost one.
///
/// When compositors choose to dismiss a popup, they may dismiss every
/// nested grabbing popup as well. When a compositor dismisses popups, it
/// will follow the same dismissing order as required from the client.
///
/// If the topmost grabbing popup is destroyed, the grab will be returned to
/// the parent of the popup, if that parent previously had an explicit grab.
///
/// If the parent is a grabbing popup which has already been dismissed, this
/// popup will be immediately dismissed. If the parent is a popup that did
/// not take an explicit grab, an error will be raised.
///
/// During a popup grab, the client owning the grab will receive pointer
/// and touch events for all their surfaces as normal (similar to an
/// "owner-events" grab in X11 parlance), while the top most grabbing popup
/// will always have keyboard focus.
struct xdg_popup::request::grab {
    static constexpr Wopcode<xdg_popup> opcode{ 1 };
    /// the wl_seat of the user event
    Wobject<wl_seat> seat;
    /// the serial of the user event
    Wuint serial;
};

/// recalculate the popup's location
///
/// Reposition an already-mapped popup. The popup will be placed given the
/// details in the passed xdg_positioner object, and a
/// xdg_popup.repositioned followed by xdg_popup.configure and
/// xdg_surface.configure will be emitted in response. Any parameters set
/// by the previous positioner will be discarded.
///
/// The passed token will be sent in the corresponding
/// xdg_popup.repositioned event. The new popup position will not take
/// effect until the corresponding configure event is acknowledged by the
/// client. See xdg_popup.repositioned for details. The token itself is
/// opaque, and has no other special meaning.
///
/// If multiple reposition requests are sent, the compositor may skip all
/// but the last one.
///
/// If the popup is repositioned in response to a configure event for its
/// parent, the client should send an xdg_positioner.set_parent_configure
/// and possibly an xdg_positioner.set_parent_size request to allow the
/// compositor to properly constrain the popup.
///
/// If the popup is repositioned together with a parent that is being
/// resized, but not in response to a configure event, the client should
/// send an xdg_positioner.set_parent_size request.
struct xdg_popup::request::reposition {
    static constexpr Wopcode<xdg_popup> opcode{ 2 };
    Wobject<xdg_positioner> positioner;
    /// reposition request token
    Wuint token;
};

/// configure the popup surface
///
/// This event asks the popup surface to configure itself given the
/// configuration. The configured state should not be applied immediately.
/// See xdg_surface.configure for details.
///
/// The x and y arguments represent the position the popup was placed at
/// given the xdg_positioner rule, relative to the upper left corner of the
/// window geometry of the parent surface.
///
/// For version 2 or older, the configure event for an xdg_popup is only
/// ever sent once for the initial configuration. Starting with version 3,
/// it may be sent again if the popup is setup with an xdg_positioner with
/// set_reactive requested, or in response to xdg_popup.reposition requests.
struct xdg_popup::event::configure {
    static constexpr Wopcode<xdg_popup> opcode{ 0 };
    /// x position relative to parent surface window geometry
    Wint x;
    /// y position relative to parent surface window geometry
    Wint y;
    /// window geometry width
    Wint width;
    /// window geometry height
    Wint height;
};

/// popup interaction is done
///
/// The popup_done event is sent out when a popup is dismissed by the
/// compositor. The client should destroy the xdg_popup object at this
/// point.
struct xdg_popup::event::popup_done {
    static constexpr Wopcode<xdg_popup> opcode{ 1 };
};

/// signal the completion of a repositioned request
///
/// The repositioned event is sent as part of a popup configuration
/// sequence, together with xdg_popup.configure and lastly
/// xdg_surface.configure to notify the completion of a reposition request.
///
/// The repositioned event is to notify about the completion of a
/// xdg_popup.reposition request. The token argument is the token passed
/// in the xdg_popup.reposition request.
///
/// Immediately after this event is emitted, xdg_popup.configure and
/// xdg_surface.configure will be sent with the updated size and position,
/// as well as a new configure serial.
///
/// The client should optionally update the content of the popup, but must
/// acknowledge the new popup configuration for the new position to take
/// effect. See xdg_surface.ack_configure for details.
struct xdg_popup::event::repositioned {
    static constexpr Wopcode<xdg_popup> opcode{ 2 };
    /// reposition request token
    Wuint token;
};

} // namespace protocols
} // namespace wl
} // namespace ger
