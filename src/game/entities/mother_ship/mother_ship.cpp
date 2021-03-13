/*
 * Created on 06.03.2021 by nefchen.
 */

#include "game/entities/mother_ship/mother_ship.hpp"


namespace game::mother_ship
{
    void MotherShip::draw(SDL_Renderer* renderer)
    {
        if (m_current_image.m_texture == nullptr)
        {
            m_current_image.m_texture = load_image(
                renderer, "ships/mother_ship_normal.png"
            );
            m_current_image.m_rect = {50, 50, 50, 67};
        }

        SDL_RenderCopy(
            renderer,
            m_current_image.m_texture.get(),
            NULL,
            &m_current_image.m_rect
        );
    };
}

