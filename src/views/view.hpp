/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef VIEWS_VIEW_HPP
#define VIEWS_VIEW_HPP

#include <memory>

#include "widget.hpp"
#include "../comm.hpp"
#include "../types.hpp"


namespace view
{
    struct View
    {
        View(comm::WindowNode win_node): m_win_node(std::move(win_node))
        {};

        void propagate_window_resize(Rect win_size)
        {
            for (auto& w: m_view_widgets)
            {
                w->m_context_rect = win_size;
            }
        };

        std::vector<std::unique_ptr<Widget>> m_view_widgets;
        comm::WindowNode m_win_node;
    };
}

#endif  // VIEWS_VIEW_HPP

