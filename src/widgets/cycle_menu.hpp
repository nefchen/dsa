/*
 * Created on 28.01.2021 by nefchen.
 */

#ifndef WIDGETS_CYCLE_MENU_HPP
#define WIDGETS_CYCLE_MENU_HPP

#include <vector>
#include <string>
#include <functional>

#include "widgets/label.hpp"
#include "widgets/widget.hpp"
#include "views/view.hpp"
#include "types/basic.hpp"
#include "types/input.hpp"
#include "types/sdl.hpp"


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

        void on_option_hover(u32 option_index, Point point, input::MouseHover hover);
        void on_option_clicked(input::MouseButton button, OptionCallback callback);

        std::vector<std::shared_ptr<Label>> m_labels;

        u8 m_option_font_size_on_hover{50};
        u8 m_option_font_size{40};
        u32 m_option_spacing{30};
    };
}

#endif  // WIDGETS_CYCLE_MENU_HPP

