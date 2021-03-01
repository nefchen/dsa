/*
 * Created on 28.02.2021 by nefchen.
 */

#include "views/game_screen/game_screen.hpp"
#include "views/positioning.hpp"
#include "types/basic.hpp"


namespace game_screen
{
    View::View(comm::Node comm_node): view::View(comm_node)
    {
        m_title = std::make_shared<view::Label>(
            this, "Game Screen", 25, Color{0, 0, 0}
        );
        m_viewport = std::make_shared<view::Viewport>(this);

        insert_widget(m_viewport, nullptr);
        insert_widget(m_title, nullptr);

        m_comm_node->start_game.emit(m_viewport->m_handle);
    };

    void View::resize(Point point)
    {
        Rect rect{0, 0, point.x, point.y};
        reposition_rect(
            m_title->m_rect, rect,
            Align::left_top,
            {10, 0, 0, 10}
        );

        m_viewport->m_rect = rect;
        m_viewport->m_handle->m_rect = rect;
        reposition_rect(
            m_viewport->m_rect, rect,
            Align::left_top,
            {0, 0, 0, 0}
        );
    };
}

