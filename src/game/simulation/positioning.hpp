/*
 * Created on 28.03.2021 by nefchen.
 */

#ifndef GAME_SIMULATION_POSITIONING_HPP
#define GAME_SIMULATION_POSITIONING_HPP

#include <vector>
#include <cmath>

#include "types/basic.hpp"


namespace game
{
    // Distribute num_centers inside rect.
    inline std::vector<Rect> create_sparse_regions_in_rect(Rect rect, u32 num_centers)
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
}

#endif  // GAME_SIMULATION_POSITIONING_HPP

