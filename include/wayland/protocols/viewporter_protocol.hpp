// Generated from Wayland xml protocol: viewporter

/// @file viewporter xml protocol

/// Copyright © 2013-2016 Collabora, Ltd.
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

#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {
namespace protocols {

/// Declare everything before they might be used.

struct wp_viewporter {
    enum class error : Wint::integral_type;

    struct request {
        struct destroy;
        struct get_viewport;
    };

    struct event {
    };
};
struct wp_viewport {
    enum class error : Wint::integral_type;

    struct request {
        struct destroy;
        struct set_source;
        struct set_destination;
    };

    struct event {
    };
};
enum class wp_viewporter::error : Wint::integral_type {
    /// the surface already has a viewport object associated
    Eviewport_exists = 0
};

/// unbind from the cropping and scaling interface
///
/// Informs the server that the client will not be using this
/// protocol object anymore. This does not affect any other objects,
/// wp_viewport objects included.
struct wp_viewporter::request::destroy {
    static constexpr Wopcode<wp_viewporter> opcode{ 0 };
};

/// extend surface interface for crop and scale
///
/// Instantiate an interface extension for the given wl_surface to
/// crop and scale its content. If the given wl_surface already has
/// a wp_viewport object associated, the viewport_exists
/// protocol error is raised.
struct wp_viewporter::request::get_viewport {
    static constexpr Wopcode<wp_viewporter> opcode{ 1 };
    /// the new viewport interface id
    Wnew_id<wp_viewport> id;
    /// the surface
    Wobject<wl_surface> surface;
};

enum class wp_viewport::error : Wint::integral_type {
    /// negative or zero values in width or height
    Ebad_value = 0,
    /// destination size is not integer
    Ebad_size = 1,
    /// source rectangle extends outside of the content area
    Eout_of_buffer = 2,
    /// the wl_surface was destroyed
    Eno_surface = 3
};

/// remove scaling and cropping from the surface
///
/// The associated wl_surface's crop and scale state is removed.
/// The change is applied on the next wl_surface.commit.
struct wp_viewport::request::destroy {
    static constexpr Wopcode<wp_viewport> opcode{ 0 };
};

/// set the source rectangle for cropping
///
/// Set the source rectangle of the associated wl_surface. See
/// wp_viewport for the description, and relation to the wl_buffer
/// size.
///
/// If all of x, y, width and height are -1.0, the source rectangle is
/// unset instead. Any other set of values where width or height are zero
/// or negative, or x or y are negative, raise the bad_value protocol
/// error.
///
/// The crop and scale state is double-buffered, see wl_surface.commit.
struct wp_viewport::request::set_source {
    static constexpr Wopcode<wp_viewport> opcode{ 1 };
    /// source rectangle x
    Wfixed x;
    /// source rectangle y
    Wfixed y;
    /// source rectangle width
    Wfixed width;
    /// source rectangle height
    Wfixed height;
};

/// set the surface size for scaling
///
/// Set the destination size of the associated wl_surface. See
/// wp_viewport for the description, and relation to the wl_buffer
/// size.
///
/// If width is -1 and height is -1, the destination size is unset
/// instead. Any other pair of values for width and height that
/// contains zero or negative values raises the bad_value protocol
/// error.
///
/// The crop and scale state is double-buffered, see wl_surface.commit.
struct wp_viewport::request::set_destination {
    static constexpr Wopcode<wp_viewport> opcode{ 2 };
    /// surface width
    Wint width;
    /// surface height
    Wint height;
};

} // namespace protocols
} // namespace wl
} // namespace ger
