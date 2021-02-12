/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef WIDGETS_LABEL_HPP
#define WIDGETS_LABEL_HPP

#include <string>

#include "widgets/widget.hpp"
#include "text/fonts.hpp"
#include "types/basic.hpp"
#include "types/sdl.hpp"


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

#endif  // WIDGETS_LABEL_HPP

