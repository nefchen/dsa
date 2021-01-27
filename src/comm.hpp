/*
 * Created on 22.01.2021 by nefchen.
 */

#ifndef COMM_HPP
#define COMM_HPP

#include <memory>

#include "signals.hpp"
#include "user_input.hpp"
#include "types.hpp"


namespace comm
{
    template <typename... Ts>
    std::shared_ptr<Signal<Ts...>>
    _shared(std::shared_ptr<Dispatcher> d)
    {
        return std::make_shared<Signal<Ts...>>(d);
    };

    /*
     * NOTE: Nodes are special exceptions to the rule of
     *       appending m_ at the beginning of struct
     *       attributes... there is always an exception.
     */

    struct AppControlNode
    {
        AppControlNode(std::shared_ptr<Dispatcher> dispatcher)
            : app_exit_request{_shared<>(dispatcher)},
              create_window_request{_shared<>(dispatcher)},
              destroy_window_request{_shared<Id>(dispatcher)}
        {};

        std::shared_ptr<Signal<>> app_exit_request;
        std::shared_ptr<Signal<>> create_window_request;
        std::shared_ptr<Signal<Id>> destroy_window_request;
    };

    struct WindowNode
    {
        WindowNode(std::shared_ptr<Dispatcher> dispatcher)
            : mouse_button_click{_shared<mouse::Button, u8>(dispatcher)},
              mouse_moved{_shared<Point>(dispatcher)},
              window_resized{_shared<Point>(dispatcher)},
              window_moved{_shared<Point>(dispatcher)}
        {};

        std::shared_ptr<Signal<mouse::Button, u8>> mouse_button_click;
        std::shared_ptr<Signal<Point>> mouse_moved;
        std::shared_ptr<Signal<Point>> window_resized;
        std::shared_ptr<Signal<Point>> window_moved;
    };
}

#endif  // COMM_HPP

