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
    // Entity contains all shared attributes (attributes used by
    // other systems) of every entity type.
    struct Entity
    {
        Id m_id;

        // Rendering attributes.
        bool m_renderable{true};
        Rect m_rect;
        Point m_pos;
        virtual void draw(SDL_Renderer* r)
        {};

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

