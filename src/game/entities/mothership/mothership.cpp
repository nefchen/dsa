/*
 * Created on 06.03.2021 by nefchen.
 */

#include "game/entities/mothership/mothership.hpp"


namespace game::mothership
{
    void Mothership::draw(SDL_Renderer* renderer)
    {
        if (m_current_image.m_texture == nullptr)
        {
            m_current_image = load_image(renderer, "ships/mothership_normal.png");
        }

        m_current_image.m_rect = m_rect;

        SDL_RenderCopy(
            renderer,
            m_current_image.m_texture.get(),
            nullptr,
            &m_current_image.m_rect
        );
    };
}

