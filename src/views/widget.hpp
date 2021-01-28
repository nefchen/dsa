/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef VIEWS_WIDGET_HPP
#define VIEWS_WIDGET_HPP

#include <SDL2/SDL.h>

#include "../types.hpp"
#include "../comm.hpp"


namespace view
{
    struct View;

    struct Widget
    {
        Widget(View* view): m_view{view}
        {};

        virtual void draw(SDL_Renderer* renderer)
        {};

        Rect m_rect{0, 0, 0, 0};
        Rect m_context_rect{0, 0, 0, 0};
        View* m_view;
    };
}

#endif  // VIEWS_WIDGET_HPP

