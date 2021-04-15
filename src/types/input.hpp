/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef TYPES_INPUT_HPP
#define TYPES_INPUT_HPP

#include <SDL2/SDL.h>
#include <unordered_map>

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

    enum struct KeyState
    {
        pressed = SDL_PRESSED,
        released = SDL_RELEASED
    };

    using KeyboardState = std::unordered_map<SDL_Keycode, KeyState>;

    inline bool is_shift_pressed(const KeyboardState* const ks)
    {
        return
            ks->contains(SDLK_LSHIFT) && (ks->at(SDLK_LSHIFT) == KeyState::pressed) ||
            ks->contains(SDLK_RSHIFT) && (ks->at(SDLK_RSHIFT) == KeyState::pressed);
    }
}

#endif  // TYPES_INPUT_HPP

