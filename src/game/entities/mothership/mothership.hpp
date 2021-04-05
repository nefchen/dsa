/*
 * Created on 06.03.2021 by nefchen.
 */

#ifndef GAME_ENTITIES_MOTHERSHIP_MOTHERSHIP_HPP
#define GAME_ENTITIES_MOTHERSHIP_MOTHERSHIP_HPP

#include <memory>

#include "types/basic.hpp"
#include "types/physics.hpp"
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
        auto entity{std::make_unique<Mothership>()};

        entity->m_simul_size = {100, 133, 0};
        entity->m_collision_rects = {Rect{0, 0, 100, 133}};

        return entity;
    }
}

#endif  // GAME_ENTITIES_MOTHERSHIP_MOTHERSHIP_HPP

