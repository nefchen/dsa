/*
 * Created on 22.01.2021 by nefchen.
 */

#ifndef TYPES_BASIC_HPP
#define TYPES_BASIC_HPP

#include <SDL2/SDL.h>
#include <cstdint>


using u8 = std::uint8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;

using Id = u32;
using Rect = SDL_Rect;
using Color = SDL_Color;

struct Point
{
    i32 m_x;
    i32 m_y;
};

struct Range
{
    double m_min;
    double m_max;
};

struct Margin
{
    i32 m_top{0};
    i32 m_right{0};
    i32 m_bottom{0};
    i32 m_left{0};
};

enum struct Align
{
    center, center_top, center_bottom,
    left, left_top, left_bottom,
    right, right_top, right_bottom
};

#endif  // TYPES_BASIC_HPP

