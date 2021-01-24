/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include <SDL2/SDL.h>

#include "types.hpp"


namespace mouse
{
    enum struct Button: u8
    {
        left = SDL_BUTTON_LEFT,
        right = SDL_BUTTON_RIGHT,
        middle = SDL_BUTTON_MIDDLE,
        x1 = SDL_BUTTON_X1,
        x2 = SDL_BUTTON_X2
    };

    enum struct ButtonState: u8
    {
        pressed = SDL_PRESSED,
        released = SDL_RELEASED
    };
}

#endif  // USER_INPUT_HPP

