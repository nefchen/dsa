/*
 * Created on 29.03.2021 by nefchen.
 */

#include "game/simulation/simulation.hpp"


namespace game
{
    void Simulation::reset_to_initial_state()
    {};

    bool Simulation::add_entity_to_simulation(
        std::unique_ptr<Entity> const& entity, bool relax_spawn_point)
    {
        return false;
    };
}

