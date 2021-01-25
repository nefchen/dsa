/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef VIEWS_GAME_WAIT_DIALOG_HPP
#define VIEWS_GAME_WAIT_DIALOG_HPP

#include "view.hpp"
#include "label.hpp"
#include "../comm.hpp"


namespace game_wait_dialog
{
    struct View: view::View
    {
        View(comm::WindowNode win_node): view::View(win_node)
        {
            view::Label label{"Click to start Game", m_win_node};
            label.m_rect = {50, 50, 500, 50};
            label.m_text_color = {255, 255, 255};

            m_view_widgets.push_back(
                std::make_unique<view::Label>(std::move(label))
            );
        };
    };
}

#endif  // VIEWS_GAME_WAIT_DIALOG_HPP

