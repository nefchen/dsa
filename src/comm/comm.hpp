/*
 * Created on 22.01.2021 by nefchen.
 */

#ifndef COMM_COMM_HPP
#define COMM_COMM_HPP

#include <memory>

#include "comm/signals.hpp"
#include "types/input.hpp"


namespace comm
{
    struct _Node
    {
        _Node(std::shared_ptr<Dispatcher> dispatcher)
            : app_exit_request{dispatcher},
              window_resized{dispatcher},
              window_hidden{dispatcher},
              window_exposed{dispatcher},
              mouse_button_clicked{dispatcher},
              mouse_moved{dispatcher}
        {};

        Signal<> app_exit_request;
        Signal<Point> window_resized;
        Signal<> window_hidden;
        Signal<> window_exposed;
        Signal<Point, input::MouseButton, u8> mouse_button_clicked;
        Signal<Point> mouse_moved;
    };

    using Node = std::shared_ptr<_Node>;
    using AutodeleteLifetime = std::function<void(void)>;

    template <typename... Args>
    inline AutodeleteLifetime bind_autodelete_lifetime(Lifetime lf, Signal<Args...>& sig)
    {
        return [&sig, lf = std::move(lf)]() mutable {
            sig.disconnect(std::move(lf));
        };
    }
}

#endif  // COMM_COMM_HPP

