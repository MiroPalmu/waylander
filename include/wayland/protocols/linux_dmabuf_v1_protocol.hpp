// Generated from Wayland xml protocol: linux_dmabuf_v1

/// @file linux_dmabuf_v1 xml protocol

/// Copyright © 2014, 2015 Collabora, Ltd.
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

struct zwp_linux_dmabuf_v1 {
    struct request {
        struct destroy;
        struct create_params;
        struct get_default_feedback;
        struct get_surface_feedback;
    };

    struct event {
        struct format;
        struct modifier;
    };
};
struct zwp_linux_buffer_params_v1 {
    enum class error : Wint::integral_type;
    enum class flags : Wuint::integral_type;

    struct request {
        struct destroy;
        struct add;
        struct create;
        struct create_immed;
    };

    struct event {
        struct created;
        struct failed;
    };
};
struct zwp_linux_dmabuf_feedback_v1 {
    enum class tranche_flags : Wuint::integral_type;

    struct request {
        struct destroy;
    };

    struct event {
        struct done;
        struct format_table;
        struct main_device;
        struct tranche_done;
        struct tranche_target_device;
        struct tranche_formats;
        struct tranche_flags;
    };
};
/// unbind the factory
///
/// Objects created through this interface, especially wl_buffers, will
/// remain valid.
struct zwp_linux_dmabuf_v1::request::destroy {
    static constexpr Wopcode<zwp_linux_dmabuf_v1> opcode{ 0 };
};

/// create a temporary object for buffer parameters
///
/// This temporary object is used to collect multiple dmabuf handles into
/// a single batch to create a wl_buffer. It can only be used once and
/// should be destroyed after a 'created' or 'failed' event has been
/// received.
struct zwp_linux_dmabuf_v1::request::create_params {
    static constexpr Wopcode<zwp_linux_dmabuf_v1> opcode{ 1 };
    /// the new temporary
    Wnew_id<zwp_linux_buffer_params_v1> params_id;
};

/// get default feedback
///
/// This request creates a new wp_linux_dmabuf_feedback object not bound
/// to a particular surface. This object will deliver feedback about dmabuf
/// parameters to use if the client doesn't support per-surface feedback
/// (see get_surface_feedback).
struct zwp_linux_dmabuf_v1::request::get_default_feedback {
    static constexpr Wopcode<zwp_linux_dmabuf_v1> opcode{ 2 };
    Wnew_id<zwp_linux_dmabuf_feedback_v1> id;
};

/// get feedback for a surface
///
/// This request creates a new wp_linux_dmabuf_feedback object for the
/// specified wl_surface. This object will deliver feedback about dmabuf
/// parameters to use for buffers attached to this surface.
///
/// If the surface is destroyed before the wp_linux_dmabuf_feedback object,
/// the feedback object becomes inert.
struct zwp_linux_dmabuf_v1::request::get_surface_feedback {
    static constexpr Wopcode<zwp_linux_dmabuf_v1> opcode{ 3 };
    Wnew_id<zwp_linux_dmabuf_feedback_v1> id;
    Wobject<wl_surface> surface;
};

/// supported buffer format
///
/// This event advertises one buffer format that the server supports.
/// All the supported formats are advertised once when the client
/// binds to this interface. A roundtrip after binding guarantees
/// that the client has received all supported formats.
///
/// For the definition of the format codes, see the
/// zwp_linux_buffer_params_v1::create request.
///
/// Starting version 4, the format event is deprecated and must not be
/// sent by compositors. Instead, use get_default_feedback or
/// get_surface_feedback.
struct zwp_linux_dmabuf_v1::event::format {
    static constexpr Wopcode<zwp_linux_dmabuf_v1> opcode{ 0 };
    /// DRM_FORMAT code
    Wuint format;
};

