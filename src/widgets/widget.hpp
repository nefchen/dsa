/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef WIDGETS_WIDGET_HPP
#define WIDGETS_WIDGET_HPP

#include "comm/comm.hpp"
#include "types/input.hpp"
#include "types/sdl.hpp"
#include "types/basic.hpp"


namespace view
{
    struct View;

    struct Widget
    {
        Widget(View* view)
            : m_view{view}
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
        Id m_id{0};

        // Widget signals.
        comm::Signal<Point, input::MouseHover> m_mouse_hover_signal{};
        comm::Signal<Point, input::MouseButton> m_clicked{};

        std::vector<comm::Lifetime> m_signal_lfs;
    };

    struct WidgetState
    {
        bool m_hovered{false};
    };
}

#endif  // WIDGETS_WIDGET_HPP

