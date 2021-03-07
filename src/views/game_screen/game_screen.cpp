/*
 * Created on 28.02.2021 by nefchen.
 */

#include "views/game_screen/game_screen.hpp"
#include "views/start_screen/start_screen.hpp"
#include "views/positioning.hpp"
#include "views/loader.hpp"
#include "types/basic.hpp"
#include "types/input.hpp"
#include "text/fonts.hpp"


namespace game_screen
{
    View::View(comm::Node comm_node): view::View(comm_node)
    {
        m_exit_label = std::make_shared<view::Label>(
            this,
            "Back",
            20,
            Color{255, 255, 255},
            Fonts::baloo
        );
        m_viewport = std::make_shared<view::Viewport>(this);

        insert_widget(m_viewport, nullptr);
        insert_widget(m_exit_label, nullptr);

        m_comm_node->create_game.emit();
        m_comm_node->add_viewport_handle_to_game.emit(m_viewport->m_handle);

        // Bind exit signal.
        m_lifetimes.push_back(
            comm::bind_autodelete_lifetime(
                m_exit_label->m_clicked.connect(
                    [comm_node = m_comm_node](Point p, input::MouseButton button) {
                        comm_node->load_view.emit(
                            {view::from_type<start_screen::View>{}, comm_node}
                        );
                    }
                ),
                m_exit_label->m_clicked
            )
        );
    };

    View::~View()
    {
        m_comm_node->exit_game.emit();
    };

    void View::resize(Point point)
    {
        Rect rect{0, 0, point.x, point.y};
        reposition_rect(
            m_exit_label->m_rect, rect,
            Align::right_top,
            {5, 10, 0, 0}
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

