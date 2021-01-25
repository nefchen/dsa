/*
 * Created on 22.01.2021 by nefchen.
 */

#ifndef TYPES_HPP
#define TYPES_HPP

#include <SDL2/SDL.h>
#include <cstdint>


using i32 = std::int32_t;
using u32 = std::uint32_t;
using u8 = std::uint8_t;

using Id = u32;
using Rect = SDL_Rect;
using Color = SDL_Color;

struct Point
{
    i32 x;
    i32 y;
};

#endif  // TYPES_HPP

