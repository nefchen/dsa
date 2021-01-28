/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef VIEWS_START_SCREEN_HPP
#define VIEWS_START_SCREEN_HPP

#include <memory>

#include "view.hpp"
#include "widgets/label.hpp"
#include "../comm.hpp"
#include "../types.hpp"


namespace start_screen
{
    struct View: view::View
    {
        View(comm::WindowNode win_node);

        virtual void propagate_window_resize(Rect win_rect) override;

        std::shared_ptr<view::Label> m_title;
        std::shared_ptr<view::Label> m_version_number;
        //std::shared_ptr<view::Menu> m_version_number;
    };
}

#endif  // VIEWS_START_SCREEN_HPP

