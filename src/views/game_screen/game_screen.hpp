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

        virtual ~View() override;
        virtual void resize(Point point) override;

        void start_basic_game_instance();

        std::shared_ptr<view::Label> m_exit_label;
        std::shared_ptr<view::Viewport> m_viewport;
        std::vector<comm::unsafe::AutodeleteLifetime> m_lifetimes;
    };
}

#endif  // VIEWS_GAME_SCREEN_GAME_SCREEN_HPP

