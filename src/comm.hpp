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
    struct _Node
    {
        _Node(std::shared_ptr<Dispatcher> dispatcher)
            : app_exit_request{dispatcher},
              create_window_request{dispatcher},
              destroy_window_request{dispatcher},
              mouse_button_clicked{dispatcher},
              mouse_moved{dispatcher},
              window_resized{dispatcher},
              window_moved{dispatcher},
              window_hidden{dispatcher},
              window_exposed{dispatcher}
        {};

        Signal<> app_exit_request;
        Signal<> create_window_request;
        Signal<Id> destroy_window_request;
        Signal<Point, mouse::Button> mouse_button_clicked;
        Signal<Point> mouse_moved;
        Signal<Point> window_resized;
        Signal<Point> window_moved;
        Signal<> window_hidden;
        Signal<> window_exposed;
    };

    using Node = std::shared_ptr<_Node>;
}

#endif  // COMM_HPP

