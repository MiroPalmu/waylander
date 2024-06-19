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

int main() {
    auto client = ger::wl::connected_client{};

    const auto registery     = client.reserve_object_id<wl_registery>();
    const auto sync_callback = client.reserve_object_id<wl_callback>();

    client.register_request(ger::wl::global_display_object, get_registery_t{ registery });
    client.register_request(ger::wl::global_display_object, sync_t{ sync_callback });
    client.flush_registered_requests();

    auto done_on_sync_received{ false };

    auto msg_overloads = ger::wl::message_overload_set{};

    msg_overloads.add_overload<done_t>(sync_callback, [&](const done_t& msg) {
        done_on_sync_received = true;
        std::println("done event {} for the sync!", msg.callback_data.value);
    });

    msg_overloads.add_overload<global_t>(registery, [&](const global_t& msg) {
        std::println("registery global event:");
        std::println("    name: {}", msg.name.value);
        // wonky stuff:
        std::println("    interface: {}",
                     std::string_view(reinterpret_cast<char const*>(msg.interface.data()),
                                      msg.interface.size()));
        std::println("    version: {}", msg.version.value);
    });

    std::println("Sending requests get_registery and sync to global display object...");
    std::println("Receiving events:\n");
    while (not done_on_sync_received) {
        auto msg_parser = client.recv_events();
        ger::wl::message_visit([] { std::println("Received unhandeled event!"); },
                               msg_overloads,
                               msg_parser.message_generator());
    }
}
