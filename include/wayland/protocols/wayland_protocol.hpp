// Generated from Wayland xml protocol: wayland

/// @file
/// wayland xml protocol
///
/// Copyright © 2008-2011 Kristian Høgsberg
/// Copyright © 2010-2011 Intel Corporation
/// Copyright © 2012-2013 Collabora, Ltd.
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

#include "wayland/protocol_primitives.hpp"

namespace ger {
namespace wl {
namespace protocols {

/// Declare everything before they might be used.

struct wl_display {
    enum class error : Wint::integral_type;

    struct request {
        struct sync;
        struct get_registry;
    };

    struct event {
        struct error;
        struct delete_id;
    };
};
struct wl_registry {
    struct request {
        struct bind;
    };

    struct event {
        struct global;
        struct global_remove;
    };
};
struct wl_callback {
    struct request {
    };

    struct event {
        struct done;
    };
};
struct wl_compositor {
    struct request {
        struct create_surface;
        struct create_region;
    };

    struct event {
    };
};
struct wl_shm_pool {
    struct request {
        struct create_buffer;
        struct destroy;
        struct resize;
    };

    struct event {
    };
};
struct wl_shm {
    enum class error : Wint::integral_type;
    enum class format : Wint::integral_type;

    struct request {
        struct create_pool;
        struct release;
    };

    struct event {
        struct format;
    };
};
struct wl_buffer {
    struct request {
        struct destroy;
    };

    struct event {
        struct release;
    };
};
struct wl_data_offer {
    enum class error : Wint::integral_type;

    struct request {
        struct accept;
        struct receive;
        struct destroy;
        struct finish;
        struct set_actions;
    };

    struct event {
        struct offer;
        struct source_actions;
        struct action;
    };
};
struct wl_data_source {
    enum class error : Wint::integral_type;

    struct request {
        struct offer;
        struct destroy;
        struct set_actions;
    };

    struct event {
        struct target;
        struct send;
        struct cancelled;
        struct dnd_drop_performed;
        struct dnd_finished;
        struct action;
    };
};
struct wl_data_device {
    enum class error : Wint::integral_type;

    struct request {
        struct start_drag;
        struct set_selection;
        struct release;
    };

    struct event {
        struct data_offer;
        struct enter;
        struct leave;
        struct motion;
        struct drop;
        struct selection;
    };
};
struct wl_data_device_manager {
    enum class dnd_action : Wuint::integral_type;

    struct request {
        struct create_data_source;
        struct get_data_device;
    };

    struct event {
    };
};
struct wl_shell {
    enum class error : Wint::integral_type;

    struct request {
        struct get_shell_surface;
    };

    struct event {
    };
};
struct wl_shell_surface {
    enum class resize : Wuint::integral_type;
    enum class transient : Wuint::integral_type;
    enum class fullscreen_method : Wint::integral_type;

    struct request {
        struct pong;
        struct move;
        struct resize;
        struct set_toplevel;
        struct set_transient;
        struct set_fullscreen;
        struct set_popup;
        struct set_maximized;
        struct set_title;
        struct set_class;
    };

    struct event {
        struct ping;
        struct configure;
        struct popup_done;
    };
};
struct wl_surface {
    enum class error : Wint::integral_type;

    struct request {
        struct destroy;
        struct attach;
        struct damage;
        struct frame;
        struct set_opaque_region;
        struct set_input_region;
        struct commit;
        struct set_buffer_transform;
        struct set_buffer_scale;
        struct damage_buffer;
        struct offset;
    };

    struct event {
        struct enter;
        struct leave;
        struct preferred_buffer_scale;
        struct preferred_buffer_transform;
    };
};
struct wl_seat {
    enum class capability : Wuint::integral_type;
    enum class error : Wint::integral_type;

    struct request {
        struct get_pointer;
        struct get_keyboard;
        struct get_touch;
        struct release;
    };

    struct event {
        struct capabilities;
        struct name;
    };
};
struct wl_pointer {
    enum class error : Wint::integral_type;
    enum class button_state : Wint::integral_type;
    enum class axis : Wint::integral_type;
    enum class axis_source : Wint::integral_type;
    enum class axis_relative_direction : Wint::integral_type;

    struct request {
        struct set_cursor;
        struct release;
    };

    struct event {
        struct enter;
        struct leave;
        struct motion;
        struct button;
        struct axis;
        struct frame;
        struct axis_source;
        struct axis_stop;
        struct axis_discrete;
        struct axis_value120;
        struct axis_relative_direction;
    };
};
struct wl_keyboard {
    enum class keymap_format : Wint::integral_type;
    enum class key_state : Wint::integral_type;

    struct request {
        struct release;
    };

    struct event {
        struct keymap;
        struct enter;
        struct leave;
        struct key;
        struct modifiers;
        struct repeat_info;
    };
};
struct wl_touch {
    struct request {
        struct release;
    };

    struct event {
        struct down;
        struct up;
        struct motion;
        struct frame;
        struct cancel;
        struct shape;
        struct orientation;
    };
};
struct wl_output {
    enum class subpixel : Wint::integral_type;
    enum class transform : Wint::integral_type;
    enum class mode : Wuint::integral_type;

    struct request {
        struct release;
    };

    struct event {
        struct geometry;
        struct mode;
        struct done;
        struct scale;
        struct name;
        struct description;
    };
};
struct wl_region {
    struct request {
        struct destroy;
        struct add;
        struct subtract;
    };

    struct event {
    };
};
struct wl_subcompositor {
    enum class error : Wint::integral_type;

    struct request {
        struct destroy;
        struct get_subsurface;
    };

    struct event {
    };
};
struct wl_subsurface {
    enum class error : Wint::integral_type;

    struct request {
        struct destroy;
        struct set_position;
        struct place_above;
        struct place_below;
        struct set_sync;
        struct set_desync;
    };

    struct event {
    };
};
struct wl_fixes {
    struct request {
        struct destroy;
        struct destroy_registry;
    };

