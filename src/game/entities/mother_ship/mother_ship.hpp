/*
 * Created on 06.03.2021 by nefchen.
 */

#ifndef GAME_ENTITIES_MOTHER_SHIP_MOTHER_SHIP_HPP
#define GAME_ENTITIES_MOTHER_SHIP_MOTHER_SHIP_HPP

#include "game/entities/entity.hpp"


namespace game::mother_ship
{
    struct MotherShip: Entity
    {
        MotherShip();
        ~MotherShip();

        virtual void draw(SDL_Renderer* renderer) override;
    };
}

#endif  // GAME_ENTITIES_MOTHER_SHIP_MOTHER_SHIP_HPP
