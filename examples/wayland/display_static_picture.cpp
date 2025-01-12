// Copyright (C) 2024 Miro Palmu.
//
// This file is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this file.  If not, see <https://www.gnu.org/licenses/>.

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <optional>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "gnulander/memory_block.hpp"
#include "wayland/message_visitor.hpp"
#include "wayland/parsed_message.hpp"
#include "wayland/connected_client.hpp"
#include "wayland/protocols/wayland_protocol.hpp"
#include "wayland/protocols/xdg_shell_protocol.hpp"

struct StaticPicture {
    // Definitions:

    static constexpr auto color_format = ger::wl::protocols::wl_shm::format::Eargb8888;
    struct Pixel {
        std::uint8_t blue, green, red, alpha;
    };
#ifdef __cpp_lib_is_implicit_lifetime
    static_assert(std::is_implicit_lifetime_v<Pixel>);
#endif
    static_assert(std::alignment_of_v<Pixel> == std::alignment_of_v<std::byte>);
    static_assert(sizeof(Pixel) == sizeof(std::uint32_t));

    struct PixelCoord {
        int x, y;
    };

    using pixel_view_type = std::tuple<PixelCoord, Pixel&>;

    // Member functions:

    constexpr int bytes_required() const { return width * height * sizeof(Pixel); };

    auto view_pixels() {
        namespace rv = std::ranges::views;
        return bytes | rv::chunk(4) | rv::enumerate | rv::transform([&](auto index_bgra_tuple) {
                   const auto index = static_cast<int>(std::get<0>(index_bgra_tuple));
                   const auto x     = index % width;
                   const auto y     = index / width;
                   auto pixel_begin = std::get<1>(index_bgra_tuple).begin();
                   return pixel_view_type{ { x, y }, reinterpret_cast<Pixel&>(*pixel_begin) };
               });
    }

    // Member data:

    int width, height;
    gnulander::memory_block buff;
    gnulander::mapped_memory bytes;

    [[nodiscard]] StaticPicture(const int width_arg, const int height_arg)
        : width{ width_arg },
          height{ height_arg },
          buff{

              [&] {
                  auto mem = gnulander::memory_block{};
                  mem.truncate(bytes_required());
                  return mem;
              }()
          },
          bytes{ buff.map(bytes_required()) } {}
};

class static_picture_window {
    // Interfaces:
    using wl_registry   = ger::wl::protocols::wl_registry;
    using wl_display    = ger::wl::protocols::wl_display;
    using wl_callback   = ger::wl::protocols::wl_callback;
    using wl_shm        = ger::wl::protocols::wl_shm;
    using wl_shm_pool   = ger::wl::protocols::wl_shm_pool;
    using wl_buffer     = ger::wl::protocols::wl_buffer;
    using wl_compositor = ger::wl::protocols::wl_compositor;
    using wl_surface    = ger::wl::protocols::wl_surface;
    using xdg_wm_base   = ger::wl::protocols::xdg_wm_base;
    using xdg_surface   = ger::wl::protocols::xdg_surface;
    using xdg_toplevel  = ger::wl::protocols::xdg_toplevel;

    // Requests:
    using wl_display_sync              = wl_display::request::sync;
    using wl_registry_bind             = wl_registry::request::bind;
    using wl_display_get_registry      = wl_display::request::get_registry;
    using wl_shm_create_pool           = wl_shm::request::create_pool;
    using wl_shm_pool_create_buffer    = wl_shm_pool::request::create_buffer;
    using wl_shm_pool_destroy          = wl_shm_pool::request::destroy;
    using wl_compositor_create_surface = wl_compositor::request::create_surface;
    using wl_surface_commit            = wl_surface::request::commit;
    using wl_surface_attach            = wl_surface::request::attach;
    using xdg_wm_base_get_xdg_surface  = xdg_wm_base::request::get_xdg_surface;
    using xdg_wm_base_pong             = xdg_wm_base::request::pong;
    using xdg_surface_get_toplevel     = xdg_surface::request::get_toplevel;
    using xdg_surface_ack_configure    = xdg_surface::request::ack_configure;

    // Events:
    using wl_callback_done      = wl_callback::event::done;
    using wl_registry_global    = wl_registry::event::global;
    using wl_display_error      = wl_display::event::error;
    using xdg_wm_base_ping      = xdg_wm_base::event::ping;
    using xdg_surface_configure = xdg_surface::event::configure;
    using xdg_toplevel_close    = xdg_toplevel::event::close;

    ger::wl::connected_client client_ = {};

    template<typename W>
    using Wobject = ger::wl::Wobject<W>;

