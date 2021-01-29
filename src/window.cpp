/*
 * Created on 24.01.2021 by nefchen.
 */

#include <SDL2/SDL.h>

#include "window.hpp"
#include "types.hpp"
#include "views/view.hpp"


namespace win
{
    Window::Window(comm::Node comm_node)
        : m_comm_node{std::move(comm_node)},
          m_rect{0, 0, 800, 800}
    {
        m_sdl_window = SDL_CreateWindow(
            "Deadly Space Adventures",
            m_rect.x, m_rect.y, m_rect.w, m_rect.h,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE |
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );
        if (m_sdl_window == nullptr)
        {
            return;
        }

        m_sdl_renderer = SDL_CreateRenderer(
            m_sdl_window, -1, SDL_RENDERER_ACCELERATED
        );

        connect_signal_events();
    };

    Window::Window(Window&& other)
        : m_comm_node{std::move(other.m_comm_node)},
          m_view{std::move(other.m_view)},
          m_rect{other.m_rect}
    {
        m_sdl_renderer = other.m_sdl_renderer;
        m_sdl_window = other.m_sdl_window;

        other.m_sdl_renderer = nullptr;
        other.m_sdl_window = nullptr;

        disconnect_signals(other.m_signal_ds);
        other.m_signal_ds.clear();

        connect_signal_events();
    };

    Window& Window::operator=(Window&& other)
    {
        SDL_DestroyRenderer(m_sdl_renderer);
        SDL_DestroyWindow(m_sdl_window);

        m_sdl_renderer = other.m_sdl_renderer;
        m_sdl_window = other.m_sdl_window;
        m_view = std::move(other.m_view);
        m_comm_node = std::move(other.m_comm_node);
        m_rect = other.m_rect;

        other.m_sdl_renderer = nullptr;
        other.m_sdl_window = nullptr;

        disconnect_signals(other.m_signal_ds);
        other.m_signal_ds.clear();

        connect_signal_events();

        return *this;
    };

    Window::~Window()
    {
        SDL_DestroyRenderer(m_sdl_renderer);
        SDL_DestroyWindow(m_sdl_window);

        comm::disconnect_signals(m_signal_ds);
    };

    void Window::connect_signal_events()
    {
        m_signal_ds.push_back(
            m_comm_node->window_resized.connect(
                [this] (Point p) {
                    this->m_rect.w = p.x;
                    this->m_rect.h = p.y;
                    this->m_view->propagate_resize(this->m_rect);
                }
            )
        );

        m_signal_ds.push_back(
            m_comm_node->window_moved.connect(
                [this] (Point p) {
                    this->m_rect.x = p.x;
                    this->m_rect.y = p.y;
                }
            )
        );

        m_signal_ds.push_back(
            m_comm_node->window_hidden.connect(
                [this] () {
                    this->m_should_render = false;
                }
            )
        );

        m_signal_ds.push_back(
            m_comm_node->window_exposed.connect(
                [this] () {
                    this->m_should_render = true;
                }
            )
        );
    };

    void Window::render()
    {
        if (!m_should_render)
        {
            return;
        }

        SDL_SetRenderDrawColor(m_sdl_renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_sdl_renderer);

        if (m_view != nullptr)
        {
            m_view->render(m_sdl_renderer, m_rect);
        }

        SDL_RenderPresent(m_sdl_renderer);
    };
}

