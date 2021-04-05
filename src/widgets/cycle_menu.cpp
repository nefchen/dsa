/*
 * Created on 28.01.2021 by nefchen.
 */

#include <algorithm>

#include "widgets/cycle_menu.hpp"
#include "widgets/label.hpp"
#include "views/positioning.hpp"
#include "types/input.hpp"
#include "types/basic.hpp"
#include "types/sdl.hpp"


namespace view
{
    CycleMenu::CycleMenu(View* view): Widget(view)
    {
        m_rect = {0, 0, 200, 200};
    };

    void CycleMenu::set_menu_options(std::vector<MenuOption>&& options)
    {
        for (u32 opt_idx = 0; opt_idx < options.size(); ++opt_idx)
        {
            auto& opt{options.at(opt_idx)};
            auto opt_label{
                std::make_shared<Label>(
                    m_view, opt.first,
                    m_option_font_size,
                    Color{255, 255, 255}
                )
            };

            m_labels.push_back(opt_label);
            m_view->insert_widget(opt_label, this);

            connect_signals_of_option(opt_idx, opt.second);
        }
    };

    void CycleMenu::connect_signals_of_option(
        u32 opt_index, OptionCallback callback)
    {
        m_lifetimes.push_back(
            comm::unsafe::bind_autodelete_lifetime(
                m_labels.at(opt_index)->m_hovered.connect(
                    [this, opt_index] (Point point, input::MouseHover hover) {
                        this->on_option_hover(opt_index, point, hover);
                    }
                ),
                m_labels.at(opt_index)->m_hovered
            )
        );
        m_lifetimes.push_back(
            comm::unsafe::bind_autodelete_lifetime(
                m_labels.at(opt_index)->m_clicked.connect(
                    [this, callback] (Point point, input::MouseButton button) {
                        this->on_option_clicked(button, callback);
                    }
                ),
                m_labels.at(opt_index)->m_clicked
            )
        );
    };

    void CycleMenu::on_option_hover(u32 option_idx, Point p, input::MouseHover hover)
    {
        auto& label{m_labels.at(option_idx)};

        switch (hover)
        {
            case input::MouseHover::enter:
                // Fall through.
            case input::MouseHover::keep:
                label->m_font_size = m_option_font_size_on_hover;
                label->fit_label_to_text();
                break;
            case input::MouseHover::leave:
                label->m_font_size = m_option_font_size;
                label->fit_label_to_text();
                break;
        }

        resize(m_context_rect);
    };

    void CycleMenu::on_option_clicked(input::MouseButton button, OptionCallback callback)
    {
        if (button == input::MouseButton::left)
        {
            callback();
        }
    }

    void CycleMenu::resize(Rect rect)
    {
        m_context_rect = rect;

        // Find longest string in options.
        auto longest_label{
            std::max_element(
                m_labels.begin(), m_labels.end(),
                [](auto& lhs, auto& rhs) {
                    return lhs->m_text.length() < rhs->m_text.length();
                }
            )
        };

        Label reference_label{
            nullptr, (*longest_label)->m_text, m_option_font_size_on_hover
        };
        m_rect.w = reference_label.m_rect.w;
        m_rect.h = reference_label.m_rect.h * m_labels.size();
        m_rect.h += m_option_spacing * m_labels.size();

        auto label_height{0};
        for (auto& label : m_labels)
        {
            reposition_rect(
                label->m_rect, m_rect, Align::center_top,
                {label_height, 0, 0, 0}
            );
            label_height += m_rect.h / m_labels.size();
        };
    };

    void CycleMenu::draw(SDL_Renderer* renderer)
    {};
}

