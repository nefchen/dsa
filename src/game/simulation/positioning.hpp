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
#include "types/physics.hpp"


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

    inline bool point_in_rect(Point const& p, Rect const& r) noexcept
    {
        return p.m_x >= r.x && p.m_x <= r.x + r.w && p.m_y >= r.y && p.m_y <= r.y + r.h;
    };

    inline bool small_rect_crosses_large_rect(Rect const& sr, Rect const& lr) noexcept
    {
        return
            // Top left point of small rect.
            (sr.x >= lr.x && sr.x <= lr.x + lr.w) &&
            (sr.y >= lr.y && sr.y <= lr.y + lr.h) ||
            // Top right point of small rect.
            (sr.x + sr.w >= lr.x && sr.x + sr.w <= lr.x + lr.w) &&
            (sr.y >= lr.y && sr.y <= lr.y + lr.h) ||
            // Bottom left point of small rect.
            (sr.x >= lr.x && sr.x <= lr.x + lr.w) &&
            (sr.y + sr.h >= lr.y && sr.y + sr.h <= lr.y + lr.h) ||
            // Bottom right point of small rect.
            (sr.x + sr.w >= lr.x && sr.x + sr.w <= lr.x + lr.w) &&
            (sr.y + sr.h >= lr.y && sr.y + sr.h <= lr.y + lr.h);
    };

    inline double clamp_value(const double d, Range const& rng) noexcept
    {
        return std::min(rng.m_max, std::max(rng.m_min, d));
    };
}

#endif  // GAME_SIMULATION_POSITIONING_HPP

