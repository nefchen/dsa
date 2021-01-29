/*
 * Created on 28.01.2021 by nefchen.
 */

#include <memory>

#include "start_screen.hpp"
#include "../types.hpp"


namespace start_screen
{
    View::View(comm::Node comm_node): view::View(comm_node)
    {
        m_title = std::make_shared<view::Label>(
            this, "Deadly Space Adventures", 60, Color{255, 255, 255}
        );
        m_title->m_rect.y = 50;

        m_version_num = std::make_shared<view::Label>(
            this, "version-0.1a", 20, Color{255, 255, 255}
        );
        m_version_num->m_rect.x = 20;

        insert_widget(m_title, nullptr);
        insert_widget(m_version_num, nullptr);

        create_start_menu();
    };

    void View::create_start_menu()
    {
        std::vector<view::MenuOption> options_of_menu{
                std::make_pair("New Game", [](){}),
                std::make_pair("Continue", [](){}),
                std::make_pair("Options", [](){})
        };

        m_start_menu = std::make_shared<view::CycleMenu>(this);
        insert_widget(m_start_menu, nullptr);

        m_start_menu->set_menu_options(std::move(options_of_menu));
    };

    void View::propagate_window_resize(Rect win_rect)
    {
        m_title->m_context_rect = win_rect;
        m_title->m_rect.x = win_rect.w / 2 - m_title->m_rect.w / 2;

        m_version_num->m_context_rect = win_rect;
        m_version_num->m_rect.y = win_rect.h - m_version_num->m_rect.h - 20;

        m_start_menu->m_context_rect = win_rect;
        m_start_menu->m_rect.x = win_rect.w / 2 - m_start_menu->m_rect.w / 2;
        m_start_menu->m_rect.y = win_rect.h / 2 - m_start_menu->m_rect.h / 2;
    };
}

