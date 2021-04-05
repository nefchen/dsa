/*
 * Created on 29.03.2021 by nefchen.
 */

#ifndef GAME_ENTITIES_CREATION_HPP
#define GAME_ENTITIES_CREATION_HPP

#include <memory>

#include "game/entities/entity.hpp"
#include "game/entities/mothership/mothership.hpp"


namespace game
{
    inline std::unique_ptr<Entity> create_entity_from_type(TEntity entity_t)
    {
        switch (entity_t)
        {
            case TEntity::mothership:
                return mothership::create_entity();
            case TEntity::explorer_ship:
                return mothership::create_entity();
            default:
                return nullptr;
        }
    };
}

#endif  // GAME_ENTITIES_CREATION_HPP

