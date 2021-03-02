/*
 * Created on 01.03.2021 by nefchen.
 */

#ifndef WIDGETS_VIEWPORT_HANDLE_HPP
#define WIDGETS_VIEWPORT_HANDLE_HPP

#include <functional>

#include "types/sdl.hpp"
#include "types/basic.hpp"


namespace view
{
    inline void default_handle_output(SDL_Renderer* renderer)
    {};

    struct ViewportHandle
    {
        std::function<void(SDL_Renderer*)> m_render{default_handle_output};
        Rect m_rect;
    };
};

#endif  // WIDGETS_VIEWPORT_HANDLE_HPP

