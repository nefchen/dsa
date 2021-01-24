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
    struct AppControlNode
    {
        AppControlNode(std::shared_ptr<Dispatcher> dispatcher)
            : app_exit_request{dispatcher},
              create_window_request{dispatcher}
        {};

        Signal<> app_exit_request;
        Signal<std::shared_ptr<AppControlNode>> create_window_request;
        Signal<Id> destroy_window_request;
    };

    struct WindowNode
    {
        WindowNode(std::shared_ptr<Dispatcher> dispatcher)
            : mouse_button_pressed_released{dispatcher},
              mouse_button_click{dispatcher},
              mouse_moved{dispatcher}
        {};

        Signal<mouse::Button, mouse::ButtonState> mouse_button_pressed_released;
        Signal<mouse::Button, u8> mouse_button_click;
        Signal<Point> mouse_moved;
    };
}

#endif  // COMM_HPP

