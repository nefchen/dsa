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