/// supported buffer format modifier
///
/// This event advertises the formats that the server supports, along with
/// the modifiers supported for each format. All the supported modifiers
/// for all the supported formats are advertised once when the client
/// binds to this interface. A roundtrip after binding guarantees that
/// the client has received all supported format-modifier pairs.
///
/// For legacy support, DRM_FORMAT_MOD_INVALID (that is, modifier_hi ==
/// 0x00ffffff and modifier_lo == 0xffffffff) is allowed in this event.
/// It indicates that the server can support the format with an implicit
/// modifier. When a plane has DRM_FORMAT_MOD_INVALID as its modifier, it
/// is as if no explicit modifier is specified. The effective modifier
/// will be derived from the dmabuf.
///
/// A compositor that sends valid modifiers and DRM_FORMAT_MOD_INVALID for
/// a given format supports both explicit modifiers and implicit modifiers.
///
/// For the definition of the format and modifier codes, see the
/// zwp_linux_buffer_params_v1::create and zwp_linux_buffer_params_v1::add
/// requests.
///
/// Starting version 4, the modifier event is deprecated and must not be
/// sent by compositors. Instead, use get_default_feedback or
/// get_surface_feedback.
struct zwp_linux_dmabuf_v1::event::modifier {
    static constexpr Wopcode<zwp_linux_dmabuf_v1> opcode{ 1 };
    /// DRM_FORMAT code
    Wuint format;
    /// high 32 bits of layout modifier
    Wuint modifier_hi;
    /// low 32 bits of layout modifier
    Wuint modifier_lo;
};

enum class zwp_linux_buffer_params_v1::error : Wint::integral_type {
    /// the dmabuf_batch object has already been used to create a wl_buffer
    Ealready_used = 0,
    /// plane index out of bounds
    Eplane_idx = 1,
    /// the plane index was already set
    Eplane_set = 2,
    /// missing or too many planes to create a buffer
    Eincomplete = 3,
    /// format not supported
    Einvalid_format = 4,
    /// invalid width or height
    Einvalid_dimensions = 5,
    /// offset + stride * height goes out of dmabuf bounds
    Eout_of_bounds = 6,
    /// invalid wl_buffer resulted from importing dmabufs via                the create_immed request on given buffer_params
    Einvalid_wl_buffer = 7
};

enum class zwp_linux_buffer_params_v1::flags : Wuint::integral_type {
    /// contents are y-inverted
    Ey_invert = 1,
    /// content is interlaced
    Einterlaced = 2,
    /// bottom field first
    Ebottom_first = 4
};

/// delete this object, used or not
///
/// Cleans up the temporary data sent to the server for dmabuf-based
/// wl_buffer creation.
struct zwp_linux_buffer_params_v1::request::destroy {
    static constexpr Wopcode<zwp_linux_buffer_params_v1> opcode{ 0 };
};

/// add a dmabuf to the temporary set
///
/// This request adds one dmabuf to the set in this
/// zwp_linux_buffer_params_v1.
///
/// The 64-bit unsigned value combined from modifier_hi and modifier_lo
/// is the dmabuf layout modifier. DRM AddFB2 ioctl calls this the
/// fb modifier, which is defined in drm_mode.h of Linux UAPI.
/// This is an opaque token. Drivers use this token to express tiling,
/// compression, etc. driver-specific modifications to the base format
/// defined by the DRM fourcc code.
///
/// Starting from version 4, the invalid_format protocol error is sent if
/// the format + modifier pair was not advertised as supported.
///
/// Starting from version 5, the invalid_format protocol error is sent if
/// all planes don't use the same modifier.
///
/// This request raises the PLANE_IDX error if plane_idx is too large.
/// The error PLANE_SET is raised if attempting to set a plane that
/// was already set.
struct zwp_linux_buffer_params_v1::request::add {
    static constexpr Wopcode<zwp_linux_buffer_params_v1> opcode{ 1 };
    /// dmabuf fd
    Wfd fd;
    /// plane index
    Wuint plane_idx;
    /// offset in bytes
    Wuint offset;
    /// stride in bytes
    Wuint stride;
    /// high 32 bits of layout modifier
    Wuint modifier_hi;
    /// low 32 bits of layout modifier
    Wuint modifier_lo;
};

