#include <config.h>

#include <algorithm>
#include <print>

#include "wayland/message_visitor.hpp"
#include "wayland/protocol.hpp"
#include "wayland/protocols/wayland_protocol.hpp"

using wl_display   = ger::wl::protocols::wl_display;
using wl_registery = ger::wl::protocols::wl_registry;
using wl_callback  = ger::wl::protocols::wl_callback;

using get_registery_t = wl_display::request::get_registry;
using global_t        = wl_registery::event::global;
using sync_t          = wl_display::request::sync;
using done_t          = wl_callback::event::done;
using fatal_error_t   = wl_display::event::error;

int main() {
    auto client = ger::wl::connected_client{};

    // Note that here the order matters. If one reserves the ids in some order,
    // then they have be binded in the same, as Wayland compositor can assume that
    // the new object ids are in monotonically increasing order.
    const auto registery     = client.reserve_object_id<wl_registery>();
    const auto sync_callback = client.reserve_object_id<wl_callback>();
    client.register_request(ger::wl::global_display_object, get_registery_t{ registery });
    client.register_request(ger::wl::global_display_object, sync_t{ sync_callback });

    std::println("Sending requests get_registery and sync to global display object...");
    client.flush_registered_requests();

    auto ov = ger::wl::message_overload_set{};
    ov.add_overload<global_t>(registery, [&](const global_t& msg) {
        std::println("{} version {}: {}",
                     msg.name.value,
                     msg.version.value,
                     std::string_view(reinterpret_cast<char const*>(msg.interface.data()),
                                      msg.interface.size()));
    });
    ov.add_overload<fatal_error_t>(ger::wl::global_display_object, [&](const auto& err) {
        std::println("Received global display object error event:");
        std::println("    object id: {}", err.object_id.value);

        const auto err_type = [&]() -> std::string {
            using err_enum = ger::wl::protocols::wl_display::error;
            switch (static_cast<err_enum>(err.code.value)) {
                case err_enum::Einvalid_object: return "invalid object";
                case err_enum::Einvalid_method: return "invalid method";
                case err_enum::Eno_memory: return "no memory";
                case err_enum::Eimplementation: return "implementation";
                default: return "non-global error";
            }
        }();
        std::println("    error type: {}", err_type);
        // wonky stuff:
        std::println("    desc: {}",
                     std::string_view(reinterpret_cast<char const*>(err.message.data()),
                                      err.message.size()));
        std::exit(1);
    });

    std::println("Registry globals:");
    client.recv_and_visit_events(ov).until<done_t>(sync_callback);
}
