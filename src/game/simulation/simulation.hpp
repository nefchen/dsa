/*
 * Created on 29.03.2021 by nefchen.
 */

#ifndef GAME_SIMULATION_SIMULATION_HPP
#define GAME_SIMULATION_SIMULATION_HPP

#include <memory>
#include <vector>

#include "types/basic.hpp"
#include "types/physics.hpp"
#include "game/entities/entity.hpp"


namespace game
{
    // Proxy object that contains references to physics related
    // information of a particular entity.
    struct EntityPhysics
    {
        Id& m_id;

        physics::Acceleration& m_acceleration;
        physics::Velocity& m_velocity;
        physics::Position& m_position;
        physics::Size& m_size;

        std::vector<Rect>& m_collision_rects;
    };

    struct Simulation
    {
        void reset_to_initial_state();
        std::vector<EntityPhysics*> find_entities_in_rect(Rect rect);
        bool add_entity_to_simulation(
            std::unique_ptr<Entity> const& entity,
            Rect spawn_area
        );

        physics::Size m_size;
        std::vector<EntityPhysics> m_entities;
    };

    inline EntityPhysics create_entity_physics(std::unique_ptr<Entity> const& entity)
    {
        return {
            .m_id = entity->m_id,
            .m_acceleration = entity->m_simul_acceleration,
            .m_velocity = entity->m_simul_velocity,
            .m_position = entity->m_simul_position,
            .m_size = entity->m_simul_size,
            .m_collision_rects = entity->m_collision_rects
        };
    };
}

#endif  // GAME_SIMULATION_SIMULATION_HPP

