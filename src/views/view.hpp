/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef VIEWS_VIEW_HPP
#define VIEWS_VIEW_HPP

#include <memory>

#include "widget.hpp"
#include "../comm.hpp"


namespace view
{
    struct View
    {
        View(comm::WindowNode win_node): m_win_node(std::move(win_node))
        {};

        std::vector<std::unique_ptr<Widget>> m_view_widgets;
        comm::WindowNode m_win_node;
    };
}

#endif  // VIEWS_VIEW_HPP

