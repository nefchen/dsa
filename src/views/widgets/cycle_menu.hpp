/*
 * Created on 28.01.2021 by nefchen.
 */

#ifndef VIEWS_WIDGETS_CYCLE_MENU_HPP
#define VIEWS_WIDGETS_CYCLE_MENU_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <functional>

#include "../widget.hpp"
#include "../view.hpp"
#include "label.hpp"


namespace view
{
    using MenuOption = std::pair<std::string, std::function<void(void)>>;
    struct CycleMenu: Widget
    {
        CycleMenu(View* view);

        virtual void draw(SDL_Renderer* renderer) override;

        void set_menu_options(std::vector<MenuOption>&& options);

        std::vector<std::shared_ptr<Label>> m_labels;
    };
}

#endif  // VIEWS_WIDGETS_CYCLE_MENU_HPP

