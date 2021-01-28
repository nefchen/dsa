/*
 * Created on 28.01.2021 by nefchen.
 */

#include <memory>

#include "start_screen.hpp"
#include "../types.hpp"


namespace start_screen
{
    View::View(comm::WindowNode win_node): view::View(win_node)
    {
        m_title = std::make_shared<view::Label>(
            this, "Deadly Space Adventures", 50, Color{255, 255, 255}
        );
        m_title->m_rect.y = 50;

        m_version_number = std::make_shared<view::Label>(
            this, "version-0.1a", 20, Color{255, 255, 255}
        );
        m_version_number->m_rect.x = 20;

        m_view_widgets.push_back(m_title);
        m_view_widgets.push_back(m_version_number);
    };

    void View::propagate_window_resize(Rect win_rect)
    {
        m_title->m_context_rect = win_rect;
        m_title->m_rect.x = win_rect.w / 2 - m_title->m_rect.w / 2;

        m_version_number->m_context_rect = win_rect;
        m_version_number->m_rect.y = win_rect.h - m_version_number->m_rect.h - 20;
    };
}

