/*
 * Created on 28.02.2021 by nefchen.
 */

#ifndef VIEWS_GAME_SCREEN_GAME_SCREEN_HPP
#define VIEWS_GAME_SCREEN_GAME_SCREEN_HPP

#include <memory>

#include "views/view.hpp"
#include "widgets/label.hpp"
#include "widgets/viewport.hpp"
#include "comm/comm.hpp"


namespace game_screen
{
    struct View: view::View
    {
        View(comm::Node comm_node);

        virtual void resize(Point point) override;

        std::shared_ptr<view::Label> m_title;
        std::shared_ptr<view::Viewport> m_viewport;
    };
}

#endif  // VIEWS_GAME_SCREEN_GAME_SCREEN_HPP

