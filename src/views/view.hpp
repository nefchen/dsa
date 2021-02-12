/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef VIEWS_VIEW_HPP
#define VIEWS_VIEW_HPP

#include <algorithm>
#include <memory>
#include <vector>
#include <map>

#include "widgets/widget.hpp"
#include "comm/comm.hpp"
#include "types/input.hpp"
#include "types/basic.hpp"
#include "types/sdl.hpp"


namespace view
{
    struct View
    {
        View(comm::Node comm_node);

        inline const std::vector<std::shared_ptr<Widget>>& widgets() const
        {
            return m_widgets;
        };

        inline auto find_widget_pos(Id w_id) const
        {
            return std::distance(
                m_widgets.begin(),
                std::find_if(
                    m_widgets.begin(),
                    m_widgets.end(),
                    [w_id](auto& w) {
                        return w_id == w->m_id;
                    }
                )
            );
        };

        inline u32 get_new_widget_id()
        {
            return m_current_widget_count++;
        };

        virtual void resize(Point point)
        {};

        void propagate_mouse_move(Point point);
        void propagate_mouse_click(Point point, input::MouseButton button, u8 clicks);
        void propagate_resize(Point point, std::shared_ptr<Widget> widget = nullptr);

        void insert_widget(std::shared_ptr<Widget> widget, const Widget* parent);
        void remove_widget(std::shared_ptr<Widget> widget, const Widget* parent);

        void render(SDL_Renderer* renderer);

        comm::Node m_comm_node;

        private:
            std::vector<std::shared_ptr<Widget>> m_widgets;
            std::vector<Rect> m_rect_cache;
            std::map<Id, u8> m_layers;
            std::map<Id, WidgetState> m_widget_state;
            u32 m_current_widget_count{0};
            Rect m_rect;
    };
}

#endif  // VIEWS_VIEW_HPP

