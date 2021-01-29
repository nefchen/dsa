/*
*std::max() * Created on 28.01.2021 by nefchen.
 */

#include <SDL2/SDL.h>
#include <memory>
#include <algorithm>

#include "widget.hpp"
#include "view.hpp"


namespace view
{
    void View::insert_widget(std::shared_ptr<Widget> widget, const Widget* parent)
    {
        auto find_widget{
            [this](Id w_id) {
                return std::find_if(
                    this->m_widgets.begin(),
                    this->m_widgets.end(),
                    [w_id](auto& w) { return w->m_id == w_id; }
                );
            }
        };

        // Already present in view, no duplication.
        if (find_widget(widget->m_id) != m_widgets.end())
        {
            return;
        }

        if (!parent || find_widget(parent->m_id) == m_widgets.end())
        {
            // No parent or parent not in view.
            m_layers.insert(std::make_pair(widget->m_id, 0));
            m_widgets.push_back(std::move(widget));
        }
        else
        {
            // Parent present.
            auto parent_it{find_widget(parent->m_id)};

            m_layers.insert(
                std::make_pair(
                    widget->m_id,
                    m_layers.at((*parent_it)->m_id) + 1
                )
            );
            m_widgets.insert(std::next(parent_it), std::move(widget));
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