    Wobject<wl_display> display_obj_id_ = { ger::wl::global_display_object };
    Wobject<wl_registry> registry_obj_id_;
    Wobject<wl_callback> sync_obj_id_;
    Wobject<wl_compositor> compositor_obj_id_;
    Wobject<wl_shm> shm_obj_id_;
    Wobject<wl_buffer> buffer_obj_id_;
    Wobject<wl_surface> wl_surface_obj_id_;
    Wobject<xdg_wm_base> wm_base_obj_id_;
    Wobject<xdg_surface> xdg_surface_obj_id_;
    Wobject<xdg_toplevel> toplevel_obj_id_;

    StaticPicture picture_;

    /// Handels: wl_display:error
    auto fatal_error_overloads_() {
        auto vis = ger::wl::message_overload_set{};
        vis.add_overload<wl_display_error>(display_obj_id_, [](const wl_display_error& err) {
            const auto err_type_str = [&]() -> std::string {
                switch (static_cast<wl_display::error>(err.code.value)) {
                    case wl_display::error::Einvalid_object: return "invalid object";
                    case wl_display::error::Einvalid_method: return "invalid method";
                    case wl_display::error::Eno_memory: return "no memory";
                    case wl_display::error::Eimplementation: return "implementation";
                    default: return "non-global error";
                }
            }();
            std::println("Received global display object error event:");
            std::println("    object id: {}", err.object_id.value);
            std::println("    error type: {}", err_type_str);
            // wonky stuff:
            std::println("    desc: {}",
                         std::string_view(reinterpret_cast<char const*>(err.message.data()),
                                          err.message.size()));
            std::exit(1);
        });
        return vis;
    }

    /// Handels: wl_display::error, xdg_wm_base::ping
    ///
    /// Returned overload set should not escape the scope of the member function which
    /// called this function, as this pointer is stored in it which could be invalidated.
    auto default_overloads_() {
        auto vis = fatal_error_overloads_();
        vis.add_overload<xdg_wm_base_ping>(wm_base_obj_id_, [this](const xdg_wm_base_ping ping) {
            client_.register_request(wm_base_obj_id_, xdg_wm_base_pong{ ping.serial });
        });
        return vis;
    }

  public:
    /// Connects to compositor, enumerates and binds globals
    [[nodiscard]] explicit static_picture_window(StaticPicture pic) : picture_{ std::move(pic) } {
        // Initial stage:

        const auto get_reg = wl_display_get_registry{ registry_obj_id_.reserve_id(client_) };
        const auto sync    = wl_display_sync{ sync_obj_id_.reserve_id(client_) };
        client_.register_request(display_obj_id_, get_reg);
        client_.register_request(display_obj_id_, sync);
        client_.flush_registered_requests();

        auto globals_binder = fatal_error_overloads_();

        auto wl_shm_found        = false;
        auto wl_compositor_found = false;
        auto xdg_wm_base_found   = false;

        globals_binder.add_overload<wl_registry_global>(
            registry_obj_id_,
            [&](const wl_registry_global& msg) {
                const auto id = [&]() -> std::optional<ger::wl::Wnew_id<>> {
                    if (msg.interface == u8"wl_shm") {
                        // wl_shm is a signleton so it is expected to find only one.
                        wl_shm_found = true;
                        return shm_obj_id_.reserve_id(client_);
                    } else if (msg.interface == u8"wl_compositor") {
                        // wl_compositor is a signleton so it is expected to find only one.
                        wl_compositor_found = true;
                        return compositor_obj_id_.reserve_id(client_);
                    } else if (not xdg_wm_base_found and msg.interface == u8"xdg_wm_base") {
                        // xdg_wm_base is not specified as singleton so bind just to the first one.
                        xdg_wm_base_found = true;
                        return wm_base_obj_id_.reserve_id(client_);
                    }
                    return {};
                }();
                if (id) {
                    client_.register_request(
                        registry_obj_id_,
                        wl_registry_bind{ msg.name, msg.interface, msg.version, id.value() });
                }
            });

        client_.recv_and_visit_events(globals_binder).until<wl_callback_done>(sync_obj_id_);
        if (not(wl_shm_found and wl_compositor_found and xdg_wm_base_found)) {
            std::println("fatal error: not all wanted globals found!");
            std::println("found (wl_shm, wl_compositor, xdg_wm_base): ({}, {}, {})",
                         wl_shm_found,
                         wl_compositor_found,
                         xdg_wm_base_found);
            std::exit(1);
        }
        client_.flush_registered_requests();

        // Config stage:

        const auto shm_pool_obj_id = client_.reserve_object_id<wl_shm_pool>();
        const auto create_pool     = wl_shm_create_pool{ .id   = shm_pool_obj_id,
                                                         .fd   = { gnulander::fd_ref{ picture_.buff } },
                                                         .size = picture_.bytes_required() };
        const auto create_buffer =
            wl_shm_pool_create_buffer{ .id     = buffer_obj_id_.reserve_id(client_),
                                       .offset = 0,
                                       .width  = picture_.width,
                                       .height = picture_.height,
                                       .stride = picture_.width
                                                 * static_cast<int>(sizeof(StaticPicture::Pixel)),
                                       .format = StaticPicture::color_format };

        // This will trigger format events from the wl_shm object,
        // but as we are using Eargb8888 which is required to be supported by all compositors,
        // we can ignore those events happily.
        client_.register_request(shm_obj_id_, create_pool);
        client_.register_request(shm_pool_obj_id, create_buffer);
        // As the buffers store reference to reference counted pool,
        // it can be immediatly destroyed.
        client_.register_request(shm_pool_obj_id, wl_shm_pool_destroy{});

        const auto create_surface =
            wl_compositor_create_surface{ wl_surface_obj_id_.reserve_id(client_) };
        const auto get_xdg_surface =
            xdg_wm_base_get_xdg_surface{ .id      = xdg_surface_obj_id_.reserve_id(client_),
                                         .surface = wl_surface_obj_id_ };
        const auto get_toplevel = xdg_surface_get_toplevel{ toplevel_obj_id_.reserve_id(client_) };
        client_.register_request(compositor_obj_id_, create_surface);
        client_.register_request(wm_base_obj_id_, get_xdg_surface);
        client_.register_request(xdg_surface_obj_id_, get_toplevel);
        client_.register_request(wl_surface_obj_id_, wl_surface_commit{});
        client_.flush_registered_requests();

        // Now we wait for xdg_surface_configure to mark the end of a configure sequence.
        // Before it there could be events advertising states of wl_surface or xdg_toplevel,
        // but we do not care about them, so they are ignored.
        auto config_stage_handler = default_overloads_();
        client_.recv_and_visit_events(config_stage_handler)
            .until<xdg_surface_configure>(xdg_surface_obj_id_, [&](const auto msg) {
                client_.register_request(xdg_surface_obj_id_,
                                         xdg_surface_ack_configure{ msg.serial });
            });

        client_.register_request(wl_surface_obj_id_,
                                 wl_surface_attach{ .buffer = buffer_obj_id_, .x = 0, .y = 0 });
        client_.register_request(wl_surface_obj_id_, wl_surface_commit{});
        client_.flush_registered_requests();
    }

