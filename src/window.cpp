/*
 * Created on 24.01.2021 by nefchen.
 */

#include <SDL2/SDL.h>

#include "window.hpp"
#include "views/view.hpp"


namespace win
{
    Window::Window(comm::AppControlNode app_node, comm::WindowNode win_node)
        : m_app_node{std::move(app_node)},
          m_win_node{std::move(win_node)},
          m_rect{0, 0, 800, 800}
    {
        m_sdl_window = SDL_CreateWindow(
            "Deadly Space Adventures",
            m_rect.x, m_rect.y, m_rect.w, m_rect.h, SDL_WINDOW_SHOWN
        );
        if (m_sdl_window == nullptr)
        {
            return;
        }

        m_sdl_renderer = SDL_CreateRenderer(
            m_sdl_window, -1, SDL_RENDERER_ACCELERATED
        );
    };

    Window::Window(Window&& other)
        : m_app_node{std::move(other.m_app_node)},
          m_win_node{std::move(other.m_win_node)},
          m_view{std::move(other.m_view)},
          m_rect{other.m_rect}
    {
        m_sdl_renderer = other.m_sdl_renderer;
        m_sdl_window = other.m_sdl_window;

        other.m_sdl_renderer = nullptr;
        other.m_sdl_window = nullptr;
    };

    Window& Window::operator=(Window&& other)
    {
        SDL_DestroyRenderer(m_sdl_renderer);
        SDL_DestroyWindow(m_sdl_window);

        m_sdl_renderer = other.m_sdl_renderer;
        m_sdl_window = other.m_sdl_window;
        m_view = std::move(other.m_view);
        m_app_node = std::move(other.m_app_node);
        m_win_node = std::move(other.m_win_node);

        other.m_sdl_renderer = nullptr;
        other.m_sdl_window = nullptr;

        return *this;
    };

    Window::~Window()
    {
        SDL_DestroyRenderer(m_sdl_renderer);
        SDL_DestroyWindow(m_sdl_window);
    };

    void Window::render()
    {
        SDL_SetRenderDrawColor(m_sdl_renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(m_sdl_renderer, &m_rect);

        if (m_view != nullptr)
        {
            for (auto& widget :m_view->m_view_widgets)
            {
                widget->draw(m_sdl_renderer);
            }
        }

        SDL_RenderPresent(m_sdl_renderer);
    };
}

