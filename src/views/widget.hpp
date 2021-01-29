/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef VIEWS_WIDGET_HPP
#define VIEWS_WIDGET_HPP

#include <SDL2/SDL.h>

#include "../types.hpp"
#include "../utils.hpp"
#include "../comm.hpp"


namespace view
{
    struct View;

    struct Widget
    {
        Widget(View* view)
            : m_view{view}, m_id{get_unique_id()}
        {};

        virtual void draw(SDL_Renderer* renderer)
        {};

        virtual void resize(Rect rect)
        {
            m_context_rect = rect;
        };

        Rect m_rect{0, 0, 0, 0};
        Rect m_context_rect{0, 0, 0, 0};
        View* m_view;
        Id m_id;
    };
}

#endif  // VIEWS_WIDGET_HPP