    void wait_for_close_event() {
        auto vis = default_overloads_();
        client_.recv_and_visit_events(vis).until<xdg_toplevel_close>(toplevel_obj_id_, [](auto) {});
    }
};

template<std::integral I>
constexpr I pow(const I base, const std::type_identity_t<I> exp) {
    switch (exp) {
        case I{ 0 }: return 1;
        case I{ 1 }: return base;
    }

    const auto base_to_half_floor_exp = pow(base, exp / I{ 2 });
    if ((exp % I{ 2 }) == 0) {
        return base_to_half_floor_exp * base_to_half_floor_exp;
    } else {
        return base * base_to_half_floor_exp * base_to_half_floor_exp;
    }
}

int main() {
    const auto recursions_env = std::getenv("RECURSIONS");
    const auto recursions     = recursions_env ? std::stoi(std::string{ recursions_env }) : 2;

    const auto base_length_env = std::getenv("BASE_LENGTH");
    const auto base_length     = base_length_env ? std::stoi(std::string{ base_length_env }) : 20;
    const auto side_length     = base_length * pow(3, recursions);

    std::println("Drawing Sierpinski's Carpet with {} recursion and {} pixels as base length.",
                 recursions,
                 base_length);
    std::println("They can be controlled using enviroment variables RECURSIONS and BASE_LENGTH.");

    auto picture = StaticPicture(side_length, side_length);
    for (auto [coords, pixel] : picture.view_pixels()) {
        auto in_gap = false;
        for (const auto n : std::views::iota(1, recursions + 1)) {
            const auto recursive_side_length = base_length * pow(3, n);
            const auto recursive_x           = coords.x % recursive_side_length;
            const auto recursive_y           = coords.y % recursive_side_length;
            const auto recursive_x_frac = recursive_x / static_cast<double>(recursive_side_length);
            const auto recursive_y_frac = recursive_y / static_cast<double>(recursive_side_length);
            const auto x_in_mid_third =
                (recursive_x_frac > (1.0 / 3.0)) and (recursive_x_frac < (2.0 / 3.0));
            const auto y_in_mid_third =
                (recursive_y_frac > (1.0 / 3.0)) and (recursive_y_frac < (2.0 / 3.0));

            if (x_in_mid_third and y_in_mid_third) {
                in_gap = true;
                break;
            }
        }

        if (in_gap) {
            pixel.alpha = 0;
        } else {
            pixel = StaticPicture::Pixel{ .blue = 0, .green = 255, .red = 0, .alpha = 255 };
        }
    }

    auto window = static_picture_window{ std::move(picture) };
    window.wait_for_close_event();
}
