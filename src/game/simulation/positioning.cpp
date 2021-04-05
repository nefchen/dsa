/*
 * Created on 05.04.2021 by nefchen.
 */

#include "game/simulation/positioning.hpp"


namespace game
{
    std::vector<Rect> create_sparse_regions_in_rect(Rect rect, u32 num_centers)
    {
        if (num_centers == 0)
        {
            return {};
        }

        // Divide rectangle into a grid to fit all points.
        auto num_splits_per_side{
            static_cast<int>(std::ceil(std::sqrt(num_centers)))
        };
        auto w_split_size{
            static_cast<int>(rect.w / num_splits_per_side)
        };
        auto h_split_size{
            static_cast<int>(rect.h / num_splits_per_side)
        };

        std::vector<Rect> center_regions;
        for (int i = 0; i < num_centers; ++i)
        {
            Rect new_region{
                .x = (i % num_splits_per_side) * w_split_size,
                .y = static_cast<int>(std::floor(i / num_splits_per_side)) * h_split_size,
                .w = w_split_size,
                .h = h_split_size
            };

            // Adjust casting inaccuracy.
            if (new_region.x + new_region.w > rect.w)
            {
                new_region.w += new_region.x - rect.w;
            }
            if (new_region.y + new_region.h > rect.h)
            {
                new_region.h += new_region.y - rect.h;
            }

            // Re-center with respect to parent.
            new_region.x += rect.x;
            new_region.y += rect.y;

            center_regions.push_back(new_region);
        }

        return center_regions;
    }

    std::vector<EntityPhysics*> get_colliding_entities(
        EntityPhysics const& entity, std::vector<EntityPhysics*> const& collision_targets)
    {
        std::vector<EntityPhysics*> colliding_entities{};

        for (auto& target_entity: collision_targets)
        {
            auto collision_verified{false};
            // Copy in iteration!
            for (auto r_t: target_entity->m_collision_rects)
            {
                r_t.x += static_cast<int>(target_entity->m_position.m_x);
                r_t.y += static_cast<int>(target_entity->m_position.m_y);

                // Copy in iteration!
                for (auto r_o: entity.m_collision_rects)
                {
                    r_o.x += static_cast<int>(entity.m_position.m_x);
                    r_o.y += static_cast<int>(entity.m_position.m_y);

                    // Forward check for every vertice of origin rect (r_o).
                    if (point_in_rect({r_o.x, r_o.y}, r_t) ||
                        point_in_rect({r_o.x + r_o.w, r_o.y}, r_t) ||
                        point_in_rect({r_o.x + r_o.w, r_o.y + r_o.h}, r_t) ||
                        point_in_rect({r_o.x, r_o.y + r_o.h}, r_t))
                    {
                        colliding_entities.push_back(target_entity);
                        collision_verified = true;
                        break;
                    }

                    // Backward check for every vertice of target rect (r_t).
                    if (point_in_rect({r_t.x, r_t.y}, r_o) ||
                        point_in_rect({r_t.x + r_t.w, r_t.y}, r_o) ||
                        point_in_rect({r_t.x + r_t.w, r_t.y + r_t.h}, r_o) ||
                        point_in_rect({r_t.x, r_t.y + r_t.h}, r_o))
                    {
                        colliding_entities.push_back(target_entity);
                        collision_verified = true;
                        break;
                    }
                }

                if (collision_verified)
                {
                    break;
                }
            }
        }

        return colliding_entities;
    };

    bool place_entity_with_naive_spiral(
        Rect spawn_area,
        EntityPhysics& entity,
        std::vector<EntityPhysics*> collision_targets)
    {
        auto spiral_size{1};
        auto change_amount{10};
        auto counter_x{0};
        auto counter_y{0};

        // All inital position calculations will be integer based.
        entity.m_position.m_x = static_cast<int>(spawn_area.x + (spawn_area.w / 2));
        entity.m_position.m_y = static_cast<int>(spawn_area.y + (spawn_area.h / 2));

        while (spiral_size < std::min(spawn_area.w, spawn_area.h))
        {
            if (get_colliding_entities(entity, collision_targets).size() == 0)
            {
                return true;
            }

            if (counter_x == spiral_size)
            {
                entity.m_position.m_y += change_amount;
                counter_y++;

                if (counter_y == spiral_size)
                {
                    change_amount *= -1;
                    spiral_size++;
                    counter_x = 0;
                    counter_y = 0;
                }
            }
            else
            {
                entity.m_position.m_x += change_amount;
                counter_x++;
            }
        };

        return false;
    };
}