    struct event {
    };
};
/// global error values
///
/// These errors are global and can be emitted in response to any
/// server request.
enum class wl_display::error : Wint::integral_type {
    /// server couldn't find object
    Einvalid_object = 0,
    /// method doesn't exist on the specified interface or malformed request
    Einvalid_method = 1,
    /// server is out of memory
    Eno_memory = 2,
    /// implementation error in compositor
    Eimplementation = 3
};

/// asynchronous roundtrip
///
/// The sync request asks the server to emit the 'done' event
/// on the returned wl_callback object.  Since requests are
/// handled in-order and events are delivered in-order, this can
/// be used as a barrier to ensure all previous requests and the
/// resulting events have been handled.
///
/// The object returned by this request will be destroyed by the
/// compositor after the callback is fired and as such the client must not
/// attempt to use it after that point.
///
/// The callback_data passed in the callback is undefined and should be ignored.
struct wl_display::request::sync {
    static constexpr Wopcode<wl_display> opcode{ 0 };
    /// callback object for the sync request
    Wnew_id<wl_callback> callback;
};

/// get global registry object
///
/// This request creates a registry object that allows the client
/// to list and bind the global objects available from the
/// compositor.
///
/// It should be noted that the server side resources consumed in
/// response to a get_registry request can only be released when the
/// client disconnects, not when the client side proxy is destroyed.
/// Therefore, clients should invoke get_registry as infrequently as
/// possible to avoid wasting memory.
struct wl_display::request::get_registry {
    static constexpr Wopcode<wl_display> opcode{ 1 };
    /// global registry object
    Wnew_id<wl_registry> registry;
};

/// fatal error event
///
/// The error event is sent out when a fatal (non-recoverable)
/// error has occurred.  The object_id argument is the object
/// where the error occurred, most often in response to a request
/// to that object.  The code identifies the error and is defined
/// by the object interface.  As such, each interface defines its
/// own set of error codes.  The message is a brief description
/// of the error, for (debugging) convenience.
struct wl_display::event::error {
    static constexpr Wopcode<wl_display> opcode{ 0 };
    /// object where the error occurred
    Wobject<> object_id;
    /// error code
    Wuint code;
    /// error description
    Wstring message;
};

/// acknowledge object ID deletion
///
/// This event is used internally by the object ID management
/// logic. When a client deletes an object that it had created,
/// the server will send this event to acknowledge that it has
/// seen the delete request. When the client receives this event,
/// it will know that it can safely reuse the object ID.
struct wl_display::event::delete_id {
    static constexpr Wopcode<wl_display> opcode{ 1 };
    /// deleted object ID
    Wuint id;
};

/// bind an object to the display
///
/// Binds a new, client-created object to the server using the
/// specified name as the identifier.
struct wl_registry::request::bind {
    static constexpr Wopcode<wl_registry> opcode{ 0 };
    /// unique numeric name of the object
    Wuint name;
    /// Interface name, e.g. from registry::global event, for following Wnew_id<>.
    Wstring new_id_interface;
    /// Interface version, e.g. from registry::global event, for following Wnew_id<>.
    Wuint new_id_interface_version;
    /// bounded object
    Wnew_id<> id;
};

/// announce global object
///
/// Notify the client of global objects.
///
/// The event notifies the client that a global object with
/// the given name is now available, and it implements the
/// given version of the given interface.
struct wl_registry::event::global {
    static constexpr Wopcode<wl_registry> opcode{ 0 };
    /// numeric name of the global object
    Wuint name;
    /// interface implemented by the object
    Wstring interface;
    /// interface version
    Wuint version;
};

/// announce removal of global object
///
/// Notify the client of removed global objects.
///
/// This event notifies the client that the global identified
/// by name is no longer available.  If the client bound to
/// the global using the bind request, the client should now
/// destroy that object.
///
/// The object remains valid and requests to the object will be
/// ignored until the client destroys it, to avoid races between
/// the global going away and a client sending a request to it.
struct wl_registry::event::global_remove {
    static constexpr Wopcode<wl_registry> opcode{ 1 };
    /// numeric name of the global object
    Wuint name;
};

/// done event
///
/// Notify the client when the related request is done.
struct wl_callback::event::done {
    static constexpr Wopcode<wl_callback> opcode{ 0 };
    /// request-specific data for the callback
    Wuint callback_data;
};

/// create new surface
///
/// Ask the compositor to create a new surface.
struct wl_compositor::request::create_surface {
    static constexpr Wopcode<wl_compositor> opcode{ 0 };
    /// the new surface
    Wnew_id<wl_surface> id;
};

/// create new region
///
/// Ask the compositor to create a new region.
struct wl_compositor::request::create_region {
    static constexpr Wopcode<wl_compositor> opcode{ 1 };
    /// the new region
    Wnew_id<wl_region> id;
};

/// create a buffer from the pool
///
/// Create a wl_buffer object from the pool.
///
/// The buffer is created offset bytes into the pool and has
/// width and height as specified.  The stride argument specifies
/// the number of bytes from the beginning of one row to the beginning
/// of the next.  The format is the pixel format of the buffer and
/// must be one of those advertised through the wl_shm.format event.
///
/// A buffer will keep a reference to the pool it was created from
/// so it is valid to destroy the pool immediately after creating
/// a buffer from it.
struct wl_shm_pool::request::create_buffer {
    static constexpr Wopcode<wl_shm_pool> opcode{ 0 };
    /// buffer to create
    Wnew_id<wl_buffer> id;
    /// buffer byte offset within the pool
    Wint offset;
    /// buffer width, in pixels
    Wint width;
    /// buffer height, in pixels
    Wint height;
    /// number of bytes from the beginning of one row to the beginning of the next row
    Wint stride;
    /// buffer pixel format
    wl_shm::format format;
};

/// destroy the pool
///
/// Destroy the shared memory pool.
///
/// The mmapped memory will be released when all
/// buffers that have been created from this pool
/// are gone.
struct wl_shm_pool::request::destroy {
    static constexpr Wopcode<wl_shm_pool> opcode{ 1 };
};

/// change the size of the pool mapping
///
/// This request will cause the server to remap the backing memory
/// for the pool from the file descriptor passed when the pool was
/// created, but using the new size.  This request can only be
/// used to make the pool bigger.
///
/// This request only changes the amount of bytes that are mmapped
/// by the server and does not touch the file corresponding to the
/// file descriptor passed at creation time. It is the client's
/// responsibility to ensure that the file is at least as big as
/// the new pool size.
struct wl_shm_pool::request::resize {
    static constexpr Wopcode<wl_shm_pool> opcode{ 2 };
    /// new size of the pool, in bytes
    Wint size;
};

/// wl_shm error values
///
/// These errors can be emitted in response to wl_shm requests.
enum class wl_shm::error : Wint::integral_type {
    /// buffer format is not known
    Einvalid_format = 0,
    /// invalid size or stride during pool or buffer creation
    Einvalid_stride = 1,
    /// mmapping the file descriptor failed
    Einvalid_fd = 2
};

/// pixel formats
///
/// This describes the memory layout of an individual pixel.
///
/// All renderers should support argb8888 and xrgb8888 but any other
/// formats are optional and may not be supported by the particular
/// renderer in use.
///
/// The drm format codes match the macros defined in drm_fourcc.h, except
/// argb8888 and xrgb8888. The formats actually supported by the compositor
/// will be reported by the format event.
///
/// For all wl_shm formats and unless specified in another protocol
/// extension, pre-multiplied alpha is used for pixel values.
enum class wl_shm::format : Wint::integral_type {
    /// 32-bit ARGB format, [31:0] A:R:G:B 8:8:8:8 little endian
    Eargb8888 = 0,
    /// 32-bit RGB format, [31:0] x:R:G:B 8:8:8:8 little endian
    Exrgb8888 = 1,
    /// 8-bit color index format, [7:0] C
    Ec8 = 538982467,
    /// 8-bit RGB format, [7:0] R:G:B 3:3:2
    Ergb332 = 943867730,
    /// 8-bit BGR format, [7:0] B:G:R 2:3:3
    Ebgr233 = 944916290,
    /// 16-bit xRGB format, [15:0] x:R:G:B 4:4:4:4 little endian
    Exrgb4444 = 842093144,
    /// 16-bit xBGR format, [15:0] x:B:G:R 4:4:4:4 little endian
    Exbgr4444 = 842089048,
    /// 16-bit RGBx format, [15:0] R:G:B:x 4:4:4:4 little endian
    Ergbx4444 = 842094674,
    /// 16-bit BGRx format, [15:0] B:G:R:x 4:4:4:4 little endian
    Ebgrx4444 = 842094658,
    /// 16-bit ARGB format, [15:0] A:R:G:B 4:4:4:4 little endian
    Eargb4444 = 842093121,
    /// 16-bit ABGR format, [15:0] A:B:G:R 4:4:4:4 little endian
    Eabgr4444 = 842089025,
    /// 16-bit RBGA format, [15:0] R:G:B:A 4:4:4:4 little endian
    Ergba4444 = 842088786,
    /// 16-bit BGRA format, [15:0] B:G:R:A 4:4:4:4 little endian
    Ebgra4444 = 842088770,
    /// 16-bit xRGB format, [15:0] x:R:G:B 1:5:5:5 little endian
    Exrgb1555 = 892424792,
    /// 16-bit xBGR 1555 format, [15:0] x:B:G:R 1:5:5:5 little endian
    Exbgr1555 = 892420696,
    /// 16-bit RGBx 5551 format, [15:0] R:G:B:x 5:5:5:1 little endian
    Ergbx5551 = 892426322,
    /// 16-bit BGRx 5551 format, [15:0] B:G:R:x 5:5:5:1 little endian
    Ebgrx5551 = 892426306,
    /// 16-bit ARGB 1555 format, [15:0] A:R:G:B 1:5:5:5 little endian
    Eargb1555 = 892424769,
    /// 16-bit ABGR 1555 format, [15:0] A:B:G:R 1:5:5:5 little endian
    Eabgr1555 = 892420673,
    /// 16-bit RGBA 5551 format, [15:0] R:G:B:A 5:5:5:1 little endian
    Ergba5551 = 892420434,
    /// 16-bit BGRA 5551 format, [15:0] B:G:R:A 5:5:5:1 little endian
    Ebgra5551 = 892420418,
    /// 16-bit RGB 565 format, [15:0] R:G:B 5:6:5 little endian
    Ergb565 = 909199186,
    /// 16-bit BGR 565 format, [15:0] B:G:R 5:6:5 little endian
    Ebgr565 = 909199170,
    /// 24-bit RGB format, [23:0] R:G:B little endian
    Ergb888 = 875710290,
    /// 24-bit BGR format, [23:0] B:G:R little endian
    Ebgr888 = 875710274,
    /// 32-bit xBGR format, [31:0] x:B:G:R 8:8:8:8 little endian
    Exbgr8888 = 875709016,
    /// 32-bit RGBx format, [31:0] R:G:B:x 8:8:8:8 little endian
    Ergbx8888 = 875714642,
    /// 32-bit BGRx format, [31:0] B:G:R:x 8:8:8:8 little endian
    Ebgrx8888 = 875714626,
    /// 32-bit ABGR format, [31:0] A:B:G:R 8:8:8:8 little endian
    Eabgr8888 = 875708993,
    /// 32-bit RGBA format, [31:0] R:G:B:A 8:8:8:8 little endian
    Ergba8888 = 875708754,
    /// 32-bit BGRA format, [31:0] B:G:R:A 8:8:8:8 little endian
    Ebgra8888 = 875708738,
    /// 32-bit xRGB format, [31:0] x:R:G:B 2:10:10:10 little endian
    Exrgb2101010 = 808669784,
    /// 32-bit xBGR format, [31:0] x:B:G:R 2:10:10:10 little endian
    Exbgr2101010 = 808665688,
    /// 32-bit RGBx format, [31:0] R:G:B:x 10:10:10:2 little endian
    Ergbx1010102 = 808671314,
    /// 32-bit BGRx format, [31:0] B:G:R:x 10:10:10:2 little endian
    Ebgrx1010102 = 808671298,
    /// 32-bit ARGB format, [31:0] A:R:G:B 2:10:10:10 little endian
    Eargb2101010 = 808669761,
    /// 32-bit ABGR format, [31:0] A:B:G:R 2:10:10:10 little endian
    Eabgr2101010 = 808665665,
    /// 32-bit RGBA format, [31:0] R:G:B:A 10:10:10:2 little endian
    Ergba1010102 = 808665426,
    /// 32-bit BGRA format, [31:0] B:G:R:A 10:10:10:2 little endian
    Ebgra1010102 = 808665410,
    /// packed YCbCr format, [31:0] Cr0:Y1:Cb0:Y0 8:8:8:8 little endian
    Eyuyv = 1448695129,
    /// packed YCbCr format, [31:0] Cb0:Y1:Cr0:Y0 8:8:8:8 little endian
    Eyvyu = 1431918169,
    /// packed YCbCr format, [31:0] Y1:Cr0:Y0:Cb0 8:8:8:8 little endian
    Euyvy = 1498831189,
    /// packed YCbCr format, [31:0] Y1:Cb0:Y0:Cr0 8:8:8:8 little endian
    Evyuy = 1498765654,
    /// packed AYCbCr format, [31:0] A:Y:Cb:Cr 8:8:8:8 little endian
    Eayuv = 1448433985,
    /// 2 plane YCbCr Cr:Cb format, 2x2 subsampled Cr:Cb plane
    Env12 = 842094158,
    /// 2 plane YCbCr Cb:Cr format, 2x2 subsampled Cb:Cr plane
    Env21 = 825382478,
    /// 2 plane YCbCr Cr:Cb format, 2x1 subsampled Cr:Cb plane
    Env16 = 909203022,
    /// 2 plane YCbCr Cb:Cr format, 2x1 subsampled Cb:Cr plane
    Env61 = 825644622,
    /// 3 plane YCbCr format, 4x4 subsampled Cb (1) and Cr (2) planes
    Eyuv410 = 961959257,
    /// 3 plane YCbCr format, 4x4 subsampled Cr (1) and Cb (2) planes
    Eyvu410 = 961893977,
    /// 3 plane YCbCr format, 4x1 subsampled Cb (1) and Cr (2) planes
    Eyuv411 = 825316697,
    /// 3 plane YCbCr format, 4x1 subsampled Cr (1) and Cb (2) planes
    Eyvu411 = 825316953,
    /// 3 plane YCbCr format, 2x2 subsampled Cb (1) and Cr (2) planes
    Eyuv420 = 842093913,
    /// 3 plane YCbCr format, 2x2 subsampled Cr (1) and Cb (2) planes
    Eyvu420 = 842094169,
    /// 3 plane YCbCr format, 2x1 subsampled Cb (1) and Cr (2) planes
    Eyuv422 = 909202777,
    /// 3 plane YCbCr format, 2x1 subsampled Cr (1) and Cb (2) planes
    Eyvu422 = 909203033,
    /// 3 plane YCbCr format, non-subsampled Cb (1) and Cr (2) planes
    Eyuv444 = 875713881,
    /// 3 plane YCbCr format, non-subsampled Cr (1) and Cb (2) planes
    Eyvu444 = 875714137,
    /// [7:0] R
    Er8 = 538982482,
    /// [15:0] R little endian
    Er16 = 540422482,
    /// [15:0] R:G 8:8 little endian
    Erg88 = 943212370,
    /// [15:0] G:R 8:8 little endian
    Egr88 = 943215175,
    /// [31:0] R:G 16:16 little endian
    Erg1616 = 842221394,
    /// [31:0] G:R 16:16 little endian
    Egr1616 = 842224199,
    /// [63:0] x:R:G:B 16:16:16:16 little endian
    Exrgb16161616f = 1211388504,
    /// [63:0] x:B:G:R 16:16:16:16 little endian
    Exbgr16161616f = 1211384408,
    /// [63:0] A:R:G:B 16:16:16:16 little endian
    Eargb16161616f = 1211388481,
    /// [63:0] A:B:G:R 16:16:16:16 little endian
    Eabgr16161616f = 1211384385,
    /// [31:0] X:Y:Cb:Cr 8:8:8:8 little endian
    Exyuv8888 = 1448434008,
    /// [23:0] Cr:Cb:Y 8:8:8 little endian
    Evuy888 = 875713878,
    /// Y followed by U then V, 10:10:10. Non-linear modifier only
    Evuy101010 = 808670550,
    /// [63:0] Cr0:0:Y1:0:Cb0:0:Y0:0 10:6:10:6:10:6:10:6 little endian per 2 Y pixels
    Ey210 = 808530521,
    /// [63:0] Cr0:0:Y1:0:Cb0:0:Y0:0 12:4:12:4:12:4:12:4 little endian per 2 Y pixels
    Ey212 = 842084953,
    /// [63:0] Cr0:Y1:Cb0:Y0 16:16:16:16 little endian per 2 Y pixels
    Ey216 = 909193817,
    /// [31:0] A:Cr:Y:Cb 2:10:10:10 little endian
    Ey410 = 808531033,
    /// [63:0] A:0:Cr:0:Y:0:Cb:0 12:4:12:4:12:4:12:4 little endian
    Ey412 = 842085465,
    /// [63:0] A:Cr:Y:Cb 16:16:16:16 little endian
    Ey416 = 909194329,
    /// [31:0] X:Cr:Y:Cb 2:10:10:10 little endian
    Exvyu2101010 = 808670808,
    /// [63:0] X:0:Cr:0:Y:0:Cb:0 12:4:12:4:12:4:12:4 little endian
    Exvyu12_16161616 = 909334104,
    /// [63:0] X:Cr:Y:Cb 16:16:16:16 little endian
    Exvyu16161616 = 942954072,
    /// [63:0]   A3:A2:Y3:0:Cr0:0:Y2:0:A1:A0:Y1:0:Cb0:0:Y0:0  1:1:8:2:8:2:8:2:1:1:8:2:8:2:8:2 little endian
    Ey0l0 = 810299481,
    /// [63:0]   X3:X2:Y3:0:Cr0:0:Y2:0:X1:X0:Y1:0:Cb0:0:Y0:0  1:1:8:2:8:2:8:2:1:1:8:2:8:2:8:2 little endian
    Ex0l0 = 810299480,
    /// [63:0]   A3:A2:Y3:Cr0:Y2:A1:A0:Y1:Cb0:Y0  1:1:10:10:10:1:1:10:10:10 little endian
    Ey0l2 = 843853913,
    /// [63:0]   X3:X2:Y3:Cr0:Y2:X1:X0:Y1:Cb0:Y0  1:1:10:10:10:1:1:10:10:10 little endian
    Ex0l2 = 843853912,
    Eyuv420_8bit = 942691673,
    Eyuv420_10bit = 808539481,
    Exrgb8888_a8 = 943805016,
    Exbgr8888_a8 = 943800920,
    Ergbx8888_a8 = 943806546,
    Ebgrx8888_a8 = 943806530,
    Ergb888_a8 = 943798354,
    Ebgr888_a8 = 943798338,
    Ergb565_a8 = 943797586,
    Ebgr565_a8 = 943797570,
    /// non-subsampled Cr:Cb plane
    Env24 = 875714126,
    /// non-subsampled Cb:Cr plane
    Env42 = 842290766,
    /// 2x1 subsampled Cr:Cb plane, 10 bit per channel
    Ep210 = 808530512,
    /// 2x2 subsampled Cr:Cb plane 10 bits per channel
    Ep010 = 808530000,
    /// 2x2 subsampled Cr:Cb plane 12 bits per channel
    Ep012 = 842084432,
    /// 2x2 subsampled Cr:Cb plane 16 bits per channel
    Ep016 = 909193296,
    /// [63:0] A:x:B:x:G:x:R:x 10:6:10:6:10:6:10:6 little endian
    Eaxbxgxrx106106106106 = 808534593,
    /// 2x2 subsampled Cr:Cb plane
    Env15 = 892425806,
    Eq410 = 808531025,
    Eq401 = 825242705,
    /// [63:0] x:R:G:B 16:16:16:16 little endian
    Exrgb16161616 = 942953048,
    /// [63:0] x:B:G:R 16:16:16:16 little endian
    Exbgr16161616 = 942948952,
    /// [63:0] A:R:G:B 16:16:16:16 little endian
    Eargb16161616 = 942953025,
    /// [63:0] A:B:G:R 16:16:16:16 little endian
    Eabgr16161616 = 942948929,
    /// [7:0] C0:C1:C2:C3:C4:C5:C6:C7 1:1:1:1:1:1:1:1 eight pixels/byte
    Ec1 = 538980675,
    /// [7:0] C0:C1:C2:C3 2:2:2:2 four pixels/byte
    Ec2 = 538980931,
    /// [7:0] C0:C1 4:4 two pixels/byte
    Ec4 = 538981443,
    /// [7:0] D0:D1:D2:D3:D4:D5:D6:D7 1:1:1:1:1:1:1:1 eight pixels/byte
    Ed1 = 538980676,
    /// [7:0] D0:D1:D2:D3 2:2:2:2 four pixels/byte
    Ed2 = 538980932,
    /// [7:0] D0:D1 4:4 two pixels/byte
    Ed4 = 538981444,
    /// [7:0] D
    Ed8 = 538982468,
    /// [7:0] R0:R1:R2:R3:R4:R5:R6:R7 1:1:1:1:1:1:1:1 eight pixels/byte
    Er1 = 538980690,
    /// [7:0] R0:R1:R2:R3 2:2:2:2 four pixels/byte
    Er2 = 538980946,
    /// [7:0] R0:R1 4:4 two pixels/byte
    Er4 = 538981458,
    /// [15:0] x:R 6:10 little endian
    Er10 = 540029266,
    /// [15:0] x:R 4:12 little endian
    Er12 = 540160338,
    /// [31:0] A:Cr:Cb:Y 8:8:8:8 little endian
    Eavuy8888 = 1498764865,
    /// [31:0] X:Cr:Cb:Y 8:8:8:8 little endian
    Exvuy8888 = 1498764888,
    /// 2x2 subsampled Cr:Cb plane 10 bits per channel packed
    Ep030 = 808661072
};

/// create a shm pool
///
/// Create a new wl_shm_pool object.
///
/// The pool can be used to create shared memory based buffer
/// objects.  The server will mmap size bytes of the passed file
/// descriptor, to use as backing memory for the pool.
struct wl_shm::request::create_pool {
    static constexpr Wopcode<wl_shm> opcode{ 0 };
    /// pool to create
    Wnew_id<wl_shm_pool> id;
    /// file descriptor for the pool
    Wfd fd;
    /// pool size, in bytes
    Wint size;
};

/// release the shm object
///
/// Using this request a client can tell the server that it is not going to
/// use the shm object anymore.
///
/// Objects created via this interface remain unaffected.
struct wl_shm::request::release {
    static constexpr Wopcode<wl_shm> opcode{ 1 };
};

/// pixel format description
///
/// Informs the client about a valid pixel format that
/// can be used for buffers. Known formats include
/// argb8888 and xrgb8888.
struct wl_shm::event::format {
    static constexpr Wopcode<wl_shm> opcode{ 0 };
    /// buffer pixel format
    wl_shm::format format;
};

/// destroy a buffer
///
/// Destroy a buffer. If and how you need to release the backing
/// storage is defined by the buffer factory interface.
///
/// For possible side-effects to a surface, see wl_surface.attach.
struct wl_buffer::request::destroy {
    static constexpr Wopcode<wl_buffer> opcode{ 0 };
};

/// compositor releases buffer
///
/// Sent when this wl_buffer is no longer used by the compositor.
/// The client is now free to reuse or destroy this buffer and its
/// backing storage.
///
/// If a client receives a release event before the frame callback
/// requested in the same wl_surface.commit that attaches this
/// wl_buffer to a surface, then the client is immediately free to
/// reuse the buffer and its backing storage, and does not need a
/// second buffer for the next surface content update. Typically
/// this is possible, when the compositor maintains a copy of the
/// wl_surface contents, e.g. as a GL texture. This is an important
/// optimization for GL(ES) compositors with wl_shm clients.
struct wl_buffer::event::release {
    static constexpr Wopcode<wl_buffer> opcode{ 0 };
};

enum class wl_data_offer::error : Wint::integral_type {
    /// finish request was called untimely
    Einvalid_finish = 0,
    /// action mask contains invalid values
    Einvalid_action_mask = 1,
    /// action argument has an invalid value
    Einvalid_action = 2,
    /// offer doesn't accept this request
    Einvalid_offer = 3
};

/// accept one of the offered mime types
///
/// Indicate that the client can accept the given mime type, or
/// NULL for not accepted.
///
/// For objects of version 2 or older, this request is used by the
/// client to give feedback whether the client can receive the given
/// mime type, or NULL if none is accepted; the feedback does not
/// determine whether the drag-and-drop operation succeeds or not.
///
/// For objects of version 3 or newer, this request determines the
/// final result of the drag-and-drop operation. If the end result
/// is that no mime types were accepted, the drag-and-drop operation
/// will be cancelled and the corresponding drag source will receive
/// wl_data_source.cancelled. Clients may still use this event in
/// conjunction with wl_data_source.action for feedback.
struct wl_data_offer::request::accept {
    static constexpr Wopcode<wl_data_offer> opcode{ 0 };
    /// serial number of the accept request
    Wuint serial;
    /// mime type accepted by the client
    Wstring mime_type;
};

/// request that the data is transferred
///
/// To transfer the offered data, the client issues this request
/// and indicates the mime type it wants to receive.  The transfer
/// happens through the passed file descriptor (typically created
/// with the pipe system call).  The source client writes the data
/// in the mime type representation requested and then closes the
/// file descriptor.
///
/// The receiving client reads from the read end of the pipe until
/// EOF and then closes its end, at which point the transfer is
/// complete.
///
/// This request may happen multiple times for different mime types,
/// both before and after wl_data_device.drop. Drag-and-drop destination
/// clients may preemptively fetch data or examine it more closely to
/// determine acceptance.
struct wl_data_offer::request::receive {
    static constexpr Wopcode<wl_data_offer> opcode{ 1 };
    /// mime type desired by receiver
    Wstring mime_type;
    /// file descriptor for data transfer
    Wfd fd;
};

/// destroy data offer
///
/// Destroy the data offer.
struct wl_data_offer::request::destroy {
    static constexpr Wopcode<wl_data_offer> opcode{ 2 };
};

/// the offer will no longer be used
///
/// Notifies the compositor that the drag destination successfully
/// finished the drag-and-drop operation.
///
/// Upon receiving this request, the compositor will emit
/// wl_data_source.dnd_finished on the drag source client.
///
/// It is a client error to perform other requests than
/// wl_data_offer.destroy after this one. It is also an error to perform
/// this request after a NULL mime type has been set in
/// wl_data_offer.accept or no action was received through
/// wl_data_offer.action.
///
/// If wl_data_offer.finish request is received for a non drag and drop
/// operation, the invalid_finish protocol error is raised.
struct wl_data_offer::request::finish {
    static constexpr Wopcode<wl_data_offer> opcode{ 3 };
};

/// set the available/preferred drag-and-drop actions
///
/// Sets the actions that the destination side client supports for
/// this operation. This request may trigger the emission of
/// wl_data_source.action and wl_data_offer.action events if the compositor
/// needs to change the selected action.
///
/// This request can be called multiple times throughout the
/// drag-and-drop operation, typically in response to wl_data_device.enter
/// or wl_data_device.motion events.
///
/// This request determines the final result of the drag-and-drop
/// operation. If the end result is that no action is accepted,
/// the drag source will receive wl_data_source.cancelled.
///
/// The dnd_actions argument must contain only values expressed in the
/// wl_data_device_manager.dnd_actions enum, and the preferred_action
/// argument must only contain one of those values set, otherwise it
/// will result in a protocol error.
///
/// While managing an "ask" action, the destination drag-and-drop client
/// may perform further wl_data_offer.receive requests, and is expected
/// to perform one last wl_data_offer.set_actions request with a preferred
/// action other than "ask" (and optionally wl_data_offer.accept) before
/// requesting wl_data_offer.finish, in order to convey the action selected
/// by the user. If the preferred action is not in the
/// wl_data_offer.source_actions mask, an error will be raised.
///
/// If the "ask" action is dismissed (e.g. user cancellation), the client
/// is expected to perform wl_data_offer.destroy right away.
///
/// This request can only be made on drag-and-drop offers, a protocol error
/// will be raised otherwise.
struct wl_data_offer::request::set_actions {
    static constexpr Wopcode<wl_data_offer> opcode{ 4 };
    /// actions supported by the destination client
    wl_data_device_manager::dnd_action dnd_actions;
    /// action preferred by the destination client
    wl_data_device_manager::dnd_action preferred_action;
};

/// advertise offered mime type
///
/// Sent immediately after creating the wl_data_offer object.  One
/// event per offered mime type.
struct wl_data_offer::event::offer {
    static constexpr Wopcode<wl_data_offer> opcode{ 0 };
    /// offered mime type
    Wstring mime_type;
};

/// notify the source-side available actions
///
/// This event indicates the actions offered by the data source. It
/// will be sent immediately after creating the wl_data_offer object,
/// or anytime the source side changes its offered actions through
/// wl_data_source.set_actions.
struct wl_data_offer::event::source_actions {
    static constexpr Wopcode<wl_data_offer> opcode{ 1 };
    /// actions offered by the data source
    wl_data_device_manager::dnd_action source_actions;
};

/// notify the selected action
///
/// This event indicates the action selected by the compositor after
/// matching the source/destination side actions. Only one action (or
/// none) will be offered here.
///
/// This event can be emitted multiple times during the drag-and-drop
/// operation in response to destination side action changes through
/// wl_data_offer.set_actions.
///
/// This event will no longer be emitted after wl_data_device.drop
/// happened on the drag-and-drop destination, the client must
/// honor the last action received, or the last preferred one set
/// through wl_data_offer.set_actions when handling an "ask" action.
///
/// Compositors may also change the selected action on the fly, mainly
/// in response to keyboard modifier changes during the drag-and-drop
/// operation.
///
/// The most recent action received is always the valid one. Prior to
/// receiving wl_data_device.drop, the chosen action may change (e.g.
/// due to keyboard modifiers being pressed). At the time of receiving
/// wl_data_device.drop the drag-and-drop destination must honor the
/// last action received.
///
/// Action changes may still happen after wl_data_device.drop,
/// especially on "ask" actions, where the drag-and-drop destination
/// may choose another action afterwards. Action changes happening
/// at this stage are always the result of inter-client negotiation, the
/// compositor shall no longer be able to induce a different action.
///
/// Upon "ask" actions, it is expected that the drag-and-drop destination
/// may potentially choose a different action and/or mime type,
/// based on wl_data_offer.source_actions and finally chosen by the
/// user (e.g. popping up a menu with the available options). The
/// final wl_data_offer.set_actions and wl_data_offer.accept requests
/// must happen before the call to wl_data_offer.finish.
struct wl_data_offer::event::action {
    static constexpr Wopcode<wl_data_offer> opcode{ 2 };
    /// action selected by the compositor
    wl_data_device_manager::dnd_action dnd_action;
};

enum class wl_data_source::error : Wint::integral_type {
    /// action mask contains invalid values
    Einvalid_action_mask = 0,
    /// source doesn't accept this request
    Einvalid_source = 1
};

/// add an offered mime type
///
/// This request adds a mime type to the set of mime types
/// advertised to targets.  Can be called several times to offer
/// multiple types.
struct wl_data_source::request::offer {
    static constexpr Wopcode<wl_data_source> opcode{ 0 };
    /// mime type offered by the data source
    Wstring mime_type;
};

/// destroy the data source
///
/// Destroy the data source.
struct wl_data_source::request::destroy {
    static constexpr Wopcode<wl_data_source> opcode{ 1 };
};

/// set the available drag-and-drop actions
///
/// Sets the actions that the source side client supports for this
/// operation. This request may trigger wl_data_source.action and
/// wl_data_offer.action events if the compositor needs to change the
/// selected action.
///
/// The dnd_actions argument must contain only values expressed in the
/// wl_data_device_manager.dnd_actions enum, otherwise it will result
/// in a protocol error.
///
/// This request must be made once only, and can only be made on sources
/// used in drag-and-drop, so it must be performed before
/// wl_data_device.start_drag. Attempting to use the source other than
/// for drag-and-drop will raise a protocol error.
struct wl_data_source::request::set_actions {
    static constexpr Wopcode<wl_data_source> opcode{ 2 };
    /// actions supported by the data source
    wl_data_device_manager::dnd_action dnd_actions;
};

/// a target accepts an offered mime type
///
/// Sent when a target accepts pointer_focus or motion events.  If
/// a target does not accept any of the offered types, type is NULL.
///
/// Used for feedback during drag-and-drop.
struct wl_data_source::event::target {
    static constexpr Wopcode<wl_data_source> opcode{ 0 };
    /// mime type accepted by the target
    Wstring mime_type;
};

/// send the data
///
/// Request for data from the client.  Send the data as the
/// specified mime type over the passed file descriptor, then
/// close it.
struct wl_data_source::event::send {
    static constexpr Wopcode<wl_data_source> opcode{ 1 };
    /// mime type for the data
    Wstring mime_type;
    /// file descriptor for the data
    Wfd fd;
};

/// selection was cancelled
///
/// This data source is no longer valid. There are several reasons why
/// this could happen:
///
/// - The data source has been replaced by another data source.
/// - The drag-and-drop operation was performed, but the drop destination
/// did not accept any of the mime types offered through
/// wl_data_source.target.
/// - The drag-and-drop operation was performed, but the drop destination
/// did not select any of the actions present in the mask offered through
/// wl_data_source.action.
/// - The drag-and-drop operation was performed but didn't happen over a
/// surface.
/// - The compositor cancelled the drag-and-drop operation (e.g. compositor
/// dependent timeouts to avoid stale drag-and-drop transfers).
///
/// The client should clean up and destroy this data source.
///
/// For objects of version 2 or older, wl_data_source.cancelled will
/// only be emitted if the data source was replaced by another data
/// source.
struct wl_data_source::event::cancelled {
    static constexpr Wopcode<wl_data_source> opcode{ 2 };
};

/// the drag-and-drop operation physically finished
///
/// The user performed the drop action. This event does not indicate
/// acceptance, wl_data_source.cancelled may still be emitted afterwards
/// if the drop destination does not accept any mime type.
///
/// However, this event might however not be received if the compositor
/// cancelled the drag-and-drop operation before this event could happen.
///
/// Note that the data_source may still be used in the future and should
/// not be destroyed here.
struct wl_data_source::event::dnd_drop_performed {
    static constexpr Wopcode<wl_data_source> opcode{ 3 };
};

/// the drag-and-drop operation concluded
///
/// The drop destination finished interoperating with this data
/// source, so the client is now free to destroy this data source and
/// free all associated data.
///
/// If the action used to perform the operation was "move", the
/// source can now delete the transferred data.
struct wl_data_source::event::dnd_finished {
    static constexpr Wopcode<wl_data_source> opcode{ 4 };
};

/// notify the selected action
///
/// This event indicates the action selected by the compositor after
/// matching the source/destination side actions. Only one action (or
/// none) will be offered here.
///
/// This event can be emitted multiple times during the drag-and-drop
/// operation, mainly in response to destination side changes through
/// wl_data_offer.set_actions, and as the data device enters/leaves
/// surfaces.
///
/// It is only possible to receive this event after
/// wl_data_source.dnd_drop_performed if the drag-and-drop operation
/// ended in an "ask" action, in which case the final wl_data_source.action
/// event will happen immediately before wl_data_source.dnd_finished.
///
/// Compositors may also change the selected action on the fly, mainly
/// in response to keyboard modifier changes during the drag-and-drop
/// operation.
///
/// The most recent action received is always the valid one. The chosen
/// action may change alongside negotiation (e.g. an "ask" action can turn
/// into a "move" operation), so the effects of the final action must
/// always be applied in wl_data_offer.dnd_finished.
///
/// Clients can trigger cursor surface changes from this point, so
/// they reflect the current action.
struct wl_data_source::event::action {
    static constexpr Wopcode<wl_data_source> opcode{ 5 };
    /// action selected by the compositor
    wl_data_device_manager::dnd_action dnd_action;
};

enum class wl_data_device::error : Wint::integral_type {
    /// given wl_surface has another role
    Erole = 0,
    /// source has already been used
    Eused_source = 1
};

/// start drag-and-drop operation
///
/// This request asks the compositor to start a drag-and-drop
/// operation on behalf of the client.
///
/// The source argument is the data source that provides the data
/// for the eventual data transfer. If source is NULL, enter, leave
/// and motion events are sent only to the client that initiated the
/// drag and the client is expected to handle the data passing
/// internally. If source is destroyed, the drag-and-drop session will be
/// cancelled.
///
/// The origin surface is the surface where the drag originates and
/// the client must have an active implicit grab that matches the
/// serial.
///
/// The icon surface is an optional (can be NULL) surface that
/// provides an icon to be moved around with the cursor.  Initially,
/// the top-left corner of the icon surface is placed at the cursor
/// hotspot, but subsequent wl_surface.offset requests can move the
/// relative position. Attach requests must be confirmed with
/// wl_surface.commit as usual. The icon surface is given the role of
/// a drag-and-drop icon. If the icon surface already has another role,
/// it raises a protocol error.
///
/// The input region is ignored for wl_surfaces with the role of a
/// drag-and-drop icon.
///
/// The given source may not be used in any further set_selection or
/// start_drag requests. Attempting to reuse a previously-used source
/// may send a used_source error.
struct wl_data_device::request::start_drag {
    static constexpr Wopcode<wl_data_device> opcode{ 0 };
    /// data source for the eventual transfer
    Wobject<wl_data_source> source;
    /// surface where the drag originates
    Wobject<wl_surface> origin;
    /// drag-and-drop icon surface
    Wobject<wl_surface> icon;
    /// serial number of the implicit grab on the origin
    Wuint serial;
};

/// copy data to the selection
///
/// This request asks the compositor to set the selection
/// to the data from the source on behalf of the client.
///
/// To unset the selection, set the source to NULL.
///
/// The given source may not be used in any further set_selection or
/// start_drag requests. Attempting to reuse a previously-used source
/// may send a used_source error.
struct wl_data_device::request::set_selection {
    static constexpr Wopcode<wl_data_device> opcode{ 1 };
    /// data source for the selection
    Wobject<wl_data_source> source;
    /// serial number of the event that triggered this request
    Wuint serial;
};

/// destroy data device
///
/// This request destroys the data device.
struct wl_data_device::request::release {
    static constexpr Wopcode<wl_data_device> opcode{ 2 };
};

/// introduce a new wl_data_offer
///
/// The data_offer event introduces a new wl_data_offer object,
/// which will subsequently be used in either the
/// data_device.enter event (for drag-and-drop) or the
/// data_device.selection event (for selections).  Immediately
/// following the data_device.data_offer event, the new data_offer
/// object will send out data_offer.offer events to describe the
/// mime types it offers.
struct wl_data_device::event::data_offer {
    static constexpr Wopcode<wl_data_device> opcode{ 0 };
    /// the new data_offer object
    Wnew_id<wl_data_offer> id;
};

/// initiate drag-and-drop session
///
/// This event is sent when an active drag-and-drop pointer enters
/// a surface owned by the client.  The position of the pointer at
/// enter time is provided by the x and y arguments, in surface-local
/// coordinates.
struct wl_data_device::event::enter {
    static constexpr Wopcode<wl_data_device> opcode{ 1 };
    /// serial number of the enter event
    Wuint serial;
    /// client surface entered
    Wobject<wl_surface> surface;
    /// surface-local x coordinate
    Wfixed x;
    /// surface-local y coordinate
    Wfixed y;
    /// source data_offer object
    Wobject<wl_data_offer> id;
};

/// end drag-and-drop session
///
/// This event is sent when the drag-and-drop pointer leaves the
/// surface and the session ends.  The client must destroy the
/// wl_data_offer introduced at enter time at this point.
struct wl_data_device::event::leave {
    static constexpr Wopcode<wl_data_device> opcode{ 2 };
};

/// drag-and-drop session motion
///
/// This event is sent when the drag-and-drop pointer moves within
/// the currently focused surface. The new position of the pointer
/// is provided by the x and y arguments, in surface-local
/// coordinates.
struct wl_data_device::event::motion {
    static constexpr Wopcode<wl_data_device> opcode{ 3 };
    /// timestamp with millisecond granularity
    Wuint time;
    /// surface-local x coordinate
    Wfixed x;
    /// surface-local y coordinate
    Wfixed y;
};

/// end drag-and-drop session successfully
///
/// The event is sent when a drag-and-drop operation is ended
/// because the implicit grab is removed.
///
/// The drag-and-drop destination is expected to honor the last action
/// received through wl_data_offer.action, if the resulting action is
/// "copy" or "move", the destination can still perform
/// wl_data_offer.receive requests, and is expected to end all
/// transfers with a wl_data_offer.finish request.
///
/// If the resulting action is "ask", the action will not be considered
/// final. The drag-and-drop destination is expected to perform one last
/// wl_data_offer.set_actions request, or wl_data_offer.destroy in order
/// to cancel the operation.
struct wl_data_device::event::drop {
    static constexpr Wopcode<wl_data_device> opcode{ 4 };
};

/// advertise new selection
///
/// The selection event is sent out to notify the client of a new
/// wl_data_offer for the selection for this device.  The
/// data_device.data_offer and the data_offer.offer events are
/// sent out immediately before this event to introduce the data
/// offer object.  The selection event is sent to a client
/// immediately before receiving keyboard focus and when a new
/// selection is set while the client has keyboard focus.  The
/// data_offer is valid until a new data_offer or NULL is received
/// or until the client loses keyboard focus.  Switching surface with
/// keyboard focus within the same client doesn't mean a new selection
/// will be sent.  The client must destroy the previous selection
/// data_offer, if any, upon receiving this event.
struct wl_data_device::event::selection {
    static constexpr Wopcode<wl_data_device> opcode{ 5 };
    /// selection data_offer object
    Wobject<wl_data_offer> id;
};

/// drag and drop actions
///
/// This is a bitmask of the available/preferred actions in a
/// drag-and-drop operation.
///
/// In the compositor, the selected action is a result of matching the
/// actions offered by the source and destination sides.  "action" events
/// with a "none" action will be sent to both source and destination if
/// there is no match. All further checks will effectively happen on
/// (source actions ∩ destination actions).
///
/// In addition, compositors may also pick different actions in
/// reaction to key modifiers being pressed. One common design that
/// is used in major toolkits (and the behavior recommended for
/// compositors) is:
///
/// - If no modifiers are pressed, the first match (in bit order)
/// will be used.
/// - Pressing Shift selects "move", if enabled in the mask.
/// - Pressing Control selects "copy", if enabled in the mask.
///
/// Behavior beyond that is considered implementation-dependent.
/// Compositors may for example bind other modifiers (like Alt/Meta)
/// or drags initiated with other buttons than BTN_LEFT to specific
/// actions (e.g. "ask").
enum class wl_data_device_manager::dnd_action : Wuint::integral_type {
    /// no action
    Enone = 0,
    /// copy action
    Ecopy = 1,
    /// move action
    Emove = 2,
    /// ask action
    Eask = 4
};

/// create a new data source
///
/// Create a new data source.
struct wl_data_device_manager::request::create_data_source {
    static constexpr Wopcode<wl_data_device_manager> opcode{ 0 };
    /// data source to create
    Wnew_id<wl_data_source> id;
};

/// create a new data device
///
/// Create a new data device for a given seat.
struct wl_data_device_manager::request::get_data_device {
    static constexpr Wopcode<wl_data_device_manager> opcode{ 1 };
    /// data device to create
    Wnew_id<wl_data_device> id;
    /// seat associated with the data device
    Wobject<wl_seat> seat;
};

enum class wl_shell::error : Wint::integral_type {
    /// given wl_surface has another role
    Erole = 0
};

/// create a shell surface from a surface
///
/// Create a shell surface for an existing surface. This gives
/// the wl_surface the role of a shell surface. If the wl_surface
/// already has another role, it raises a protocol error.
///
/// Only one shell surface can be associated with a given surface.
struct wl_shell::request::get_shell_surface {
    static constexpr Wopcode<wl_shell> opcode{ 0 };
    /// shell surface to create
    Wnew_id<wl_shell_surface> id;
    /// surface to be given the shell surface role
    Wobject<wl_surface> surface;
};

/// edge values for resizing
///
/// These values are used to indicate which edge of a surface
/// is being dragged in a resize operation. The server may
/// use this information to adapt its behavior, e.g. choose
/// an appropriate cursor image.
enum class wl_shell_surface::resize : Wuint::integral_type {
    /// no edge
    Enone = 0,
    /// top edge
    Etop = 1,
    /// bottom edge
    Ebottom = 2,
    /// left edge
    Eleft = 4,
    /// top and left edges
    Etop_left = 5,
    /// bottom and left edges
    Ebottom_left = 6,
    /// right edge
    Eright = 8,
    /// top and right edges
    Etop_right = 9,
    /// bottom and right edges
    Ebottom_right = 10
};

/// details of transient behaviour
///
/// These flags specify details of the expected behaviour
/// of transient surfaces. Used in the set_transient request.
enum class wl_shell_surface::transient : Wuint::integral_type {
    /// do not set keyboard focus
    Einactive = 1
};

/// different method to set the surface fullscreen
///
/// Hints to indicate to the compositor how to deal with a conflict
/// between the dimensions of the surface and the dimensions of the
/// output. The compositor is free to ignore this parameter.
enum class wl_shell_surface::fullscreen_method : Wint::integral_type {
    /// no preference, apply default policy
    Edefault = 0,
    /// scale, preserve the surface's aspect ratio and center on output
    Escale = 1,
    /// switch output mode to the smallest mode that can fit the surface, add black borders to compensate size mismatch
    Edriver = 2,
    /// no upscaling, center on output and add black borders to compensate size mismatch
    Efill = 3
};

/// respond to a ping event
///
/// A client must respond to a ping event with a pong request or
/// the client may be deemed unresponsive.
struct wl_shell_surface::request::pong {
    static constexpr Wopcode<wl_shell_surface> opcode{ 0 };
    /// serial number of the ping event
    Wuint serial;
};

/// start an interactive move
///
/// Start a pointer-driven move of the surface.
///
/// This request must be used in response to a button press event.
/// The server may ignore move requests depending on the state of
/// the surface (e.g. fullscreen or maximized).
struct wl_shell_surface::request::move {
    static constexpr Wopcode<wl_shell_surface> opcode{ 1 };
    /// seat whose pointer is used
    Wobject<wl_seat> seat;
    /// serial number of the implicit grab on the pointer
    Wuint serial;
};

/// start an interactive resize
///
/// Start a pointer-driven resizing of the surface.
///
/// This request must be used in response to a button press event.
/// The server may ignore resize requests depending on the state of
/// the surface (e.g. fullscreen or maximized).
struct wl_shell_surface::request::resize {
    static constexpr Wopcode<wl_shell_surface> opcode{ 2 };
    /// seat whose pointer is used
    Wobject<wl_seat> seat;
    /// serial number of the implicit grab on the pointer
    Wuint serial;
    /// which edge or corner is being dragged
    wl_shell_surface::resize edges;
};

/// make the surface a toplevel surface
///
/// Map the surface as a toplevel surface.
///
/// A toplevel surface is not fullscreen, maximized or transient.
struct wl_shell_surface::request::set_toplevel {
    static constexpr Wopcode<wl_shell_surface> opcode{ 3 };
};

/// make the surface a transient surface
///
/// Map the surface relative to an existing surface.
///
/// The x and y arguments specify the location of the upper left
/// corner of the surface relative to the upper left corner of the
/// parent surface, in surface-local coordinates.
///
/// The flags argument controls details of the transient behaviour.
struct wl_shell_surface::request::set_transient {
    static constexpr Wopcode<wl_shell_surface> opcode{ 4 };
    /// parent surface
    Wobject<wl_surface> parent;
    /// surface-local x coordinate
    Wint x;
    /// surface-local y coordinate
    Wint y;
    /// transient surface behavior
    wl_shell_surface::transient flags;
};

/// make the surface a fullscreen surface
///
/// Map the surface as a fullscreen surface.
///
/// If an output parameter is given then the surface will be made
/// fullscreen on that output. If the client does not specify the
/// output then the compositor will apply its policy - usually
/// choosing the output on which the surface has the biggest surface
/// area.
///
/// The client may specify a method to resolve a size conflict
/// between the output size and the surface size - this is provided
/// through the method parameter.
///
/// The framerate parameter is used only when the method is set
/// to "driver", to indicate the preferred framerate. A value of 0
/// indicates that the client does not care about framerate.  The
/// framerate is specified in mHz, that is framerate of 60000 is 60Hz.
///
/// A method of "scale" or "driver" implies a scaling operation of
/// the surface, either via a direct scaling operation or a change of
/// the output mode. This will override any kind of output scaling, so
/// that mapping a surface with a buffer size equal to the mode can
/// fill the screen independent of buffer_scale.
///
/// A method of "fill" means we don't scale up the buffer, however
/// any output scale is applied. This means that you may run into
/// an edge case where the application maps a buffer with the same
/// size of the output mode but buffer_scale 1 (thus making a
/// surface larger than the output). In this case it is allowed to
/// downscale the results to fit the screen.
///
/// The compositor must reply to this request with a configure event
/// with the dimensions for the output on which the surface will
/// be made fullscreen.
struct wl_shell_surface::request::set_fullscreen {
    static constexpr Wopcode<wl_shell_surface> opcode{ 5 };
    /// method for resolving size conflict
    wl_shell_surface::fullscreen_method method;
    /// framerate in mHz
    Wuint framerate;
    /// output on which the surface is to be fullscreen
    Wobject<wl_output> output;
};

/// make the surface a popup surface
///
/// Map the surface as a popup.
///
/// A popup surface is a transient surface with an added pointer
/// grab.
///
/// An existing implicit grab will be changed to owner-events mode,
/// and the popup grab will continue after the implicit grab ends
/// (i.e. releasing the mouse button does not cause the popup to
/// be unmapped).
///
/// The popup grab continues until the window is destroyed or a
/// mouse button is pressed in any other client's window. A click
/// in any of the client's surfaces is reported as normal, however,
/// clicks in other clients' surfaces will be discarded and trigger
/// the callback.
///
/// The x and y arguments specify the location of the upper left
/// corner of the surface relative to the upper left corner of the
/// parent surface, in surface-local coordinates.
struct wl_shell_surface::request::set_popup {
    static constexpr Wopcode<wl_shell_surface> opcode{ 6 };
    /// seat whose pointer is used
    Wobject<wl_seat> seat;
    /// serial number of the implicit grab on the pointer
    Wuint serial;
    /// parent surface
    Wobject<wl_surface> parent;
    /// surface-local x coordinate
    Wint x;
    /// surface-local y coordinate
    Wint y;
    /// transient surface behavior
    wl_shell_surface::transient flags;
};

/// make the surface a maximized surface
///
/// Map the surface as a maximized surface.
///
/// If an output parameter is given then the surface will be
/// maximized on that output. If the client does not specify the
/// output then the compositor will apply its policy - usually
/// choosing the output on which the surface has the biggest surface
/// area.
///
/// The compositor will reply with a configure event telling
/// the expected new surface size. The operation is completed
/// on the next buffer attach to this surface.
///
/// A maximized surface typically fills the entire output it is
/// bound to, except for desktop elements such as panels. This is
/// the main difference between a maximized shell surface and a
/// fullscreen shell surface.
///
/// The details depend on the compositor implementation.
struct wl_shell_surface::request::set_maximized {
    static constexpr Wopcode<wl_shell_surface> opcode{ 7 };
    /// output on which the surface is to be maximized
    Wobject<wl_output> output;
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
struct wl_shell_surface::request::set_title {
    static constexpr Wopcode<wl_shell_surface> opcode{ 8 };
    /// surface title
    Wstring title;
};

/// set surface class
///
/// Set a class for the surface.
///
/// The surface class identifies the general class of applications
/// to which the surface belongs. A common convention is to use the
/// file name (or the full path if it is a non-standard location) of
/// the application's .desktop file as the class.
struct wl_shell_surface::request::set_class {
    static constexpr Wopcode<wl_shell_surface> opcode{ 9 };
    /// surface class
    Wstring class_;
};

/// ping client
///
/// Ping a client to check if it is receiving events and sending
/// requests. A client is expected to reply with a pong request.
struct wl_shell_surface::event::ping {
    static constexpr Wopcode<wl_shell_surface> opcode{ 0 };
    /// serial number of the ping
    Wuint serial;
};

/// suggest resize
///
/// The configure event asks the client to resize its surface.
///
/// The size is a hint, in the sense that the client is free to
/// ignore it if it doesn't resize, pick a smaller size (to
/// satisfy aspect ratio or resize in steps of NxM pixels).
///
/// The edges parameter provides a hint about how the surface
/// was resized. The client may use this information to decide
/// how to adjust its content to the new size (e.g. a scrolling
/// area might adjust its content position to leave the viewable
/// content unmoved).
///
/// The client is free to dismiss all but the last configure
/// event it received.
///
/// The width and height arguments specify the size of the window
/// in surface-local coordinates.
struct wl_shell_surface::event::configure {
    static constexpr Wopcode<wl_shell_surface> opcode{ 1 };
    /// how the surface was resized
    wl_shell_surface::resize edges;
    /// new width of the surface
    Wint width;
    /// new height of the surface
    Wint height;
};

/// popup interaction is done
///
/// The popup_done event is sent out when a popup grab is broken,
/// that is, when the user clicks a surface that doesn't belong
/// to the client owning the popup surface.
struct wl_shell_surface::event::popup_done {
    static constexpr Wopcode<wl_shell_surface> opcode{ 2 };
};

/// wl_surface error values
///
/// These errors can be emitted in response to wl_surface requests.
enum class wl_surface::error : Wint::integral_type {
    /// buffer scale value is invalid
    Einvalid_scale = 0,
    /// buffer transform value is invalid
    Einvalid_transform = 1,
    /// buffer size is invalid
    Einvalid_size = 2,
    /// buffer offset is invalid
    Einvalid_offset = 3,
    /// surface was destroyed before its role object
    Edefunct_role_object = 4
};

/// delete surface
///
/// Deletes the surface and invalidates its object ID.
struct wl_surface::request::destroy {
    static constexpr Wopcode<wl_surface> opcode{ 0 };
};

/// set the surface contents
///
/// Set a buffer as the content of this surface.
///
/// The new size of the surface is calculated based on the buffer
/// size transformed by the inverse buffer_transform and the
/// inverse buffer_scale. This means that at commit time the supplied
/// buffer size must be an integer multiple of the buffer_scale. If
/// that's not the case, an invalid_size error is sent.
///
/// The x and y arguments specify the location of the new pending
/// buffer's upper left corner, relative to the current buffer's upper
/// left corner, in surface-local coordinates. In other words, the
/// x and y, combined with the new surface size define in which
/// directions the surface's size changes. Setting anything other than 0
/// as x and y arguments is discouraged, and should instead be replaced
/// with using the separate wl_surface.offset request.
///
/// When the bound wl_surface version is 5 or higher, passing any
/// non-zero x or y is a protocol violation, and will result in an
/// 'invalid_offset' error being raised. The x and y arguments are ignored
/// and do not change the pending state. To achieve equivalent semantics,
/// use wl_surface.offset.
///
/// Surface contents are double-buffered state, see wl_surface.commit.
///
/// The initial surface contents are void; there is no content.
/// wl_surface.attach assigns the given wl_buffer as the pending
/// wl_buffer. wl_surface.commit makes the pending wl_buffer the new
/// surface contents, and the size of the surface becomes the size
/// calculated from the wl_buffer, as described above. After commit,
/// there is no pending buffer until the next attach.
///
/// Committing a pending wl_buffer allows the compositor to read the
/// pixels in the wl_buffer. The compositor may access the pixels at
/// any time after the wl_surface.commit request. When the compositor
/// will not access the pixels anymore, it will send the
/// wl_buffer.release event. Only after receiving wl_buffer.release,
/// the client may reuse the wl_buffer. A wl_buffer that has been
/// attached and then replaced by another attach instead of committed
/// will not receive a release event, and is not used by the
/// compositor.
///
/// If a pending wl_buffer has been committed to more than one wl_surface,
/// the delivery of wl_buffer.release events becomes undefined. A well
/// behaved client should not rely on wl_buffer.release events in this
/// case. Alternatively, a client could create multiple wl_buffer objects
/// from the same backing storage or use wp_linux_buffer_release.
///
/// Destroying the wl_buffer after wl_buffer.release does not change
/// the surface contents. Destroying the wl_buffer before wl_buffer.release
/// is allowed as long as the underlying buffer storage isn't re-used (this
/// can happen e.g. on client process termination). However, if the client
/// destroys the wl_buffer before receiving the wl_buffer.release event and
/// mutates the underlying buffer storage, the surface contents become
/// undefined immediately.
///
/// If wl_surface.attach is sent with a NULL wl_buffer, the
/// following wl_surface.commit will remove the surface content.
///
/// If a pending wl_buffer has been destroyed, the result is not specified.
/// Many compositors are known to remove the surface content on the following
/// wl_surface.commit, but this behaviour is not universal. Clients seeking to
/// maximise compatibility should not destroy pending buffers and should
/// ensure that they explicitly remove content from surfaces, even after
/// destroying buffers.
struct wl_surface::request::attach {
    static constexpr Wopcode<wl_surface> opcode{ 1 };
    /// buffer of surface contents
    Wobject<wl_buffer> buffer;
    /// surface-local x coordinate
    Wint x;
    /// surface-local y coordinate
    Wint y;
};

/// mark part of the surface damaged
///
/// This request is used to describe the regions where the pending
/// buffer is different from the current surface contents, and where
/// the surface therefore needs to be repainted. The compositor
/// ignores the parts of the damage that fall outside of the surface.
///
/// Damage is double-buffered state, see wl_surface.commit.
///
/// The damage rectangle is specified in surface-local coordinates,
/// where x and y specify the upper left corner of the damage rectangle.
///
/// The initial value for pending damage is empty: no damage.
/// wl_surface.damage adds pending damage: the new pending damage
/// is the union of old pending damage and the given rectangle.
///
/// wl_surface.commit assigns pending damage as the current damage,
/// and clears pending damage. The server will clear the current
/// damage as it repaints the surface.
///
/// Note! New clients should not use this request. Instead damage can be
/// posted with wl_surface.damage_buffer which uses buffer coordinates
/// instead of surface coordinates.
struct wl_surface::request::damage {
    static constexpr Wopcode<wl_surface> opcode{ 2 };
    /// surface-local x coordinate
    Wint x;
    /// surface-local y coordinate
    Wint y;
    /// width of damage rectangle
    Wint width;
    /// height of damage rectangle
    Wint height;
};

/// request a frame throttling hint
///
/// Request a notification when it is a good time to start drawing a new
/// frame, by creating a frame callback. This is useful for throttling
/// redrawing operations, and driving animations.
///
/// When a client is animating on a wl_surface, it can use the 'frame'
/// request to get notified when it is a good time to draw and commit the
/// next frame of animation. If the client commits an update earlier than
/// that, it is likely that some updates will not make it to the display,
/// and the client is wasting resources by drawing too often.
///
/// The frame request will take effect on the next wl_surface.commit.
/// The notification will only be posted for one frame unless
/// requested again. For a wl_surface, the notifications are posted in
/// the order the frame requests were committed.
///
/// The server must send the notifications so that a client
/// will not send excessive updates, while still allowing
/// the highest possible update rate for clients that wait for the reply
/// before drawing again. The server should give some time for the client
/// to draw and commit after sending the frame callback events to let it
/// hit the next output refresh.
///
/// A server should avoid signaling the frame callbacks if the
/// surface is not visible in any way, e.g. the surface is off-screen,
/// or completely obscured by other opaque surfaces.
///
/// The object returned by this request will be destroyed by the
/// compositor after the callback is fired and as such the client must not
/// attempt to use it after that point.
///
/// The callback_data passed in the callback is the current time, in
/// milliseconds, with an undefined base.
struct wl_surface::request::frame {
    static constexpr Wopcode<wl_surface> opcode{ 3 };
    /// callback object for the frame request
    Wnew_id<wl_callback> callback;
};

/// set opaque region
///
/// This request sets the region of the surface that contains
/// opaque content.
///
/// The opaque region is an optimization hint for the compositor
/// that lets it optimize the redrawing of content behind opaque
/// regions.  Setting an opaque region is not required for correct
/// behaviour, but marking transparent content as opaque will result
/// in repaint artifacts.
///
/// The opaque region is specified in surface-local coordinates.
///
/// The compositor ignores the parts of the opaque region that fall
/// outside of the surface.
///
/// Opaque region is double-buffered state, see wl_surface.commit.
///
/// wl_surface.set_opaque_region changes the pending opaque region.
/// wl_surface.commit copies the pending region to the current region.
/// Otherwise, the pending and current regions are never changed.
///
/// The initial value for an opaque region is empty. Setting the pending
/// opaque region has copy semantics, and the wl_region object can be
/// destroyed immediately. A NULL wl_region causes the pending opaque
/// region to be set to empty.
struct wl_surface::request::set_opaque_region {
    static constexpr Wopcode<wl_surface> opcode{ 4 };
    /// opaque region of the surface
    Wobject<wl_region> region;
};

/// set input region
///
/// This request sets the region of the surface that can receive
/// pointer and touch events.
///
/// Input events happening outside of this region will try the next
/// surface in the server surface stack. The compositor ignores the
/// parts of the input region that fall outside of the surface.
///
/// The input region is specified in surface-local coordinates.
///
/// Input region is double-buffered state, see wl_surface.commit.
///
/// wl_surface.set_input_region changes the pending input region.
/// wl_surface.commit copies the pending region to the current region.
/// Otherwise the pending and current regions are never changed,
/// except cursor and icon surfaces are special cases, see
/// wl_pointer.set_cursor and wl_data_device.start_drag.
///
/// The initial value for an input region is infinite. That means the
/// whole surface will accept input. Setting the pending input region
/// has copy semantics, and the wl_region object can be destroyed
/// immediately. A NULL wl_region causes the input region to be set
/// to infinite.
struct wl_surface::request::set_input_region {
    static constexpr Wopcode<wl_surface> opcode{ 5 };
    /// input region of the surface
    Wobject<wl_region> region;
};

/// commit pending surface state
///
/// Surface state (input, opaque, and damage regions, attached buffers,
/// etc.) is double-buffered. Protocol requests modify the pending state,
/// as opposed to the active state in use by the compositor.
///
/// A commit request atomically creates a content update from the pending
/// state, even if the pending state has not been touched. The content
/// update is placed in a queue until it becomes active. After commit, the
/// new pending state is as documented for each related request.
///
/// When the content update is applied, the wl_buffer is applied before all
/// other state. This means that all coordinates in double-buffered state
/// are relative to the newly attached wl_buffers, except for
/// wl_surface.attach itself. If there is no newly attached wl_buffer, the
/// coordinates are relative to the previous content update.
///
/// All requests that need a commit to become effective are documented
/// to affect double-buffered state.
///
/// Other interfaces may add further double-buffered surface state.
struct wl_surface::request::commit {
    static constexpr Wopcode<wl_surface> opcode{ 6 };
};

/// sets the buffer transformation
///
/// This request sets the transformation that the client has already applied
/// to the content of the buffer. The accepted values for the transform
/// parameter are the values for wl_output.transform.
///
/// The compositor applies the inverse of this transformation whenever it
/// uses the buffer contents.
///
/// Buffer transform is double-buffered state, see wl_surface.commit.
///
/// A newly created surface has its buffer transformation set to normal.
///
/// wl_surface.set_buffer_transform changes the pending buffer
/// transformation. wl_surface.commit copies the pending buffer
/// transformation to the current one. Otherwise, the pending and current
/// values are never changed.
///
/// The purpose of this request is to allow clients to render content
/// according to the output transform, thus permitting the compositor to
/// use certain optimizations even if the display is rotated. Using
/// hardware overlays and scanning out a client buffer for fullscreen
/// surfaces are examples of such optimizations. Those optimizations are
/// highly dependent on the compositor implementation, so the use of this
/// request should be considered on a case-by-case basis.
///
/// Note that if the transform value includes 90 or 270 degree rotation,
/// the width of the buffer will become the surface height and the height
/// of the buffer will become the surface width.
///
/// If transform is not one of the values from the
/// wl_output.transform enum the invalid_transform protocol error
/// is raised.
struct wl_surface::request::set_buffer_transform {
    static constexpr Wopcode<wl_surface> opcode{ 7 };
    /// transform for interpreting buffer contents
    wl_output::transform transform;
};

/// sets the buffer scaling factor
///
/// This request sets an optional scaling factor on how the compositor
/// interprets the contents of the buffer attached to the window.
///
/// Buffer scale is double-buffered state, see wl_surface.commit.
///
/// A newly created surface has its buffer scale set to 1.
///
/// wl_surface.set_buffer_scale changes the pending buffer scale.
/// wl_surface.commit copies the pending buffer scale to the current one.
/// Otherwise, the pending and current values are never changed.
///
/// The purpose of this request is to allow clients to supply higher
/// resolution buffer data for use on high resolution outputs. It is
/// intended that you pick the same buffer scale as the scale of the
/// output that the surface is displayed on. This means the compositor
/// can avoid scaling when rendering the surface on that output.
///
/// Note that if the scale is larger than 1, then you have to attach
/// a buffer that is larger (by a factor of scale in each dimension)
/// than the desired surface size.
///
/// If scale is not greater than 0 the invalid_scale protocol error is
/// raised.
struct wl_surface::request::set_buffer_scale {
    static constexpr Wopcode<wl_surface> opcode{ 8 };
    /// scale for interpreting buffer contents
    Wint scale;
};

/// mark part of the surface damaged using buffer coordinates
///
/// This request is used to describe the regions where the pending
/// buffer is different from the current surface contents, and where
/// the surface therefore needs to be repainted. The compositor
/// ignores the parts of the damage that fall outside of the surface.
///
/// Damage is double-buffered state, see wl_surface.commit.
///
/// The damage rectangle is specified in buffer coordinates,
/// where x and y specify the upper left corner of the damage rectangle.
///
/// The initial value for pending damage is empty: no damage.
/// wl_surface.damage_buffer adds pending damage: the new pending
/// damage is the union of old pending damage and the given rectangle.
///
/// wl_surface.commit assigns pending damage as the current damage,
/// and clears pending damage. The server will clear the current
/// damage as it repaints the surface.
///
/// This request differs from wl_surface.damage in only one way - it
/// takes damage in buffer coordinates instead of surface-local
/// coordinates. While this generally is more intuitive than surface
/// coordinates, it is especially desirable when using wp_viewport
/// or when a drawing library (like EGL) is unaware of buffer scale
/// and buffer transform.
///
/// Note: Because buffer transformation changes and damage requests may
/// be interleaved in the protocol stream, it is impossible to determine
/// the actual mapping between surface and buffer damage until
/// wl_surface.commit time. Therefore, compositors wishing to take both
/// kinds of damage into account will have to accumulate damage from the
/// two requests separately and only transform from one to the other
/// after receiving the wl_surface.commit.
struct wl_surface::request::damage_buffer {
    static constexpr Wopcode<wl_surface> opcode{ 9 };
    /// buffer-local x coordinate
    Wint x;
    /// buffer-local y coordinate
    Wint y;
    /// width of damage rectangle
    Wint width;
    /// height of damage rectangle
    Wint height;
};

/// set the surface contents offset
///
/// The x and y arguments specify the location of the new pending
/// buffer's upper left corner, relative to the current buffer's upper
/// left corner, in surface-local coordinates. In other words, the
/// x and y, combined with the new surface size define in which
/// directions the surface's size changes.
///
/// The exact semantics of wl_surface.offset are role-specific. Refer to
/// the documentation of specific roles for more information.
///
/// Surface location offset is double-buffered state, see
/// wl_surface.commit.
///
/// This request is semantically equivalent to and the replaces the x and y
/// arguments in the wl_surface.attach request in wl_surface versions prior
/// to 5. See wl_surface.attach for details.
struct wl_surface::request::offset {
    static constexpr Wopcode<wl_surface> opcode{ 10 };
    /// surface-local x coordinate
    Wint x;
    /// surface-local y coordinate
    Wint y;
};

/// surface enters an output
///
/// This is emitted whenever a surface's creation, movement, or resizing
/// results in some part of it being within the scanout region of an
/// output.
///
/// Note that a surface may be overlapping with zero or more outputs.
struct wl_surface::event::enter {
    static constexpr Wopcode<wl_surface> opcode{ 0 };
    /// output entered by the surface
    Wobject<wl_output> output;
};

/// surface leaves an output
///
/// This is emitted whenever a surface's creation, movement, or resizing
/// results in it no longer having any part of it within the scanout region
/// of an output.
///
/// Clients should not use the number of outputs the surface is on for frame
/// throttling purposes. The surface might be hidden even if no leave event
/// has been sent, and the compositor might expect new surface content
/// updates even if no enter event has been sent. The frame event should be
/// used instead.
struct wl_surface::event::leave {
    static constexpr Wopcode<wl_surface> opcode{ 1 };
    /// output left by the surface
    Wobject<wl_output> output;
};

/// preferred buffer scale for the surface
///
/// This event indicates the preferred buffer scale for this surface. It is
/// sent whenever the compositor's preference changes.
///
/// Before receiving this event the preferred buffer scale for this surface
/// is 1.
///
/// It is intended that scaling aware clients use this event to scale their
/// content and use wl_surface.set_buffer_scale to indicate the scale they
/// have rendered with. This allows clients to supply a higher detail
/// buffer.
///
/// The compositor shall emit a scale value greater than 0.
struct wl_surface::event::preferred_buffer_scale {
    static constexpr Wopcode<wl_surface> opcode{ 2 };
    /// preferred scaling factor
    Wint factor;
};

/// preferred buffer transform for the surface
///
/// This event indicates the preferred buffer transform for this surface.
/// It is sent whenever the compositor's preference changes.
///
/// Before receiving this event the preferred buffer transform for this
/// surface is normal.
///
/// Applying this transformation to the surface buffer contents and using
/// wl_surface.set_buffer_transform might allow the compositor to use the
/// surface buffer more efficiently.
struct wl_surface::event::preferred_buffer_transform {
    static constexpr Wopcode<wl_surface> opcode{ 3 };
    /// preferred transform
    wl_output::transform transform;
};

/// seat capability bitmask
///
/// This is a bitmask of capabilities this seat has; if a member is
/// set, then it is present on the seat.
enum class wl_seat::capability : Wuint::integral_type {
    /// the seat has pointer devices
    Epointer = 1,
    /// the seat has one or more keyboards
    Ekeyboard = 2,
    /// the seat has touch devices
    Etouch = 4
};

/// wl_seat error values
///
/// These errors can be emitted in response to wl_seat requests.
enum class wl_seat::error : Wint::integral_type {
    /// get_pointer, get_keyboard or get_touch called on seat without the matching capability
    Emissing_capability = 0
};

/// return pointer object
///
/// The ID provided will be initialized to the wl_pointer interface
/// for this seat.
///
/// This request only takes effect if the seat has the pointer
/// capability, or has had the pointer capability in the past.
/// It is a protocol violation to issue this request on a seat that has
/// never had the pointer capability. The missing_capability error will
/// be sent in this case.
struct wl_seat::request::get_pointer {
    static constexpr Wopcode<wl_seat> opcode{ 0 };
    /// seat pointer
    Wnew_id<wl_pointer> id;
};

/// return keyboard object
///
/// The ID provided will be initialized to the wl_keyboard interface
/// for this seat.
///
/// This request only takes effect if the seat has the keyboard
/// capability, or has had the keyboard capability in the past.
/// It is a protocol violation to issue this request on a seat that has
/// never had the keyboard capability. The missing_capability error will
/// be sent in this case.
struct wl_seat::request::get_keyboard {
    static constexpr Wopcode<wl_seat> opcode{ 1 };
    /// seat keyboard
    Wnew_id<wl_keyboard> id;
};

/// return touch object
///
/// The ID provided will be initialized to the wl_touch interface
/// for this seat.
///
/// This request only takes effect if the seat has the touch
/// capability, or has had the touch capability in the past.
/// It is a protocol violation to issue this request on a seat that has
/// never had the touch capability. The missing_capability error will
/// be sent in this case.
struct wl_seat::request::get_touch {
    static constexpr Wopcode<wl_seat> opcode{ 2 };
    /// seat touch interface
    Wnew_id<wl_touch> id;
};

/// release the seat object
///
/// Using this request a client can tell the server that it is not going to
/// use the seat object anymore.
struct wl_seat::request::release {
    static constexpr Wopcode<wl_seat> opcode{ 3 };
};

/// seat capabilities changed
///
/// This is emitted whenever a seat gains or loses the pointer,
/// keyboard or touch capabilities.  The argument is a capability
/// enum containing the complete set of capabilities this seat has.
///
/// When the pointer capability is added, a client may create a
/// wl_pointer object using the wl_seat.get_pointer request. This object
/// will receive pointer events until the capability is removed in the
/// future.
///
/// When the pointer capability is removed, a client should destroy the
/// wl_pointer objects associated with the seat where the capability was
/// removed, using the wl_pointer.release request. No further pointer
/// events will be received on these objects.
///
/// In some compositors, if a seat regains the pointer capability and a
/// client has a previously obtained wl_pointer object of version 4 or
/// less, that object may start sending pointer events again. This
/// behavior is considered a misinterpretation of the intended behavior
/// and must not be relied upon by the client. wl_pointer objects of
/// version 5 or later must not send events if created before the most
/// recent event notifying the client of an added pointer capability.
///
/// The above behavior also applies to wl_keyboard and wl_touch with the
/// keyboard and touch capabilities, respectively.
struct wl_seat::event::capabilities {
    static constexpr Wopcode<wl_seat> opcode{ 0 };
    /// capabilities of the seat
    wl_seat::capability capabilities;
};

/// unique identifier for this seat
///
/// In a multi-seat configuration the seat name can be used by clients to
/// help identify which physical devices the seat represents.
///
/// The seat name is a UTF-8 string with no convention defined for its
/// contents. Each name is unique among all wl_seat globals. The name is
/// only guaranteed to be unique for the current compositor instance.
///
/// The same seat names are used for all clients. Thus, the name can be
/// shared across processes to refer to a specific wl_seat global.
///
/// The name event is sent after binding to the seat global. This event is
/// only sent once per seat object, and the name does not change over the
/// lifetime of the wl_seat global.
///
/// Compositors may re-use the same seat name if the wl_seat global is
/// destroyed and re-created later.
struct wl_seat::event::name {
    static constexpr Wopcode<wl_seat> opcode{ 1 };
    /// seat identifier
    Wstring name;
};

enum class wl_pointer::error : Wint::integral_type {
    /// given wl_surface has another role
    Erole = 0
};

/// physical button state
///
/// Describes the physical state of a button that produced the button
/// event.
enum class wl_pointer::button_state : Wint::integral_type {
    /// the button is not pressed
    Ereleased = 0,
    /// the button is pressed
    Epressed = 1
};

/// axis types
///
/// Describes the axis types of scroll events.
enum class wl_pointer::axis : Wint::integral_type {
    /// vertical axis
    Evertical_scroll = 0,
    /// horizontal axis
    Ehorizontal_scroll = 1
};

/// axis source types
///
/// Describes the source types for axis events. This indicates to the
/// client how an axis event was physically generated; a client may
/// adjust the user interface accordingly. For example, scroll events
/// from a "finger" source may be in a smooth coordinate space with
/// kinetic scrolling whereas a "wheel" source may be in discrete steps
/// of a number of lines.
///
/// The "continuous" axis source is a device generating events in a
/// continuous coordinate space, but using something other than a
/// finger. One example for this source is button-based scrolling where
/// the vertical motion of a device is converted to scroll events while
/// a button is held down.
///
/// The "wheel tilt" axis source indicates that the actual device is a
/// wheel but the scroll event is not caused by a rotation but a
/// (usually sideways) tilt of the wheel.
enum class wl_pointer::axis_source : Wint::integral_type {
    /// a physical wheel rotation
    Ewheel = 0,
    /// finger on a touch surface
    Efinger = 1,
    /// continuous coordinate space
    Econtinuous = 2,
    /// a physical wheel tilt
    Ewheel_tilt = 3
};

/// axis relative direction
///
/// This specifies the direction of the physical motion that caused a
/// wl_pointer.axis event, relative to the wl_pointer.axis direction.
enum class wl_pointer::axis_relative_direction : Wint::integral_type {
    /// physical motion matches axis direction
    Eidentical = 0,
    /// physical motion is the inverse of the axis direction
    Einverted = 1
};

/// set the pointer surface
///
/// Set the pointer surface, i.e., the surface that contains the
/// pointer image (cursor). This request gives the surface the role
/// of a cursor. If the surface already has another role, it raises
/// a protocol error.
///
/// The cursor actually changes only if the pointer
/// focus for this device is one of the requesting client's surfaces
/// or the surface parameter is the current pointer surface. If
/// there was a previous surface set with this request it is
/// replaced. If surface is NULL, the pointer image is hidden.
///
/// The parameters hotspot_x and hotspot_y define the position of
/// the pointer surface relative to the pointer location. Its
/// top-left corner is always at (x, y) - (hotspot_x, hotspot_y),
/// where (x, y) are the coordinates of the pointer location, in
/// surface-local coordinates.
///
/// On wl_surface.offset requests to the pointer surface, hotspot_x
/// and hotspot_y are decremented by the x and y parameters
/// passed to the request. The offset must be applied by
/// wl_surface.commit as usual.
///
/// The hotspot can also be updated by passing the currently set
/// pointer surface to this request with new values for hotspot_x
/// and hotspot_y.
///
/// The input region is ignored for wl_surfaces with the role of
/// a cursor. When the use as a cursor ends, the wl_surface is
/// unmapped.
///
/// The serial parameter must match the latest wl_pointer.enter
/// serial number sent to the client. Otherwise the request will be
/// ignored.
struct wl_pointer::request::set_cursor {
    static constexpr Wopcode<wl_pointer> opcode{ 0 };
    /// serial number of the enter event
    Wuint serial;
    /// pointer surface
    Wobject<wl_surface> surface;
    /// surface-local x coordinate
    Wint hotspot_x;
    /// surface-local y coordinate
    Wint hotspot_y;
};

/// release the pointer object
///
/// Using this request a client can tell the server that it is not going to
/// use the pointer object anymore.
///
/// This request destroys the pointer proxy object, so clients must not call
/// wl_pointer_destroy() after using this request.
struct wl_pointer::request::release {
    static constexpr Wopcode<wl_pointer> opcode{ 1 };
};

/// enter event
///
/// Notification that this seat's pointer is focused on a certain
/// surface.
///
/// When a seat's focus enters a surface, the pointer image
/// is undefined and a client should respond to this event by setting
/// an appropriate pointer image with the set_cursor request.
struct wl_pointer::event::enter {
    static constexpr Wopcode<wl_pointer> opcode{ 0 };
    /// serial number of the enter event
    Wuint serial;
    /// surface entered by the pointer
    Wobject<wl_surface> surface;
    /// surface-local x coordinate
    Wfixed surface_x;
    /// surface-local y coordinate
    Wfixed surface_y;
};

/// leave event
///
/// Notification that this seat's pointer is no longer focused on
/// a certain surface.
///
/// The leave notification is sent before the enter notification
/// for the new focus.
struct wl_pointer::event::leave {
    static constexpr Wopcode<wl_pointer> opcode{ 1 };
    /// serial number of the leave event
    Wuint serial;
    /// surface left by the pointer
    Wobject<wl_surface> surface;
};

/// pointer motion event
///
/// Notification of pointer location change. The arguments
/// surface_x and surface_y are the location relative to the
/// focused surface.
struct wl_pointer::event::motion {
    static constexpr Wopcode<wl_pointer> opcode{ 2 };
    /// timestamp with millisecond granularity
    Wuint time;
    /// surface-local x coordinate
    Wfixed surface_x;
    /// surface-local y coordinate
    Wfixed surface_y;
};

/// pointer button event
///
/// Mouse button click and release notifications.
///
/// The location of the click is given by the last motion or
/// enter event.
/// The time argument is a timestamp with millisecond
/// granularity, with an undefined base.
///
/// The button is a button code as defined in the Linux kernel's
/// linux/input-event-codes.h header file, e.g. BTN_LEFT.
///
/// Any 16-bit button code value is reserved for future additions to the
/// kernel's event code list. All other button codes above 0xFFFF are
/// currently undefined but may be used in future versions of this
/// protocol.
struct wl_pointer::event::button {
    static constexpr Wopcode<wl_pointer> opcode{ 3 };
    /// serial number of the button event
    Wuint serial;
    /// timestamp with millisecond granularity
    Wuint time;
    /// button that produced the event
    Wuint button;
    /// physical state of the button
    wl_pointer::button_state state;
};

/// axis event
///
/// Scroll and other axis notifications.
///
/// For scroll events (vertical and horizontal scroll axes), the
/// value parameter is the length of a vector along the specified
/// axis in a coordinate space identical to those of motion events,
/// representing a relative movement along the specified axis.
///
/// For devices that support movements non-parallel to axes multiple
/// axis events will be emitted.
///
/// When applicable, for example for touch pads, the server can
/// choose to emit scroll events where the motion vector is
/// equivalent to a motion event vector.
///
/// When applicable, a client can transform its content relative to the
/// scroll distance.
struct wl_pointer::event::axis {
    static constexpr Wopcode<wl_pointer> opcode{ 4 };
    /// timestamp with millisecond granularity
    Wuint time;
    /// axis type
    wl_pointer::axis axis;
    /// length of vector in surface-local coordinate space
    Wfixed value;
};

/// end of a pointer event sequence
///
/// Indicates the end of a set of events that logically belong together.
/// A client is expected to accumulate the data in all events within the
/// frame before proceeding.
///
/// All wl_pointer events before a wl_pointer.frame event belong
/// logically together. For example, in a diagonal scroll motion the
/// compositor will send an optional wl_pointer.axis_source event, two
/// wl_pointer.axis events (horizontal and vertical) and finally a
/// wl_pointer.frame event. The client may use this information to
/// calculate a diagonal vector for scrolling.
///
/// When multiple wl_pointer.axis events occur within the same frame,
/// the motion vector is the combined motion of all events.
/// When a wl_pointer.axis and a wl_pointer.axis_stop event occur within
/// the same frame, this indicates that axis movement in one axis has
/// stopped but continues in the other axis.
/// When multiple wl_pointer.axis_stop events occur within the same
/// frame, this indicates that these axes stopped in the same instance.
///
/// A wl_pointer.frame event is sent for every logical event group,
/// even if the group only contains a single wl_pointer event.
/// Specifically, a client may get a sequence: motion, frame, button,
/// frame, axis, frame, axis_stop, frame.
///
/// The wl_pointer.enter and wl_pointer.leave events are logical events
/// generated by the compositor and not the hardware. These events are
/// also grouped by a wl_pointer.frame. When a pointer moves from one
/// surface to another, a compositor should group the
/// wl_pointer.leave event within the same wl_pointer.frame.
/// However, a client must not rely on wl_pointer.leave and
/// wl_pointer.enter being in the same wl_pointer.frame.
/// Compositor-specific policies may require the wl_pointer.leave and
/// wl_pointer.enter event being split across multiple wl_pointer.frame
/// groups.
struct wl_pointer::event::frame {
    static constexpr Wopcode<wl_pointer> opcode{ 5 };
};

/// axis source event
///
/// Source information for scroll and other axes.
///
/// This event does not occur on its own. It is sent before a
/// wl_pointer.frame event and carries the source information for
/// all events within that frame.
///
/// The source specifies how this event was generated. If the source is
/// wl_pointer.axis_source.finger, a wl_pointer.axis_stop event will be
/// sent when the user lifts the finger off the device.
///
/// If the source is wl_pointer.axis_source.wheel,
/// wl_pointer.axis_source.wheel_tilt or
/// wl_pointer.axis_source.continuous, a wl_pointer.axis_stop event may
/// or may not be sent. Whether a compositor sends an axis_stop event
/// for these sources is hardware-specific and implementation-dependent;
/// clients must not rely on receiving an axis_stop event for these
/// scroll sources and should treat scroll sequences from these scroll
/// sources as unterminated by default.
///
/// This event is optional. If the source is unknown for a particular
/// axis event sequence, no event is sent.
/// Only one wl_pointer.axis_source event is permitted per frame.
///
/// The order of wl_pointer.axis_discrete and wl_pointer.axis_source is
/// not guaranteed.
struct wl_pointer::event::axis_source {
    static constexpr Wopcode<wl_pointer> opcode{ 6 };
    /// source of the axis event
    wl_pointer::axis_source axis_source;
};

/// axis stop event
///
/// Stop notification for scroll and other axes.
///
/// For some wl_pointer.axis_source types, a wl_pointer.axis_stop event
/// is sent to notify a client that the axis sequence has terminated.
/// This enables the client to implement kinetic scrolling.
/// See the wl_pointer.axis_source documentation for information on when
/// this event may be generated.
///
/// Any wl_pointer.axis events with the same axis_source after this
/// event should be considered as the start of a new axis motion.
///
/// The timestamp is to be interpreted identical to the timestamp in the
/// wl_pointer.axis event. The timestamp value may be the same as a
/// preceding wl_pointer.axis event.
struct wl_pointer::event::axis_stop {
    static constexpr Wopcode<wl_pointer> opcode{ 7 };
    /// timestamp with millisecond granularity
    Wuint time;
    /// the axis stopped with this event
    wl_pointer::axis axis;
};

/// axis click event
///
/// Discrete step information for scroll and other axes.
///
/// This event carries the axis value of the wl_pointer.axis event in
/// discrete steps (e.g. mouse wheel clicks).
///
/// This event is deprecated with wl_pointer version 8 - this event is not
/// sent to clients supporting version 8 or later.
///
/// This event does not occur on its own, it is coupled with a
/// wl_pointer.axis event that represents this axis value on a
/// continuous scale. The protocol guarantees that each axis_discrete
/// event is always followed by exactly one axis event with the same
/// axis number within the same wl_pointer.frame. Note that the protocol
/// allows for other events to occur between the axis_discrete and
/// its coupled axis event, including other axis_discrete or axis
/// events. A wl_pointer.frame must not contain more than one axis_discrete
/// event per axis type.
///
/// This event is optional; continuous scrolling devices
/// like two-finger scrolling on touchpads do not have discrete
/// steps and do not generate this event.
///
/// The discrete value carries the directional information. e.g. a value
/// of -2 is two steps towards the negative direction of this axis.
///
/// The axis number is identical to the axis number in the associated
/// axis event.
///
/// The order of wl_pointer.axis_discrete and wl_pointer.axis_source is
/// not guaranteed.
struct wl_pointer::event::axis_discrete {
    static constexpr Wopcode<wl_pointer> opcode{ 8 };
    /// axis type
    wl_pointer::axis axis;
    /// number of steps
    Wint discrete;
};

/// axis high-resolution scroll event
///
/// Discrete high-resolution scroll information.
///
/// This event carries high-resolution wheel scroll information,
/// with each multiple of 120 representing one logical scroll step
/// (a wheel detent). For example, an axis_value120 of 30 is one quarter of
/// a logical scroll step in the positive direction, a value120 of
/// -240 are two logical scroll steps in the negative direction within the
/// same hardware event.
/// Clients that rely on discrete scrolling should accumulate the
/// value120 to multiples of 120 before processing the event.
///
/// The value120 must not be zero.
///
/// This event replaces the wl_pointer.axis_discrete event in clients
/// supporting wl_pointer version 8 or later.
///
/// Where a wl_pointer.axis_source event occurs in the same
/// wl_pointer.frame, the axis source applies to this event.
///
/// The order of wl_pointer.axis_value120 and wl_pointer.axis_source is
/// not guaranteed.
struct wl_pointer::event::axis_value120 {
    static constexpr Wopcode<wl_pointer> opcode{ 9 };
    /// axis type
    wl_pointer::axis axis;
    /// scroll distance as fraction of 120
    Wint value120;
};

/// axis relative physical direction event
///
/// Relative directional information of the entity causing the axis
/// motion.
///
/// For a wl_pointer.axis event, the wl_pointer.axis_relative_direction
/// event specifies the movement direction of the entity causing the
/// wl_pointer.axis event. For example:
/// - if a user's fingers on a touchpad move down and this
/// causes a wl_pointer.axis vertical_scroll down event, the physical
/// direction is 'identical'
/// - if a user's fingers on a touchpad move down and this causes a
/// wl_pointer.axis vertical_scroll up scroll up event ('natural
/// scrolling'), the physical direction is 'inverted'.
///
/// A client may use this information to adjust scroll motion of
/// components. Specifically, enabling natural scrolling causes the
/// content to change direction compared to traditional scrolling.
/// Some widgets like volume control sliders should usually match the
/// physical direction regardless of whether natural scrolling is
/// active. This event enables clients to match the scroll direction of
/// a widget to the physical direction.
///
/// This event does not occur on its own, it is coupled with a
/// wl_pointer.axis event that represents this axis value.
/// The protocol guarantees that each axis_relative_direction event is
/// always followed by exactly one axis event with the same
/// axis number within the same wl_pointer.frame. Note that the protocol
/// allows for other events to occur between the axis_relative_direction
/// and its coupled axis event.
///
/// The axis number is identical to the axis number in the associated
/// axis event.
///
/// The order of wl_pointer.axis_relative_direction,
/// wl_pointer.axis_discrete and wl_pointer.axis_source is not
/// guaranteed.
struct wl_pointer::event::axis_relative_direction {
    static constexpr Wopcode<wl_pointer> opcode{ 10 };
    /// axis type
    wl_pointer::axis axis;
    /// physical direction relative to axis motion
    wl_pointer::axis_relative_direction direction;
};

/// keyboard mapping format
///
/// This specifies the format of the keymap provided to the
/// client with the wl_keyboard.keymap event.
enum class wl_keyboard::keymap_format : Wint::integral_type {
    /// no keymap; client must understand how to interpret the raw keycode
    Eno_keymap = 0,
    /// libxkbcommon compatible, null-terminated string; to determine the xkb keycode, clients must add 8 to the key event keycode
    Exkb_v1 = 1
};

/// physical key state
///
/// Describes the physical state of a key that produced the key event.
///
/// Since version 10, the key can be in a "repeated" pseudo-state which
/// means the same as "pressed", but is used to signal repetition in the
/// key event.
///
/// The key may only enter the repeated state after entering the pressed
/// state and before entering the released state. This event may be
/// generated multiple times while the key is down.
enum class wl_keyboard::key_state : Wint::integral_type {
    /// key is not pressed
    Ereleased = 0,
    /// key is pressed
    Epressed = 1,
    /// key was repeated
    Erepeated = 2
};

/// release the keyboard object
struct wl_keyboard::request::release {
    static constexpr Wopcode<wl_keyboard> opcode{ 0 };
};

/// keyboard mapping
///
/// This event provides a file descriptor to the client which can be
/// memory-mapped in read-only mode to provide a keyboard mapping
/// description.
///
/// From version 7 onwards, the fd must be mapped with MAP_PRIVATE by
/// the recipient, as MAP_SHARED may fail.
struct wl_keyboard::event::keymap {
    static constexpr Wopcode<wl_keyboard> opcode{ 0 };
    /// keymap format
    wl_keyboard::keymap_format format;
    /// keymap file descriptor
    Wfd fd;
    /// keymap size, in bytes
    Wuint size;
};

/// enter event
///
/// Notification that this seat's keyboard focus is on a certain
/// surface.
///
/// The compositor must send the wl_keyboard.modifiers event after this
/// event.
///
/// In the wl_keyboard logical state, this event sets the active surface to
/// the surface argument and the keys currently logically down to the keys
/// in the keys argument. The compositor must not send this event if the
/// wl_keyboard already had an active surface immediately before this event.
///
/// Clients should not use the list of pressed keys to emulate key-press
/// events. The order of keys in the list is unspecified.
struct wl_keyboard::event::enter {
    static constexpr Wopcode<wl_keyboard> opcode{ 1 };
    /// serial number of the enter event
    Wuint serial;
    /// surface gaining keyboard focus
    Wobject<wl_surface> surface;
    /// the keys currently logically down
    Warray keys;
};

/// leave event
///
/// Notification that this seat's keyboard focus is no longer on
/// a certain surface.
///
/// The leave notification is sent before the enter notification
/// for the new focus.
///
/// In the wl_keyboard logical state, this event resets all values to their
/// defaults. The compositor must not send this event if the active surface
/// of the wl_keyboard was not equal to the surface argument immediately
/// before this event.
struct wl_keyboard::event::leave {
    static constexpr Wopcode<wl_keyboard> opcode{ 2 };
    /// serial number of the leave event
    Wuint serial;
    /// surface that lost keyboard focus
    Wobject<wl_surface> surface;
};

/// key event
///
/// A key was pressed or released.
/// The time argument is a timestamp with millisecond
/// granularity, with an undefined base.
///
/// The key is a platform-specific key code that can be interpreted
/// by feeding it to the keyboard mapping (see the keymap event).
///
/// If this event produces a change in modifiers, then the resulting
/// wl_keyboard.modifiers event must be sent after this event.
///
/// In the wl_keyboard logical state, this event adds the key to the keys
/// currently logically down (if the state argument is pressed) or removes
/// the key from the keys currently logically down (if the state argument is
/// released). The compositor must not send this event if the wl_keyboard
/// did not have an active surface immediately before this event. The
/// compositor must not send this event if state is pressed (resp. released)
/// and the key was already logically down (resp. was not logically down)
/// immediately before this event.
///
/// Since version 10, compositors may send key events with the "repeated"
/// key state when a wl_keyboard.repeat_info event with a rate argument of
/// 0 has been received. This allows the compositor to take over the
/// responsibility of key repetition.
struct wl_keyboard::event::key {
    static constexpr Wopcode<wl_keyboard> opcode{ 3 };
    /// serial number of the key event
    Wuint serial;
    /// timestamp with millisecond granularity
    Wuint time;
    /// key that produced the event
    Wuint key;
    /// physical state of the key
    wl_keyboard::key_state state;
};

/// modifier and group state
///
/// Notifies clients that the modifier and/or group state has
/// changed, and it should update its local state.
///
/// The compositor may send this event without a surface of the client
/// having keyboard focus, for example to tie modifier information to
/// pointer focus instead. If a modifier event with pressed modifiers is sent
/// without a prior enter event, the client can assume the modifier state is
/// valid until it receives the next wl_keyboard.modifiers event. In order to
/// reset the modifier state again, the compositor can send a
/// wl_keyboard.modifiers event with no pressed modifiers.
///
/// In the wl_keyboard logical state, this event updates the modifiers and
/// group.
struct wl_keyboard::event::modifiers {
    static constexpr Wopcode<wl_keyboard> opcode{ 4 };
    /// serial number of the modifiers event
    Wuint serial;
    /// depressed modifiers
    Wuint mods_depressed;
    /// latched modifiers
    Wuint mods_latched;
    /// locked modifiers
    Wuint mods_locked;
    /// keyboard layout
    Wuint group;
};

/// repeat rate and delay
///
/// Informs the client about the keyboard's repeat rate and delay.
///
/// This event is sent as soon as the wl_keyboard object has been created,
/// and is guaranteed to be received by the client before any key press
/// event.
///
/// Negative values for either rate or delay are illegal. A rate of zero
/// will disable any repeating (regardless of the value of delay).
///
/// This event can be sent later on as well with a new value if necessary,
/// so clients should continue listening for the event past the creation
/// of wl_keyboard.
struct wl_keyboard::event::repeat_info {
    static constexpr Wopcode<wl_keyboard> opcode{ 5 };
    /// the rate of repeating keys in characters per second
    Wint rate;
    /// delay in milliseconds since key down until repeating starts
    Wint delay;
};

/// release the touch object
struct wl_touch::request::release {
    static constexpr Wopcode<wl_touch> opcode{ 0 };
};

/// touch down event and beginning of a touch sequence
///
/// A new touch point has appeared on the surface. This touch point is
/// assigned a unique ID. Future events from this touch point reference
/// this ID. The ID ceases to be valid after a touch up event and may be
/// reused in the future.
struct wl_touch::event::down {
    static constexpr Wopcode<wl_touch> opcode{ 0 };
    /// serial number of the touch down event
    Wuint serial;
    /// timestamp with millisecond granularity
    Wuint time;
    /// surface touched
    Wobject<wl_surface> surface;
    /// the unique ID of this touch point
    Wint id;
    /// surface-local x coordinate
    Wfixed x;
    /// surface-local y coordinate
    Wfixed y;
};

/// end of a touch event sequence
///
/// The touch point has disappeared. No further events will be sent for
/// this touch point and the touch point's ID is released and may be
/// reused in a future touch down event.
struct wl_touch::event::up {
    static constexpr Wopcode<wl_touch> opcode{ 1 };
    /// serial number of the touch up event
    Wuint serial;
    /// timestamp with millisecond granularity
    Wuint time;
    /// the unique ID of this touch point
    Wint id;
};

/// update of touch point coordinates
///
/// A touch point has changed coordinates.
struct wl_touch::event::motion {
    static constexpr Wopcode<wl_touch> opcode{ 2 };
    /// timestamp with millisecond granularity
    Wuint time;
    /// the unique ID of this touch point
    Wint id;
    /// surface-local x coordinate
    Wfixed x;
    /// surface-local y coordinate
    Wfixed y;
};

/// end of touch frame event
///
/// Indicates the end of a set of events that logically belong together.
/// A client is expected to accumulate the data in all events within the
/// frame before proceeding.
///
/// A wl_touch.frame terminates at least one event but otherwise no
/// guarantee is provided about the set of events within a frame. A client
/// must assume that any state not updated in a frame is unchanged from the
/// previously known state.
struct wl_touch::event::frame {
    static constexpr Wopcode<wl_touch> opcode{ 3 };
};

/// touch session cancelled
///
/// Sent if the compositor decides the touch stream is a global
/// gesture. No further events are sent to the clients from that
/// particular gesture. Touch cancellation applies to all touch points
/// currently active on this client's surface. The client is
/// responsible for finalizing the touch points, future touch points on
/// this surface may reuse the touch point ID.
///
/// No frame event is required after the cancel event.
struct wl_touch::event::cancel {
    static constexpr Wopcode<wl_touch> opcode{ 4 };
};

/// update shape of touch point
///
/// Sent when a touchpoint has changed its shape.
///
/// This event does not occur on its own. It is sent before a
/// wl_touch.frame event and carries the new shape information for
/// any previously reported, or new touch points of that frame.
///
/// Other events describing the touch point such as wl_touch.down,
/// wl_touch.motion or wl_touch.orientation may be sent within the
/// same wl_touch.frame. A client should treat these events as a single
/// logical touch point update. The order of wl_touch.shape,
/// wl_touch.orientation and wl_touch.motion is not guaranteed.
/// A wl_touch.down event is guaranteed to occur before the first
/// wl_touch.shape event for this touch ID but both events may occur within
/// the same wl_touch.frame.
///
/// A touchpoint shape is approximated by an ellipse through the major and
/// minor axis length. The major axis length describes the longer diameter
/// of the ellipse, while the minor axis length describes the shorter
/// diameter. Major and minor are orthogonal and both are specified in
/// surface-local coordinates. The center of the ellipse is always at the
/// touchpoint location as reported by wl_touch.down or wl_touch.move.
///
/// This event is only sent by the compositor if the touch device supports
/// shape reports. The client has to make reasonable assumptions about the
/// shape if it did not receive this event.
struct wl_touch::event::shape {
    static constexpr Wopcode<wl_touch> opcode{ 5 };
    /// the unique ID of this touch point
    Wint id;
    /// length of the major axis in surface-local coordinates
    Wfixed major;
    /// length of the minor axis in surface-local coordinates
    Wfixed minor;
};

/// update orientation of touch point
///
/// Sent when a touchpoint has changed its orientation.
///
/// This event does not occur on its own. It is sent before a
/// wl_touch.frame event and carries the new shape information for
/// any previously reported, or new touch points of that frame.
///
/// Other events describing the touch point such as wl_touch.down,
/// wl_touch.motion or wl_touch.shape may be sent within the
/// same wl_touch.frame. A client should treat these events as a single
/// logical touch point update. The order of wl_touch.shape,
/// wl_touch.orientation and wl_touch.motion is not guaranteed.
/// A wl_touch.down event is guaranteed to occur before the first
/// wl_touch.orientation event for this touch ID but both events may occur
/// within the same wl_touch.frame.
///
/// The orientation describes the clockwise angle of a touchpoint's major
/// axis to the positive surface y-axis and is normalized to the -180 to
/// +180 degree range. The granularity of orientation depends on the touch
/// device, some devices only support binary rotation values between 0 and
/// 90 degrees.
///
/// This event is only sent by the compositor if the touch device supports
/// orientation reports.
struct wl_touch::event::orientation {
    static constexpr Wopcode<wl_touch> opcode{ 6 };
    /// the unique ID of this touch point
    Wint id;
    /// angle between major axis and positive surface y-axis in degrees
    Wfixed orientation;
};

/// subpixel geometry information
///
/// This enumeration describes how the physical
/// pixels on an output are laid out.
enum class wl_output::subpixel : Wint::integral_type {
    /// unknown geometry
    Eunknown = 0,
    /// no geometry
    Enone = 1,
    /// horizontal RGB
    Ehorizontal_rgb = 2,
    /// horizontal BGR
    Ehorizontal_bgr = 3,
    /// vertical RGB
    Evertical_rgb = 4,
    /// vertical BGR
    Evertical_bgr = 5
};

/// transformation applied to buffer contents
///
/// This describes transformations that clients and compositors apply to
/// buffer contents.
///
/// The flipped values correspond to an initial flip around a
/// vertical axis followed by rotation.
///
/// The purpose is mainly to allow clients to render accordingly and
/// tell the compositor, so that for fullscreen surfaces, the
/// compositor will still be able to scan out directly from client
/// surfaces.
enum class wl_output::transform : Wint::integral_type {
    /// no transform
    Enormal = 0,
    /// 90 degrees counter-clockwise
    E90 = 1,
    /// 180 degrees counter-clockwise
    E180 = 2,
    /// 270 degrees counter-clockwise
    E270 = 3,
    /// 180 degree flip around a vertical axis
    Eflipped = 4,
    /// flip and rotate 90 degrees counter-clockwise
    Eflipped_90 = 5,
    /// flip and rotate 180 degrees counter-clockwise
    Eflipped_180 = 6,
    /// flip and rotate 270 degrees counter-clockwise
    Eflipped_270 = 7
};

/// mode information
///
/// These flags describe properties of an output mode.
/// They are used in the flags bitfield of the mode event.
enum class wl_output::mode : Wuint::integral_type {
    /// indicates this is the current mode
    Ecurrent = 1,
    /// indicates this is the preferred mode
    Epreferred = 2
};

/// release the output object
///
/// Using this request a client can tell the server that it is not going to
/// use the output object anymore.
struct wl_output::request::release {
    static constexpr Wopcode<wl_output> opcode{ 0 };
};

/// properties of the output
///
/// The geometry event describes geometric properties of the output.
/// The event is sent when binding to the output object and whenever
/// any of the properties change.
///
/// The physical size can be set to zero if it doesn't make sense for this
/// output (e.g. for projectors or virtual outputs).
///
/// The geometry event will be followed by a done event (starting from
/// version 2).
///
/// Clients should use wl_surface.preferred_buffer_transform instead of the
/// transform advertised by this event to find the preferred buffer
/// transform to use for a surface.
///
/// Note: wl_output only advertises partial information about the output
/// position and identification. Some compositors, for instance those not
/// implementing a desktop-style output layout or those exposing virtual
/// outputs, might fake this information. Instead of using x and y, clients
/// should use xdg_output.logical_position. Instead of using make and model,
/// clients should use name and description.
struct wl_output::event::geometry {
    static constexpr Wopcode<wl_output> opcode{ 0 };
    /// x position within the global compositor space
    Wint x;
    /// y position within the global compositor space
    Wint y;
    /// width in millimeters of the output
    Wint physical_width;
    /// height in millimeters of the output
    Wint physical_height;
    /// subpixel orientation of the output
    wl_output::subpixel subpixel;
    /// textual description of the manufacturer
    Wstring make;
    /// textual description of the model
    Wstring model;
    /// additional transformation applied to buffer contents during presentation
    wl_output::transform transform;
};

/// advertise available modes for the output
///
/// The mode event describes an available mode for the output.
///
/// The event is sent when binding to the output object and there
/// will always be one mode, the current mode.  The event is sent
/// again if an output changes mode, for the mode that is now
/// current.  In other words, the current mode is always the last
/// mode that was received with the current flag set.
///
/// Non-current modes are deprecated. A compositor can decide to only
/// advertise the current mode and never send other modes. Clients
/// should not rely on non-current modes.
///
/// The size of a mode is given in physical hardware units of
/// the output device. This is not necessarily the same as
/// the output size in the global compositor space. For instance,
/// the output may be scaled, as described in wl_output.scale,
/// or transformed, as described in wl_output.transform. Clients
/// willing to retrieve the output size in the global compositor
/// space should use xdg_output.logical_size instead.
///
/// The vertical refresh rate can be set to zero if it doesn't make
/// sense for this output (e.g. for virtual outputs).
///
/// The mode event will be followed by a done event (starting from
/// version 2).
///
/// Clients should not use the refresh rate to schedule frames. Instead,
/// they should use the wl_surface.frame event or the presentation-time
/// protocol.
///
/// Note: this information is not always meaningful for all outputs. Some
/// compositors, such as those exposing virtual outputs, might fake the
/// refresh rate or the size.
struct wl_output::event::mode {
    static constexpr Wopcode<wl_output> opcode{ 1 };
    /// bitfield of mode flags
    wl_output::mode flags;
    /// width of the mode in hardware units
    Wint width;
    /// height of the mode in hardware units
    Wint height;
    /// vertical refresh rate in mHz
    Wint refresh;
};

/// sent all information about output
///
/// This event is sent after all other properties have been
/// sent after binding to the output object and after any
/// other property changes done after that. This allows
/// changes to the output properties to be seen as
/// atomic, even if they happen via multiple events.
struct wl_output::event::done {
    static constexpr Wopcode<wl_output> opcode{ 2 };
};

/// output scaling properties
///
/// This event contains scaling geometry information
/// that is not in the geometry event. It may be sent after
/// binding the output object or if the output scale changes
/// later. The compositor will emit a non-zero, positive
/// value for scale. If it is not sent, the client should
/// assume a scale of 1.
///
/// A scale larger than 1 means that the compositor will
/// automatically scale surface buffers by this amount
/// when rendering. This is used for very high resolution
/// displays where applications rendering at the native
/// resolution would be too small to be legible.
///
/// Clients should use wl_surface.preferred_buffer_scale
/// instead of this event to find the preferred buffer
/// scale to use for a surface.
///
/// The scale event will be followed by a done event.
struct wl_output::event::scale {
    static constexpr Wopcode<wl_output> opcode{ 3 };
    /// scaling factor of output
    Wint factor;
};

/// name of this output
///
/// Many compositors will assign user-friendly names to their outputs, show
/// them to the user, allow the user to refer to an output, etc. The client
/// may wish to know this name as well to offer the user similar behaviors.
///
/// The name is a UTF-8 string with no convention defined for its contents.
/// Each name is unique among all wl_output globals. The name is only
/// guaranteed to be unique for the compositor instance.
///
/// The same output name is used for all clients for a given wl_output
/// global. Thus, the name can be shared across processes to refer to a
/// specific wl_output global.
///
/// The name is not guaranteed to be persistent across sessions, thus cannot
/// be used to reliably identify an output in e.g. configuration files.
///
/// Examples of names include 'HDMI-A-1', 'WL-1', 'X11-1', etc. However, do
/// not assume that the name is a reflection of an underlying DRM connector,
/// X11 connection, etc.
///
/// The name event is sent after binding the output object. This event is
/// only sent once per output object, and the name does not change over the
/// lifetime of the wl_output global.
///
/// Compositors may re-use the same output name if the wl_output global is
/// destroyed and re-created later. Compositors should avoid re-using the
/// same name if possible.
///
/// The name event will be followed by a done event.
struct wl_output::event::name {
    static constexpr Wopcode<wl_output> opcode{ 4 };
    /// output name
    Wstring name;
};

/// human-readable description of this output
///
/// Many compositors can produce human-readable descriptions of their
/// outputs. The client may wish to know this description as well, e.g. for
/// output selection purposes.
///
/// The description is a UTF-8 string with no convention defined for its
/// contents. The description is not guaranteed to be unique among all
/// wl_output globals. Examples might include 'Foocorp 11" Display' or
/// 'Virtual X11 output via :1'.
///
/// The description event is sent after binding the output object and
/// whenever the description changes. The description is optional, and may
/// not be sent at all.
///
/// The description event will be followed by a done event.
struct wl_output::event::description {
    static constexpr Wopcode<wl_output> opcode{ 5 };
    /// output description
    Wstring description;
};

/// destroy region
///
/// Destroy the region.  This will invalidate the object ID.
struct wl_region::request::destroy {
    static constexpr Wopcode<wl_region> opcode{ 0 };
};

/// add rectangle to region
///
/// Add the specified rectangle to the region.
struct wl_region::request::add {
    static constexpr Wopcode<wl_region> opcode{ 1 };
    /// region-local x coordinate
    Wint x;
    /// region-local y coordinate
    Wint y;
    /// rectangle width
    Wint width;
    /// rectangle height
    Wint height;
};

/// subtract rectangle from region
///
/// Subtract the specified rectangle from the region.
struct wl_region::request::subtract {
    static constexpr Wopcode<wl_region> opcode{ 2 };
    /// region-local x coordinate
    Wint x;
    /// region-local y coordinate
    Wint y;
    /// rectangle width
    Wint width;
    /// rectangle height
    Wint height;
};

enum class wl_subcompositor::error : Wint::integral_type {
    /// the to-be sub-surface is invalid
    Ebad_surface = 0,
    /// the to-be sub-surface parent is invalid
    Ebad_parent = 1
};

/// unbind from the subcompositor interface
///
/// Informs the server that the client will not be using this
/// protocol object anymore. This does not affect any other
/// objects, wl_subsurface objects included.
struct wl_subcompositor::request::destroy {
    static constexpr Wopcode<wl_subcompositor> opcode{ 0 };
};

/// give a surface the role sub-surface
///
/// Create a sub-surface interface for the given surface, and
/// associate it with the given parent surface. This turns a
/// plain wl_surface into a sub-surface.
///
/// The to-be sub-surface must not already have another role, and it
/// must not have an existing wl_subsurface object. Otherwise the
/// bad_surface protocol error is raised.
///
/// Adding sub-surfaces to a parent is a double-buffered operation on the
/// parent (see wl_surface.commit). The effect of adding a sub-surface
/// becomes visible on the next time the state of the parent surface is
/// applied.
///
/// The parent surface must not be one of the child surface's descendants,
/// and the parent must be different from the child surface, otherwise the
/// bad_parent protocol error is raised.
///
/// This request modifies the behaviour of wl_surface.commit request on
/// the sub-surface, see the documentation on wl_subsurface interface.
struct wl_subcompositor::request::get_subsurface {
    static constexpr Wopcode<wl_subcompositor> opcode{ 1 };
    /// the new sub-surface object ID
    Wnew_id<wl_subsurface> id;
    /// the surface to be turned into a sub-surface
    Wobject<wl_surface> surface;
    /// the parent surface
    Wobject<wl_surface> parent;
};

enum class wl_subsurface::error : Wint::integral_type {
    /// wl_surface is not a sibling or the parent
    Ebad_surface = 0
};

/// remove sub-surface interface
///
/// The sub-surface interface is removed from the wl_surface object
/// that was turned into a sub-surface with a
/// wl_subcompositor.get_subsurface request. The wl_surface's association
/// to the parent is deleted. The wl_surface is unmapped immediately.
struct wl_subsurface::request::destroy {
    static constexpr Wopcode<wl_subsurface> opcode{ 0 };
};

/// reposition the sub-surface
///
/// This schedules a sub-surface position change.
/// The sub-surface will be moved so that its origin (top left
/// corner pixel) will be at the location x, y of the parent surface
/// coordinate system. The coordinates are not restricted to the parent
/// surface area. Negative values are allowed.
///
/// The scheduled coordinates will take effect whenever the state of the
/// parent surface is applied.
///
/// If more than one set_position request is invoked by the client before
/// the commit of the parent surface, the position of a new request always
/// replaces the scheduled position from any previous request.
///
/// The initial position is 0, 0.
struct wl_subsurface::request::set_position {
    static constexpr Wopcode<wl_subsurface> opcode{ 1 };
    /// x coordinate in the parent surface
    Wint x;
    /// y coordinate in the parent surface
    Wint y;
};

/// restack the sub-surface
///
/// This sub-surface is taken from the stack, and put back just
/// above the reference surface, changing the z-order of the sub-surfaces.
/// The reference surface must be one of the sibling surfaces, or the
/// parent surface. Using any other surface, including this sub-surface,
/// will cause a protocol error.
///
/// The z-order is double-buffered. Requests are handled in order and
/// applied immediately to a pending state. The final pending state is
/// copied to the active state the next time the state of the parent
/// surface is applied.
///
/// A new sub-surface is initially added as the top-most in the stack
/// of its siblings and parent.
struct wl_subsurface::request::place_above {
    static constexpr Wopcode<wl_subsurface> opcode{ 2 };
    /// the reference surface
    Wobject<wl_surface> sibling;
};

/// restack the sub-surface
///
/// The sub-surface is placed just below the reference surface.
/// See wl_subsurface.place_above.
struct wl_subsurface::request::place_below {
    static constexpr Wopcode<wl_subsurface> opcode{ 3 };
    /// the reference surface
    Wobject<wl_surface> sibling;
};

/// set sub-surface to synchronized mode
///
/// Change the commit behaviour of the sub-surface to synchronized
/// mode, also described as the parent dependent mode.
///
/// In synchronized mode, wl_surface.commit on a sub-surface will
/// accumulate the committed state in a cache, but the state will
/// not be applied and hence will not change the compositor output.
/// The cached state is applied to the sub-surface immediately after
/// the parent surface's state is applied. This ensures atomic
/// updates of the parent and all its synchronized sub-surfaces.
/// Applying the cached state will invalidate the cache, so further
/// parent surface commits do not (re-)apply old state.
///
/// See wl_subsurface for the recursive effect of this mode.
struct wl_subsurface::request::set_sync {
    static constexpr Wopcode<wl_subsurface> opcode{ 4 };
};

/// set sub-surface to desynchronized mode
///
/// Change the commit behaviour of the sub-surface to desynchronized
/// mode, also described as independent or freely running mode.
///
/// In desynchronized mode, wl_surface.commit on a sub-surface will
/// apply the pending state directly, without caching, as happens
/// normally with a wl_surface. Calling wl_surface.commit on the
/// parent surface has no effect on the sub-surface's wl_surface
/// state. This mode allows a sub-surface to be updated on its own.
///
/// If cached state exists when wl_surface.commit is called in
/// desynchronized mode, the pending state is added to the cached
/// state, and applied as a whole. This invalidates the cache.
///
/// Note: even if a sub-surface is set to desynchronized, a parent
/// sub-surface may override it to behave as synchronized. For details,
/// see wl_subsurface.
///
/// If a surface's parent surface behaves as desynchronized, then
/// the cached state is applied on set_desync.
struct wl_subsurface::request::set_desync {
    static constexpr Wopcode<wl_subsurface> opcode{ 5 };
};

/// destroys this object
struct wl_fixes::request::destroy {
    static constexpr Wopcode<wl_fixes> opcode{ 0 };
};

/// destroy a wl_registry
///
/// This request destroys a wl_registry object.
///
/// The client should no longer use the wl_registry after making this
/// request.
///
/// The compositor will emit a wl_display.delete_id event with the object ID
/// of the registry and will no longer emit any events on the registry. The
/// client should re-use the object ID once it receives the
/// wl_display.delete_id event.
struct wl_fixes::request::destroy_registry {
    static constexpr Wopcode<wl_fixes> opcode{ 1 };
    /// the registry to destroy
    Wobject<wl_registry> registry;
};

} // namespace protocols
} // namespace wl
} // namespace ger
