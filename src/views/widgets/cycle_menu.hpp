/*
 * Created on 28.01.2021 by nefchen.
 */

#ifndef VIEWS_WIDGETS_CYCLE_MENU_HPP
#define VIEWS_WIDGETS_CYCLE_MENU_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <functional>

#include "label.hpp"
#include "../widget.hpp"
#include "../view.hpp"
#include "../../types.hpp"


namespace view
{
    using OptionCallback = std::function<void(void)>;
    using MenuOption = std::pair<std::string, OptionCallback>;

    struct CycleMenu: Widget
    {
        CycleMenu(View* view);

        virtual void draw(SDL_Renderer* renderer) override;
        virtual void resize(Rect rect) override;

        void set_menu_options(std::vector<MenuOption>&& options);
        void connect_signals_of_option(u32 option_index, OptionCallback callback);

        void on_option_hover(u32 option_index, Point point, Hover hover);

        std::vector<std::shared_ptr<Label>> m_labels;

        u8 m_option_font_size_on_hover{50};
        u8 m_option_font_size{40};
        u32 m_option_spacing{30};
    };
}

#endif  // VIEWS_WIDGETS_CYCLE_MENU_HPP

