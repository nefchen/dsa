/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef VIEWS_VIEW_HPP
#define VIEWS_VIEW_HPP

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <map>

#include "widget.hpp"
#include "../comm.hpp"
#include "../types.hpp"


namespace view
{
    struct View
    {
        View(comm::Node comm_node): m_comm_node(std::move(comm_node))
        {};

        virtual void propagate_window_resize(Rect win_size)
        {
            for (auto& w: m_widgets)
            {
                w->m_context_rect = win_size;
            }
        };

        const std::vector<std::shared_ptr<Widget>>& widgets()
        {
            return m_widgets;
        };

        void insert_widget(std::shared_ptr<Widget> widget, const Widget* parent);
        void remove_widget(std::shared_ptr<Widget> widget, const Widget* parent);
        void render(SDL_Renderer* renderer, Rect scope);

        comm::Node m_comm_node;

        private:
            std::vector<std::shared_ptr<Widget>> m_widgets;
            std::vector<Rect> m_widget_scopes;
            std::map<Id, u8> m_layers;
    };
}

#endif  // VIEWS_VIEW_HPP

