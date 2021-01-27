/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>
#include <memory>

#include "comm.hpp"
#include "types.hpp"
#include "views/view.hpp"


namespace win
{
    struct Window
    {
        Window(comm::AppControlNode app_node, comm::WindowNode win_node);
        Window(Window const& other) = delete;
        Window(Window&& other);
        ~Window();
        Window& operator=(Window const& other) = delete;
        Window& operator=(Window&& other);

        void render();
        void connect_signal_events();

        template <typename T>
        void load_view()
        {
            m_view = std::make_unique<T>(m_win_node);
        };

        std::unique_ptr<view::View> m_view;
        SDL_Window* m_sdl_window;
        SDL_Renderer* m_sdl_renderer;
        Rect m_rect;

        comm::AppControlNode m_app_node;
        comm::WindowNode m_win_node;
        std::vector<comm::Disconnector> m_signal_ds;
    };
}

#endif  // WINDOW_HPP

