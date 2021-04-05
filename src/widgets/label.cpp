/*
 * Created on 24.01.2021 by nefchen.
 */

#include "widgets/label.hpp"
#include "types/sdl.hpp"


namespace view
{
    void Label::draw(SDL_Renderer* renderer)
    {
        if (m_text_texture == nullptr)
        {
            sdl::Surface surf_msg{
                TTF_RenderText_Blended(m_loaded_font.get(), m_text.c_str(), m_text_color)
            };
            sdl::Texture text_msg{
                SDL_CreateTextureFromSurface(renderer, surf_msg.get())
            };

            m_text_texture = std::move(text_msg);
        }

        SDL_RenderCopy(renderer, m_text_texture.get(), nullptr, &m_rect);
    };

    void Label::fit_label_to_text()
    {
        m_loaded_font = load_font(m_font, m_font_size);
        TTF_SizeText(m_loaded_font.get(), m_text.c_str(), &m_rect.w, &m_rect.h);

        m_text_texture = nullptr;
    };
}

