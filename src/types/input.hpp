/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef TYPES_INPUT_HPP
#define TYPES_INPUT_HPP

#include <SDL2/SDL.h>

#include "types/basic.hpp"


namespace input
{
    enum struct MouseButton: u8
    {
        left = SDL_BUTTON_LEFT,
        right = SDL_BUTTON_RIGHT,
        middle = SDL_BUTTON_MIDDLE,
        x1 = SDL_BUTTON_X1,
        x2 = SDL_BUTTON_X2
    };

    enum struct MouseState: u8
    {
        pressed = SDL_PRESSED,
        released = SDL_RELEASED
    };

    enum struct MouseHover
    {
        enter, keep, leave
    };
}

#endif  // TYPES_INPUT_HPP

