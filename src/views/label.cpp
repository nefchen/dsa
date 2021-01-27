/*
 * Created on 24.01.2021 by nefchen.
 */

#include <SDL2/SDL.h>

#include "label.hpp"
#include "../fonts.hpp"


namespace view
{
    void Label::draw(SDL_Renderer* renderer)
    {
        m_rect.x = m_context_rect.w / 2 - m_rect.w / 2;
        auto font{load_font(m_font, m_font_size)};

        SDL_Surface* surf_msg{
            TTF_RenderText_Blended(font.get(), m_text.c_str(), m_text_color)
        };
        SDL_Texture* text_msg{
            SDL_CreateTextureFromSurface(renderer, surf_msg)
        };

        SDL_RenderCopy(renderer, text_msg, nullptr, &m_rect);
    };
}