/// create a wl_buffer from the given dmabufs
///
/// This asks for creation of a wl_buffer from the added dmabuf
/// buffers. The wl_buffer is not created immediately but returned via
/// the 'created' event if the dmabuf sharing succeeds. The sharing
/// may fail at runtime for reasons a client cannot predict, in
/// which case the 'failed' event is triggered.
///
/// The 'format' argument is a DRM_FORMAT code, as defined by the
/// libdrm's drm_fourcc.h. The Linux kernel's DRM sub-system is the
/// authoritative source on how the format codes should work.
///
/// The 'flags' is a bitfield of the flags defined in enum "flags".
/// 'y_invert' means the that the image needs to be y-flipped.
///
/// Flag 'interlaced' means that the frame in the buffer is not
/// progressive as usual, but interlaced. An interlaced buffer as
/// supported here must always contain both top and bottom fields.
/// The top field always begins on the first pixel row. The temporal
/// ordering between the two fields is top field first, unless
/// 'bottom_first' is specified. It is undefined whether 'bottom_first'
/// is ignored if 'interlaced' is not set.
///
/// This protocol does not convey any information about field rate,
/// duration, or timing, other than the relative ordering between the
/// two fields in one buffer. A compositor may have to estimate the
/// intended field rate from the incoming buffer rate. It is undefined
/// whether the time of receiving wl_surface.commit with a new buffer
/// attached, applying the wl_surface state, wl_surface.frame callback
/// trigger, presentation, or any other point in the compositor cycle
/// is used to measure the frame or field times. There is no support
/// for detecting missed or late frames/fields/buffers either, and
/// there is no support whatsoever for cooperating with interlaced
/// compositor output.
///
/// The composited image quality resulting from the use of interlaced
/// buffers is explicitly undefined. A compositor may use elaborate
/// hardware features or software to deinterlace and create progressive
/// output frames from a sequence of interlaced input buffers, or it
/// may produce substandard image quality. However, compositors that
/// cannot guarantee reasonable image quality in all cases are recommended
/// to just reject all interlaced buffers.
///
/// Any argument errors, including non-positive width or height,
/// mismatch between the number of planes and the format, bad
/// format, bad offset or stride, may be indicated by fatal protocol
/// errors: INCOMPLETE, INVALID_FORMAT, INVALID_DIMENSIONS,
/// OUT_OF_BOUNDS.
///
/// Dmabuf import errors in the server that are not obvious client
/// bugs are returned via the 'failed' event as non-fatal. This
/// allows attempting dmabuf sharing and falling back in the client
/// if it fails.
///
/// This request can be sent only once in the object's lifetime, after
/// which the only legal request is destroy. This object should be
/// destroyed after issuing a 'create' request. Attempting to use this
/// object after issuing 'create' raises ALREADY_USED protocol error.
///
/// It is not mandatory to issue 'create'. If a client wants to
/// cancel the buffer creation, it can just destroy this object.
struct zwp_linux_buffer_params_v1::request::create {
    static constexpr Wopcode<zwp_linux_buffer_params_v1> opcode{ 2 };
    /// base plane width in pixels
    Wint width;
    /// base plane height in pixels
    Wint height;
    /// DRM_FORMAT code
    Wuint format;
    /// see enum flags
    zwp_linux_buffer_params_v1::flags flags;
};

/// immediately create a wl_buffer from the given                      dmabufs
///
/// This asks for immediate creation of a wl_buffer by importing the
/// added dmabufs.
///
/// In case of import success, no event is sent from the server, and the
/// wl_buffer is ready to be used by the client.
///
/// Upon import failure, either of the following may happen, as seen fit
/// by the implementation:
/// - the client is terminated with one of the following fatal protocol
/// errors:
/// - INCOMPLETE, INVALID_FORMAT, INVALID_DIMENSIONS, OUT_OF_BOUNDS,
/// in case of argument errors such as mismatch between the number
/// of planes and the format, bad format, non-positive width or
/// height, or bad offset or stride.
/// - INVALID_WL_BUFFER, in case the cause for failure is unknown or
/// platform specific.
/// - the server creates an invalid wl_buffer, marks it as failed and
/// sends a 'failed' event to the client. The result of using this
/// invalid wl_buffer as an argument in any request by the client is
/// defined by the compositor implementation.
///
/// This takes the same arguments as a 'create' request, and obeys the
/// same restrictions.
struct zwp_linux_buffer_params_v1::request::create_immed {
    static constexpr Wopcode<zwp_linux_buffer_params_v1> opcode{ 3 };
    /// id for the newly created wl_buffer
    Wnew_id<wl_buffer> buffer_id;
    /// base plane width in pixels
    Wint width;
    /// base plane height in pixels
    Wint height;
    /// DRM_FORMAT code
    Wuint format;
    /// see enum flags
    zwp_linux_buffer_params_v1::flags flags;
};

