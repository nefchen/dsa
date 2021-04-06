/*
 * Created on 29.03.2021 by nefchen.
 */

#include "game/simulation/simulation.hpp"
#include "game/simulation/positioning.hpp"


namespace game
{
    void Simulation::reset_to_initial_state()
    {
        m_entities.clear();
    };

    // Tries placing entity in simulation world anywhere within
    // the designated spawn_area.
    // NOTE: different placement implementations may yield
    // different results for the same configuration.
    bool Simulation::add_entity_to_simulation(
        std::unique_ptr<Entity> const& entity, Rect spawn_area)
    {
        auto entity_physics{create_entity_physics(entity)};
        auto collision_targets{find_entities_in_rect(spawn_area)};

        if (!place_entity_with_naive_spiral(spawn_area, entity_physics, collision_targets))
        {
            // No place found to put new entity.
            return false;
        }

        m_entities.push_back(std::move(entity_physics));

        return true;
    };

    std::vector<EntityPhysics*> Simulation::find_entities_in_rect(Rect rect)
    {
        std::vector<EntityPhysics*> entities_in_rect;

        for (auto& entity: m_entities)
        {
            entities_in_rect.push_back(&entity);
        }

        return entities_in_rect;
    };
}

