/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef VIEWS_WIDGETS_LABEL_HPP
#define VIEWS_WIDGETS_LABEL_HPP

#include <SDL2/SDL.h>
#include <string>

#include "../widget.hpp"
#include "../../fonts.hpp"
#include "../../types.hpp"


namespace view
{
    struct Label: Widget
    {
        Label(View* view,
              std::string text = "",
              u8 font_size = 50,
              Color color = {0, 0, 0},
              Fonts font = Fonts::megrim)
            : Widget(view),
              m_text{std::move(text)},
              m_font_size{font_size},
              m_text_color{color},
              m_font{font}
        {
            fit_label_to_text();
        };

        virtual void draw(SDL_Renderer* renderer) override;
        void fit_label_to_text();

        std::string m_text;
        Color m_text_color;
        Fonts m_font;
        u8 m_font_size;
    };
}

#endif  // VIEWS_WIDGETS_LABEL_HPP

