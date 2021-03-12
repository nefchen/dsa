/*
 * Created on 01.03.2021 by nefchen.
 */

#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include <vector>
#include <map>
#include <memory>

#include "types/sdl.hpp"
#include "types/basic.hpp"
#include "widgets/viewport_handle.hpp"
#include "game/entities/entity.hpp"


namespace game
{
    using RenderOutput = std::shared_ptr<view::ViewportHandle>;

    struct Game
    {
        // This lambda will be called from the view context that
        // delivered the ViewportHandle. Game has no knowledge of
        // the overall rendering order and should use the render_handle
        // function to draw all game related content.
        void add_render_output(RenderOutput output);

        void create_session();
        void close_session();

        void render_handle(RenderOutput output, SDL_Renderer* renderer);
        Id add_entity_to_game(std::unique_ptr<game::Entity>&& entity);

        std::vector<std::unique_ptr<Entity>> m_entities;

        private:
            std::vector<RenderOutput> m_render_outputs;
    };
}

#endif  // GAME_GAME_HPP

