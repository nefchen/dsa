/*
 * Created on 28.01.2021 by nefchen.
 */

#ifndef VIEWS_POSITIONING_HPP
#define VIEWS_POSITIONING_HPP

#include "types/basic.hpp"


inline Rect rect_in_absolute_origin(const Rect& rect, const Rect& origin) noexcept
{
    return {rect.x + origin.x, rect.y + origin.y, rect.w, rect.h};
};

inline bool point_in_rect(const Point& p, const Rect& r) noexcept
{
    return p.m_x > r.x && p.m_x < r.x + r.w && p.m_y > r.y && p.m_y < r.y + r.h;
};

inline Point relative_point_to_rect(const Point& p, const Rect& r) noexcept
{
    return {p.m_x - r.x, p.m_y - r.y};
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
            rect.y = margin.m_top;
            break;
        case Align::center_bottom:
            rect.x = prect.w / 2 - rect.w / 2;
            rect.y = prect.h - rect.h - margin.m_bottom;
            break;
        case Align::left:
            rect.x = margin.m_left;
            rect.y = prect.h / 2 - rect.h / 2;
            break;
        case Align::left_top:
            rect.x = margin.m_left;
            rect.y = margin.m_top;
            break;
        case Align::left_bottom:
            rect.x = margin.m_left;
            rect.y = prect.h - rect.h - margin.m_bottom;
            break;
        case Align::right:
            rect.x = prect.w - rect.w - margin.m_right;
            rect.y = prect.h / 2 - rect.h / 2;
            break;
        case Align::right_top:
            rect.x = prect.w - rect.w - margin.m_right;
            rect.y = margin.m_top;
            break;
        case Align::right_bottom:
            rect.x = prect.w - rect.w - margin.m_right;
            rect.y = prect.h - rect.h - margin.m_bottom;
            break;
        default:
            break;
    }
};

#endif  // VIEWS_POSITIONING_HPP

