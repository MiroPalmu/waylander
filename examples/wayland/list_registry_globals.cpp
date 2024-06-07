#include <algorithm>
#include <print>

#include "wayland/protocol.hpp"
#include "wayland/protocols/wayland_protocol.hpp"

using wl_display   = ger::wl::protocols::wl_display;
using wl_registery = ger::wl::protocols::wl_registry;
using wl_callback  = ger::wl::protocols::wl_callback;

using get_registery_t = wl_display::request::get_registry;
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

    const auto handle_event = [&](const auto& event) {
        std::println("{{{}, {}}}", event.object_id.value, event.opcode.value);

        if (event.object_id == sync_callback and event.opcode == done_t::opcode) {
            done_on_sync_received = true;
        }
    };

    std::println("List of known object ids:");
    std::println("{} = global display object", ger::wl::global_display_object.value);
    std::println("{} = wl_registery", registery.value);
    std::println("{} = wl_callback", sync_callback.value);
    std::println();

    std::println("Sending requests get_registery and sync to global display object...");
    std::println("Received events in form {{object id, opcode}}:");
    while (not done_on_sync_received) {
        auto msg_parser = client.recv_events();
        auto msg_gen    = msg_parser.message_generator();
        std::ranges::for_each(msg_gen, handle_event);
    }
    std::println("Done event for the sync recevied!");
}