/// buffer creation succeeded
///
/// This event indicates that the attempted buffer creation was
/// successful. It provides the new wl_buffer referencing the dmabuf(s).
///
/// Upon receiving this event, the client should destroy the
/// zwp_linux_buffer_params_v1 object.
struct zwp_linux_buffer_params_v1::event::created {
    static constexpr Wopcode<zwp_linux_buffer_params_v1> opcode{ 0 };
    /// the newly created wl_buffer
    Wnew_id<wl_buffer> buffer;
};

/// buffer creation failed
///
/// This event indicates that the attempted buffer creation has
/// failed. It usually means that one of the dmabuf constraints
/// has not been fulfilled.
///
/// Upon receiving this event, the client should destroy the
/// zwp_linux_buffer_params_v1 object.
struct zwp_linux_buffer_params_v1::event::failed {
    static constexpr Wopcode<zwp_linux_buffer_params_v1> opcode{ 1 };
};

enum class zwp_linux_dmabuf_feedback_v1::tranche_flags : Wuint::integral_type {
    /// direct scan-out tranche
    Escanout = 1
};

/// destroy the feedback object
///
/// Using this request a client can tell the server that it is not going to
/// use the wp_linux_dmabuf_feedback object anymore.
struct zwp_linux_dmabuf_feedback_v1::request::destroy {
    static constexpr Wopcode<zwp_linux_dmabuf_feedback_v1> opcode{ 0 };
};

/// all feedback has been sent
///
/// This event is sent after all parameters of a wp_linux_dmabuf_feedback
/// object have been sent.
///
/// This allows changes to the wp_linux_dmabuf_feedback parameters to be
/// seen as atomic, even if they happen via multiple events.
struct zwp_linux_dmabuf_feedback_v1::event::done {
    static constexpr Wopcode<zwp_linux_dmabuf_feedback_v1> opcode{ 0 };
};

/// format and modifier table
///
/// This event provides a file descriptor which can be memory-mapped to
/// access the format and modifier table.
///
/// The table contains a tightly packed array of consecutive format +
/// modifier pairs. Each pair is 16 bytes wide. It contains a format as a
/// 32-bit unsigned integer, followed by 4 bytes of unused padding, and a
/// modifier as a 64-bit unsigned integer. The native endianness is used.
///
/// The client must map the file descriptor in read-only private mode.
///
/// Compositors are not allowed to mutate the table file contents once this
/// event has been sent. Instead, compositors must create a new, separate
/// table file and re-send feedback parameters. Compositors are allowed to
/// store duplicate format + modifier pairs in the table.
struct zwp_linux_dmabuf_feedback_v1::event::format_table {
    static constexpr Wopcode<zwp_linux_dmabuf_feedback_v1> opcode{ 1 };
    /// table file descriptor
    Wfd fd;
    /// table size, in bytes
    Wuint size;
};

/// preferred main device
///
/// This event advertises the main device that the server prefers to use
/// when direct scan-out to the target device isn't possible. The
/// advertised main device may be different for each
/// wp_linux_dmabuf_feedback object, and may change over time.
///
/// There is exactly one main device. The compositor must send at least
/// one preference tranche with tranche_target_device equal to main_device.
///
/// Clients need to create buffers that the main device can import and
/// read from, otherwise creating the dmabuf wl_buffer will fail (see the
/// wp_linux_buffer_params.create and create_immed requests for details).
/// The main device will also likely be kept active by the compositor,
/// so clients can use it instead of waking up another device for power
/// savings.
///
/// In general the device is a DRM node. The DRM node type (primary vs.
/// render) is unspecified. Clients must not rely on the compositor sending
/// a particular node type. Clients cannot check two devices for equality
/// by comparing the dev_t value.
///
/// If explicit modifiers are not supported and the client performs buffer
/// allocations on a different device than the main device, then the client
/// must force the buffer to have a linear layout.
struct zwp_linux_dmabuf_feedback_v1::event::main_device {
    static constexpr Wopcode<zwp_linux_dmabuf_feedback_v1> opcode{ 2 };
    /// device dev_t value
    Warray device;
};

