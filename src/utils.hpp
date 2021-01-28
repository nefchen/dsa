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

#endif  // UTILS_HPP

