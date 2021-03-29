/*
 * Created on 06.03.2021 by nefchen.
 */

#ifndef GAME_ENTITIES_MOTHERSHIP_MOTHERSHIP_HPP
#define GAME_ENTITIES_MOTHERSHIP_MOTHERSHIP_HPP

#include <memory>

#include "game/entities/entity.hpp"
#include "assets/images.hpp"


namespace game::mothership
{
    struct Mothership: Entity
    {
        virtual void draw(SDL_Renderer* renderer) override;

        Image m_current_image;
    };

    inline std::unique_ptr<Entity> create_entity()
    {
        return std::make_unique<Mothership>();
    }
}

#endif  // GAME_ENTITIES_MOTHERSHIP_MOTHERSHIP_HPP

