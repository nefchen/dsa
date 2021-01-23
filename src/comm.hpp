/*
 * Created on 22.01.2021 by nefchen.
 */

#ifndef COMM_HPP
#define COMM_HPP

#include <memory>

#include "signals.hpp"


namespace comm
{
    struct AppControlNode
    {
        AppControlNode(std::shared_ptr<Dispatcher> dispatcher)
            : app_exit_request{dispatcher}
        {};

        Signal<> app_exit_request;
    };
}

#endif  // COMM_HPP