/// a preference tranche has been sent
///
/// This event splits tranche_target_device and tranche_formats events in
/// preference tranches. It is sent after a set of tranche_target_device
/// and tranche_formats events; it represents the end of a tranche. The
/// next tranche will have a lower preference.
struct zwp_linux_dmabuf_feedback_v1::event::tranche_done {
    static constexpr Wopcode<zwp_linux_dmabuf_feedback_v1> opcode{ 3 };
};

/// target device
///
/// This event advertises the target device that the server prefers to use
/// for a buffer created given this tranche. The advertised target device
/// may be different for each preference tranche, and may change over time.
///
/// There is exactly one target device per tranche.
///
/// The target device may be a scan-out device, for example if the
/// compositor prefers to directly scan-out a buffer created given this
/// tranche. The target device may be a rendering device, for example if
/// the compositor prefers to texture from said buffer.
///
/// The client can use this hint to allocate the buffer in a way that makes
/// it accessible from the target device, ideally directly. The buffer must
/// still be accessible from the main device, either through direct import
/// or through a potentially more expensive fallback path. If the buffer
/// can't be directly imported from the main device then clients must be
/// prepared for the compositor changing the tranche priority or making
/// wl_buffer creation fail (see the wp_linux_buffer_params.create and
/// create_immed requests for details).
///
/// If the device is a DRM node, the DRM node type (primary vs. render) is
/// unspecified. Clients must not rely on the compositor sending a
/// particular node type. Clients cannot check two devices for equality by
/// comparing the dev_t value.
///
/// This event is tied to a preference tranche, see the tranche_done event.
struct zwp_linux_dmabuf_feedback_v1::event::tranche_target_device {
    static constexpr Wopcode<zwp_linux_dmabuf_feedback_v1> opcode{ 4 };
    /// device dev_t value
    Warray device;
};

/// supported buffer format modifier
///
/// This event advertises the format + modifier combinations that the
/// compositor supports.
///
/// It carries an array of indices, each referring to a format + modifier
/// pair in the last received format table (see the format_table event).
/// Each index is a 16-bit unsigned integer in native endianness.
///
/// For legacy support, DRM_FORMAT_MOD_INVALID is an allowed modifier.
/// It indicates that the server can support the format with an implicit
/// modifier. When a buffer has DRM_FORMAT_MOD_INVALID as its modifier, it
/// is as if no explicit modifier is specified. The effective modifier
/// will be derived from the dmabuf.
///
/// A compositor that sends valid modifiers and DRM_FORMAT_MOD_INVALID for
/// a given format supports both explicit modifiers and implicit modifiers.
///
/// Compositors must not send duplicate format + modifier pairs within the
/// same tranche or across two different tranches with the same target
/// device and flags.
///
/// This event is tied to a preference tranche, see the tranche_done event.
///
/// For the definition of the format and modifier codes, see the
/// wp_linux_buffer_params.create request.
struct zwp_linux_dmabuf_feedback_v1::event::tranche_formats {
    static constexpr Wopcode<zwp_linux_dmabuf_feedback_v1> opcode{ 5 };
    /// array of 16-bit indexes
    Warray indices;
};

/// tranche flags
///
/// This event sets tranche-specific flags.
///
/// The scanout flag is a hint that direct scan-out may be attempted by the
/// compositor on the target device if the client appropriately allocates a
/// buffer. How to allocate a buffer that can be scanned out on the target
/// device is implementation-defined.
///
/// This event is tied to a preference tranche, see the tranche_done event.
struct zwp_linux_dmabuf_feedback_v1::event::tranche_flags {
    static constexpr Wopcode<zwp_linux_dmabuf_feedback_v1> opcode{ 6 };
    /// tranche flags
    zwp_linux_dmabuf_feedback_v1::tranche_flags flags;
};

} // namespace protocols
} // namespace wl
} // namespace ger
