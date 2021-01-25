/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef VIEWS_LABEL_HPP
#define VIEWS_LABEL_HPP

#include <SDL2/SDL.h>
#include <string>

#include "widget.hpp"
#include "../fonts.hpp"
#include "../types.hpp"


namespace view
{
    struct Label: Widget
    {
        Label(std::string text, comm::WindowNode win_node)
            : m_text{std::move(text)}, Widget(std::move(win_node))
        {};

        virtual void draw(SDL_Renderer* renderer) override;

        std::string m_text{""};
        Color m_text_color{0, 0, 0};
        Fonts m_font{Fonts::megrim};
        u8 m_font_size{50};
    };
}

#endif  // VIEWS_LABEL_HPP

