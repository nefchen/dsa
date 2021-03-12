/*
 * Created on 06.03.2021 by nefchen.
 */

#include <iostream>

#include "game/entities/mother_ship/mother_ship.hpp"


namespace game::mother_ship
{
    MotherShip::MotherShip()
    {
        std::cout << "Mothership created\n";
    };

    MotherShip::~MotherShip()
    {
        std::cout << "Mothership destroyed\n";
    };

    void MotherShip::draw(SDL_Renderer* renderer)
    {
        //std::cout << "Mothership drawn\n";
    };
}

