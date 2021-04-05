/*
 * Created on 28.03.2021 by nefchen.
 */

#ifndef GAME_SIMULATION_POSITIONING_HPP
#define GAME_SIMULATION_POSITIONING_HPP

#include <vector>
#include <algorithm>
#include <cmath>

#include "game/simulation/simulation.hpp"
#include "types/basic.hpp"


namespace game
{
    // Distribute num_centers inside rect.
    std::vector<Rect> create_sparse_regions_in_rect(Rect rect, u32 num_centers);

    // NOTE: precision up to integer level.
    std::vector<EntityPhysics*> get_colliding_entities(
        EntityPhysics const& entity, std::vector<EntityPhysics*> const& collision_targets);

    // Algorithm to find a location for a rect inside another.
    // The position is found by spiraling radially from the center
    // of the spawn_area. Returns false if the area is fully occupied.
    bool place_entity_with_naive_spiral(
        Rect spawn_area,
        EntityPhysics& entity,
        std::vector<EntityPhysics*> collision_targets = {});

    inline bool point_in_rect(const Point& p, const Rect& r) noexcept
    {
        return p.x >= r.x && p.x <= r.x + r.w && p.y >= r.y && p.y <= r.y + r.h;
    };
}

#endif  // GAME_SIMULATION_POSITIONING_HPP

