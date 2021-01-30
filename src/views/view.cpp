/*
*std::max() * Created on 28.01.2021 by nefchen.
 */

#include <SDL2/SDL.h>
#include <memory>
#include <algorithm>

#include "widget.hpp"
#include "view.hpp"
#include "../types.hpp"
#include "../utils.hpp"


namespace view
{
    View::View(comm::Node comm_node): m_comm_node(std::move(comm_node))
    {
        m_signal_ds.push_back(
            m_comm_node->mouse_moved.connect(
                [this](Point p) { this->propagate_mouse_move(p); }
            )
        );

        m_signal_ds.push_back(
            m_comm_node->mouse_button_clicked.connect(
                [this](Point p, mouse::Button b) {
                    this->propagate_mouse_click(p, b);
                }
            )
        );
    };

    void View::insert_widget(std::shared_ptr<Widget> widget, const Widget* parent)
    {
        widget->m_id = get_new_widget_id();
        m_widget_state.insert(std::make_pair(widget->m_id, WidgetState{}));

        if (!parent || find_widget_pos(parent->m_id) == m_widgets.size())
        {
            // No parent or parent not in view.
            m_layers.insert(std::make_pair(widget->m_id, 0));
            m_widgets.push_back(std::move(widget));
        }
        else
        {
            m_layers.insert(
                std::make_pair(widget->m_id, m_layers.at(parent->m_id) + 1)
            );
            m_widgets.insert(
                m_widgets.begin() + find_widget_pos(parent->m_id) + 1,
                std::move(widget)
            );
        }

        m_widget_scopes.clear();
        auto total_scopes{
            std::max_element(
                m_layers.begin(),
                m_layers.end(),
                [](auto& lhs, auto& rhs) {
                    return lhs.second < rhs.second;
                }
            )->second + 1
        };

        for (u32 i = 0; i <= total_scopes; ++i)
        {
            m_widget_scopes.push_back({0, 0, 0, 0});
        }
    };

    void View::propagate_mouse_click(Point point, mouse::Button button)
    {
        m_widget_scopes.at(0) = {0, 0, 0, 0};

        for (auto& widget : m_widgets)
        {
            auto& w_state{m_widget_state.at(widget->m_id)};

            // We need to compare mouse position to widget rect
            // in absolute coordinates (relative to window).
            auto w_layer{m_layers.at(widget->m_id)};
            auto real_rect{
                rect_in_absolute_origin(
                    widget->m_rect,
                    m_widget_scopes.at(w_layer)
                )
            };
            m_widget_scopes.at(w_layer + 1) = real_rect;

            if (point_in_rect(point, real_rect))
            {
                widget->m_clicked.emit(point, button);
            }
        }
    };

    void View::propagate_mouse_move(Point point)
    {
        m_widget_scopes.at(0) = {0, 0, 0, 0};

        for (auto& widget : m_widgets)
        {
            auto& w_state{m_widget_state.at(widget->m_id)};

            // We need to compare mouse position to widget rect
            // in absolute coordinates (relative to window).
            auto w_layer{m_layers.at(widget->m_id)};
            auto real_rect{
                rect_in_absolute_origin(
                    widget->m_rect,
                    m_widget_scopes.at(w_layer)
                )
            };
            m_widget_scopes.at(w_layer + 1) = real_rect;

            if (point_in_rect(point, real_rect))
            {
                if (!w_state.m_hovered)
                {
                    widget->m_mouse_hover_signal.emit(
                        relative_point_to_rect(point, widget->m_rect),
                        Hover::enter
                    );
                    w_state.m_hovered = true;
                }
                else
                {
                    widget->m_mouse_hover_signal.emit(
                        relative_point_to_rect(point, widget->m_rect),
                        Hover::keep
                    );
                }
            }
            else if (m_widget_state.at(widget->m_id).m_hovered)
            {
                // Relative position may yield negative values.
                widget->m_mouse_hover_signal.emit(
                    relative_point_to_rect(point, widget->m_rect),
                    Hover::leave
                );
                m_widget_state.at(widget->m_id).m_hovered = false;
            }
        }
    };

    void View::propagate_resize(Rect rect, std::shared_ptr<Widget> widget)
    {
        m_widget_scopes.at(0) = {0, 0, rect.w, rect.h};
        u32 next_widget_pos{0};
        u8 starting_widget_layer{0};

        if (widget != nullptr)
        {
            // We want to resize childs of this.
            auto position_of_widget{find_widget_pos(widget->m_id)};

            // If widget is not in view or is last element of it
            // no need to propagate.
            if (position_of_widget >= m_widgets.size() - 1)
            {
                return;
            }

            next_widget_pos = position_of_widget + 1;
            starting_widget_layer = m_layers.at(widget->m_id);
            m_widget_scopes.at(starting_widget_layer + 1) = widget->m_rect;
        }

        for (u32 i = next_widget_pos; i < m_widgets.size(); ++i)
        {
            auto curr_widget{m_widgets.at(i)};
            auto curr_layer{m_layers.at(curr_widget->m_id)};

            if (curr_layer > starting_widget_layer || widget == nullptr)
            {
                curr_widget->resize(m_widget_scopes.at(curr_layer));
                m_widget_scopes.at(curr_layer + 1) = curr_widget->m_rect;
            }
            else
            {
                break;
            }
        }

        if (widget == nullptr)
        {
            // If the window is resizing we may want to
            // reorganize the view.
            resize(rect);
        }
    };

    void View::render(SDL_Renderer* renderer, Rect scope)
    {
        m_widget_scopes.at(0) = {0, 0, scope.w, scope.h};

        for (auto& widget : m_widgets)
        {
            auto w_layer = m_layers.at(widget->m_id);
            auto w_scope = m_widget_scopes.at(w_layer);

            SDL_RenderSetViewport(renderer, &w_scope);

            widget->draw(renderer);

            m_widget_scopes.at(w_layer + 1) = {
                w_scope.x + widget->m_rect.x,
                w_scope.y + widget->m_rect.y,
                widget->m_rect.w,
                widget->m_rect.h
            };
        }
    };
}

