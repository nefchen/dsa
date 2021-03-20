/*
 * Created on 22.01.2021 by nefchen.
 */

#ifndef COMM_COMM_HPP
#define COMM_COMM_HPP

#include <memory>

#include "comm/signals.hpp"
#include "types/input.hpp"
#include "views/loader.hpp"
#include "widgets/viewport_handle.hpp"


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
              mouse_moved{dispatcher},
              load_view{dispatcher},
              create_game{dispatcher},
              exit_game{dispatcher}
        {};

        Signal<> app_exit_request;
        Signal<Point> window_resized;
        Signal<> window_hidden;
        Signal<> window_exposed;
        Signal<Point, input::MouseButton, u8> mouse_button_clicked;
        Signal<Point> mouse_moved;
        Signal<view::Loader> load_view;
        Signal<> create_game;
        Signal<> exit_game;
        Signal<std::shared_ptr<view::ViewportHandle>> add_viewport_handle_to_game;
    };
    using Node = std::shared_ptr<_Node>;

    // Handy functionality to auto disconnect signals. This is
    // not part of the main signals.hpp API as it is prone to
    // seg faults if the signal goes out of scope first.
    namespace unsafe
    {
        using Fn = std::function<void(void)>;
        struct AutodeleteLifetime
        {
            AutodeleteLifetime(Fn&& deleter) : m_deleter(std::move(deleter))
            {};

            AutodeleteLifetime(AutodeleteLifetime const& rhs) = delete;
            AutodeleteLifetime& operator=(AutodeleteLifetime const& rhs) = delete;

            AutodeleteLifetime(AutodeleteLifetime&& rhs) = default;
            AutodeleteLifetime& operator=(AutodeleteLifetime&& rhs) = default;

            ~AutodeleteLifetime()
            {
                if (m_deleter != nullptr)
                {
                    m_deleter();
                }
            }

            void operator()()
            {
                if (m_deleter != nullptr)
                {
                    m_deleter();
                }
                m_deleter = nullptr;
            };

            private:
                Fn m_deleter;
        };

        template <typename... Args>
        inline AutodeleteLifetime bind_autodelete_lifetime(Lifetime&& lf, Signal<Args...>& sig)
        {
            return AutodeleteLifetime(
                [&sig, lf = std::move(lf)]() mutable {
                    sig.disconnect(std::move(lf));
                }
            );
        }
    }
}

#endif  // COMM_COMM_HPP

