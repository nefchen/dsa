/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef VIEWS_START_SCREEN_START_SCREEN_HPP
#define VIEWS_START_SCREEN_START_SCREEN_HPP

#include <memory>

#include "views/view.hpp"
#include "widgets/label.hpp"
#include "widgets/cycle_menu.hpp"
#include "comm/comm.hpp"
#include "types/basic.hpp"


namespace start_screen
{
    struct View: view::View
    {
        View(comm::Node comm_node);

        virtual void resize(Point point) override;
        void create_start_menu();

        std::shared_ptr<view::Label> m_title;
        std::shared_ptr<view::Label> m_version_num;
        std::shared_ptr<view::CycleMenu> m_start_menu;
    };
}

#endif  // VIEWS_START_SCREEN_START_SCREEN_HPP

