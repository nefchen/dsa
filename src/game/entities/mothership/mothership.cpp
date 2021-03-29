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
            m_current_image.m_texture = load_image(
                renderer, "ships/mothership_normal.png"
            );
            m_current_image.m_rect = {50, 50, 50, 67};
        }

        m_current_image.m_rect.x = 50 + m_rect.x;
        m_current_image.m_rect.y = 50 + m_rect.y;

        SDL_RenderCopy(
            renderer,
            m_current_image.m_texture.get(),
            NULL,
            &m_current_image.m_rect
        );
    };
}

