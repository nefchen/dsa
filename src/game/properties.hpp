/*
 * Created on 20.03.2021 by nefchen.
 */

#ifndef GAME_PROPERTIES_HPP
#define GAME_PROPERTIES_HPP

#include <vector>

#include "types/physics.hpp"
#include "types/basic.hpp"
#include "game/entities/entity.hpp"


namespace game
{
    enum struct TPlayer
    {
        human,
        machine,
        remote,
        spectator
    };

    struct PlayerProperties
    {
        TPlayer m_type{TPlayer::human};
        u8 m_player_team{0};
        std::vector<TEntity> m_starting_fleet{
            TEntity::mothership,
            TEntity::explorer_ship,
            TEntity::explorer_ship,
            TEntity::explorer_ship
        };
    };

    struct SessionProperties
    {
        physics::Size m_map_size{1000, 1000};
        u32 m_population_limit{20};
        bool m_fog_of_war{true};
        std::vector<PlayerProperties> m_players;
    };
}

#endif  // GAME_PROPERTIES_HPP

