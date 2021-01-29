/*
 * Created on 28.01.2021 by nefchen.
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "types.hpp"


inline Id _g_unique_id{0};
inline Id get_unique_id() noexcept
{
    return _g_unique_id++;
};

inline void reposition_rect(
    Rect& rect, Rect& prect,
    Align alignment = Align::left_top,
    Margin margin = {0, 0, 0, 0}) noexcept
{
    switch (alignment)
    {
        case Align::center:
            rect.x = prect.w / 2 - rect.w / 2;
            rect.y = prect.h / 2 - rect.h / 2;
            break;
        case Align::center_top:
            rect.x = prect.w / 2 - rect.w / 2;
            rect.y = margin.top;
            break;
        case Align::center_bottom:
            rect.x = prect.w / 2 - rect.w / 2;
            rect.y = prect.h - rect.h - margin.bottom;
            break;
        case Align::left:
            rect.x = margin.left;
            rect.y = prect.h / 2 - rect.h / 2;
            break;
        case Align::left_top:
            rect.x = margin.left;
            rect.y = margin.top;
            break;
        case Align::left_bottom:
            rect.x = margin.left;
            rect.y = prect.h - rect.h - margin.bottom;
            break;
        case Align::right:
            rect.x = prect.w - rect.w - margin.right;
            rect.y = prect.h / 2 - rect.h / 2;
            break;
        case Align::right_top:
            rect.x = prect.w - rect.w - margin.right;
            rect.y = margin.top;
            break;
        case Align::right_bottom:
            rect.x = prect.w - rect.w - margin.right;
            rect.y = prect.h - rect.h - margin.bottom;
            break;
        default:
            break;
    }
};

#endif  // UTILS_HPP

