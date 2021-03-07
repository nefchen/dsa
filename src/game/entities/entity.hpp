/*
 * Created on 06.03.2021 by nefchen.
 */

#ifndef GAME_ENTITIES_ENTITY_HPP
#define GAME_ENTITIES_ENTITY_HPP

#include <vector>
#include <functional>

#include "types/basic.hpp"
#include "types/physics.hpp"
#include "types/sdl.hpp"


namespace game
{
    inline SDL_Surface* default_draw(SDL_Renderer* r)
    {
        return nullptr;
    };

    // Entity is an aggregate with all shared attributes (attributes
    // used by other systems) of all entities. The behavior of all
    // entities is fully defined by this aggregate.
    struct Entity
    {
        // Rendering attributes.
        Rect m_rect;
        Point m_pos;
        std::function<SDL_Surface*(SDL_Renderer*)> m_draw{default_draw};

        // Physical quantities in simulation.
        physics::Acceleration m_simul_acceleration;
        physics::Velocity m_simul_velocity;
        physics::Position m_simul_position;
        physics::Size m_simul_size;

        // Collision attributes based on physics size, since this
        // value doesn't change.
        std::vector<Rect> m_collision_rects;
    };
}

#endif  // GAME_ENTITIES_ENTITY_HPP

