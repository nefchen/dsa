/*
 * Created on 28.01.2021 by nefchen.
 */

#include <SDL2/SDL.h>
#include <algorithm>

#include "cycle_menu.hpp"
#include "label.hpp"


namespace view
{
    CycleMenu::CycleMenu(View* view): Widget(view)
    {
        m_rect = {0, 0, 200, 200};
    };

    void CycleMenu::set_menu_options(std::vector<MenuOption>&& options)
    {
        for (auto& opt: options)
        {
            auto opt_label{
                std::make_shared<Label>(
                    m_view, opt.first, 40, Color{255, 255, 255}
                )
            };

            m_labels.push_back(opt_label);
            m_view->insert_widget(opt_label, this);
        }

        // Resize menu to fit labels.
        auto max_w{
            (*std::max_element(
                m_labels.begin(),
                m_labels.end(),
                [](auto& lhs, auto& rhs) {
                    return lhs->m_rect.w < rhs->m_rect.w;
                }
            ))->m_rect.w
        };
        auto max_h{
            (*std::max_element(
                m_labels.begin(),
                m_labels.end(),
                [](auto& lhs, auto& rhs) {
                    return lhs->m_rect.h < rhs->m_rect.h;
                }
            ))->m_rect.h
        };

        m_rect.w = max_w + 100;
        m_rect.h = max_w + 20 * m_labels.size();

        m_labels.at(0)->m_font_size = 50;
        m_labels.at(0)->fit_label_to_text();
    };

    void CycleMenu::draw(SDL_Renderer* renderer)
    {
        auto labels_pos_y{20};

        for (auto& label: m_labels)
        {
            label->m_rect.y = labels_pos_y - label->m_rect.h / 2;
            label->m_rect.x = m_rect.w / 2 - label->m_rect.w / 2;
            labels_pos_y += m_rect.h / m_labels.size();
        }
    };
}

