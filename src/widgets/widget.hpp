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

        Widget(Widget const& other)
            : m_rect{other.m_rect},
              m_context_rect{other.m_context_rect},
              m_view{other.m_view},
              m_id{other.m_id},
              m_hovered{other.m_hovered},
              m_clicked{other.m_clicked}
              // No copy of lifetimes.
        {};

        Widget& operator=(Widget const& other)
        {
            m_rect = other.m_rect;
            m_context_rect = other.m_context_rect;
            m_view = other.m_view;
            m_id = other.m_id;
            m_hovered = other.m_hovered;
            m_clicked = other.m_clicked;
            // No compy of lifetimes.

            m_lifetimes.clear();

            return *this;
        };

        Widget(Widget&& other)
            : m_rect{other.m_rect},
              m_context_rect{other.m_context_rect},
              m_view{other.m_view},
              m_id{other.m_id},
              m_hovered{std::move(other.m_hovered)},
              m_clicked{std::move(other.m_clicked)},
              m_lifetimes{std::move(other.m_lifetimes)}
        {
            other.m_view = nullptr;
        };

        Widget& operator=(Widget&& other)
        {
            m_rect = other.m_rect;
            m_context_rect = other.m_context_rect;
            m_view = other.m_view;
            m_id = other.m_id;
            m_hovered = std::move(other.m_hovered);
            m_clicked = std::move(other.m_clicked);
            m_lifetimes = std::move(other.m_lifetimes);

            other.m_view = nullptr;

            return *this;
        };

        virtual ~Widget() = default;

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
        comm::Signal<Point, input::MouseHover> m_hovered{};
        comm::Signal<Point, input::MouseButton> m_clicked{};

        std::vector<comm::unsafe::AutodeleteLifetime> m_lifetimes;
    };

    struct WidgetState
    {
        bool m_hovered{false};
    };
}

#endif  // WIDGETS_WIDGET_HPP

