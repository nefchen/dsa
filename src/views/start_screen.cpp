/*
 * Created on 28.01.2021 by nefchen.
 */

#include <memory>
#include <iostream>

#include "start_screen.hpp"
#include "../types.hpp"
#include "../utils.hpp"


namespace start_screen
{
    View::View(comm::Node comm_node): view::View(comm_node)
    {
        m_title = std::make_shared<view::Label>(
            this, "Deadly Space Adventures", 60, Color{255, 255, 255}
        );

        m_version_num = std::make_shared<view::Label>(
            this, "version-0.1a", 20, Color{255, 255, 255}
        );

        insert_widget(m_title, nullptr);
        insert_widget(m_version_num, nullptr);

        create_start_menu();
    };

    void View::create_start_menu()
    {
        std::vector<view::MenuOption> options_of_menu{
                std::make_pair(
                    "New Game", []() { std::cout << "Entering New Game\n"; }
                ),
                std::make_pair(
                    "Continue", []() { std::cout << "Entering Continue\n"; }
                ),
                std::make_pair(
                    "Options", []() { std::cout << "Entering Options\n"; }
                )
        };

        m_start_menu = std::make_shared<view::CycleMenu>(this);
        insert_widget(m_start_menu, nullptr);

        m_start_menu->set_menu_options(std::move(options_of_menu));
    };

    void View::resize(Rect rect)
    {
        reposition_rect(
            m_title->m_rect, rect,
            Align::center_top,
            {50, 0, 0, 0}
        );
        reposition_rect(
            m_version_num->m_rect, rect,
            Align::left_bottom,
            {0, 0, 20, 20}
        );
        reposition_rect(m_start_menu->m_rect, rect, Align::center);
    };
}

