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
              create_window_request{dispatcher},
              destroy_window_request{dispatcher},
              window_resized{dispatcher},
              window_hidden{dispatcher},
              window_exposed{dispatcher},
              mouse_button_clicked{dispatcher},
              mouse_moved{dispatcher}
        {};

        Signal<> app_exit_request;
        Signal<> create_window_request;
        Signal<Id> destroy_window_request;
        Signal<Id, Point> window_resized;
        Signal<Id> window_hidden;
        Signal<Id> window_exposed;
        Signal<Point, input::MouseButton, u8, Id> mouse_button_clicked;
        Signal<Point, Id> mouse_moved;
    };

    using Node = std::shared_ptr<_Node>;
}

#endif  // COMM_COMM_HPP

