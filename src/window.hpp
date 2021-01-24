/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>

#include "comm.hpp"
#include "view.hpp"


namespace win
{
    struct Window
    {
        Window(comm::AppControlNode app_node);
        Window(Window const& other) = delete;
        Window(Window&& other);
        ~Window();
        Window& operator=(Window const& other) = delete;
        Window& operator=(Window&& other);

        void render();

        view::View m_view;
        SDL_Window* m_sdl_window;
        SDL_Renderer* m_sdl_renderer;

        comm::AppControlNode m_app_node;
    };
}

#endif  // WINDOW_HPP

