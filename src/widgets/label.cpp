/*
 * Created on 24.01.2021 by nefchen.
 */

#include "widgets/label.hpp"
#include "types/sdl.hpp"


namespace view
{
    void Label::draw(SDL_Renderer* renderer)
    {
        auto font{load_font(m_font, m_font_size)};

        sdl::Surface surf_msg{
            TTF_RenderText_Blended(font.get(), m_text.c_str(), m_text_color)
        };
        sdl::Texture text_msg{
            SDL_CreateTextureFromSurface(renderer, surf_msg.get())
        };

        SDL_RenderCopy(renderer, text_msg.get(), nullptr, &m_rect);
    };

    void Label::fit_label_to_text()
    {
        auto font{load_font(m_font, m_font_size)};
        TTF_SizeText(font.get(), m_text.c_str(), &m_rect.w, &m_rect.h);
    };
}

