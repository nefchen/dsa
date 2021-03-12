/*
 * Created on 10.03.2021 by nefchen.
 */

#ifndef GAME_ENTITIES_PROXIES_HPP
#define GAME_ENTITIES_PROXIES_HPP

#include "game/entities/entity.hpp"
#include "types/sdl.hpp"


namespace game
{
    namespace proxy
    {
        struct Renderable
        {
            Rect* m_rect;
            Point* m_pos;
            DrawFunc* m_draw;
        };
    };

    template <typename T>
    inline T take(Entity* entity)
    {
        return T{};
    };

    template <>
    inline proxy::Renderable take<proxy::Renderable>(Entity* entity)
    {
        return {
            .m_rect = &entity->m_rect,
            .m_pos = &entity->m_pos,
            .m_draw = &entity->m_draw
        };
    };
}

#endif  // GAME_ENTITIES_PROXIES_HPP

