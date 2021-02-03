/*
 * Created on 24.01.2021 by nefchen.
 */

#include <SDL2/SDL.h>

#include "views/widgets/label.hpp"
#include "fonts.hpp"


namespace view
{
    void Label::draw(SDL_Renderer* renderer)
    {
        auto font{load_font(m_font, m_font_size)};

        SDL_Surface* surf_msg{
            TTF_RenderText_Blended(font.get(), m_text.c_str(), m_text_color)
        };
        SDL_Texture* text_msg{
            SDL_CreateTextureFromSurface(renderer, surf_msg)
        };

        SDL_RenderCopy(renderer, text_msg, nullptr, &m_rect);
        SDL_FreeSurface(surf_msg);
        SDL_DestroyTexture(text_msg);
    };

    void Label::fit_label_to_text()
    {
        auto font{load_font(m_font, m_font_size)};
        TTF_SizeText(font.get(), m_text.c_str(), &m_rect.w, &m_rect.h);
    };
}

