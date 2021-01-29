/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef VIEWS_START_SCREEN_HPP
#define VIEWS_START_SCREEN_HPP

#include <memory>

#include "view.hpp"
#include "widgets/label.hpp"
#include "widgets/cycle_menu.hpp"
#include "../comm.hpp"
#include "../types.hpp"


namespace start_screen
{
    struct View: view::View
    {
        View(comm::Node comm_node);

        virtual void resize(Rect rect) override;
        void create_start_menu();

        std::shared_ptr<view::Label> m_title;
        std::shared_ptr<view::Label> m_version_num;
        std::shared_ptr<view::CycleMenu> m_start_menu;
    };
}

#endif  // VIEWS_START_SCREEN_HPP

