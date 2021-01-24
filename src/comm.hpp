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
    struct AppControlNode;
    template <typename... Args>
    using SignalPtr = std::shared_ptr<Signal<Args...>>;

    template <typename T, typename... Args>
    std::shared_ptr<T>
    share_signal(std::shared_ptr<T>& sig, Args... args)
    {
        return std::make_shared<T>(args...);
    };

    struct AppControlNode
    {
        AppControlNode(std::shared_ptr<Dispatcher> dispatcher)
            : app_exit_request{share_signal(app_exit_request, dispatcher)},
              create_window_request{share_signal(create_window_request, dispatcher)},
              destroy_window_request{share_signal(destroy_window_request, dispatcher)}
        {};


        SignalPtr<> app_exit_request;
        SignalPtr<AppControlNode> create_window_request;
        SignalPtr<Id> destroy_window_request;
    };

    struct WindowNode
    {
        WindowNode(std::shared_ptr<Dispatcher> dispatcher)
            : mouse_button_click{share_signal(mouse_button_click, dispatcher)},
              mouse_moved{share_signal(mouse_moved, dispatcher)}
        {};

        SignalPtr<mouse::Button, u8> mouse_button_click;
        SignalPtr<Point> mouse_moved;
    };
}

#endif  // COMM_HPP

