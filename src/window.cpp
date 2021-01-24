/*
 * Created on 24.01.2021 by nefchen.
 */

#include <SDL2/SDL.h>

#include "window.hpp"


namespace win
{
    Window::Window(std::shared_ptr<comm::AppControlNode> app_node)
        : m_app_node{std::move(app_node)}
    {
        m_sdl_window = SDL_CreateWindow(
            "Deadly Space Adventures", 0, 0, 800, 800, SDL_WINDOW_SHOWN
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
    {
        m_sdl_renderer = other.m_sdl_renderer;
        m_sdl_window = other.m_sdl_window;
        m_view = std::move(other.m_view);
        m_app_node = std::move(other.m_app_node);

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
        return;
    };
}

