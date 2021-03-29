/*
 * Created on 29.03.2021 by nefchen.
 */

#ifndef GAME_SIMULATION_SIMULATION_HPP
#define GAME_SIMULATION_SIMULATION_HPP

#include <memory>

#include "types/physics.hpp"
#include "game/entities/entity.hpp"


namespace game
{
    struct Simulation
    {
        physics::Size m_size;

        void reset_to_initial_state();

        // TODO: May be better an enum as return type.
        bool add_entity_to_simulation(
            std::unique_ptr<Entity> const& entity,
            bool relax_spawn_point = true
        );
    };
}

#endif  // GAME_SIMULATION_SIMULATION_HPP

