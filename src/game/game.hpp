/*
 * Created on 01.03.2021 by nefchen.
 */

#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include <vector>
#include <memory>

#include "types/sdl.hpp"
#include "widgets/viewport_handle.hpp"


namespace game
{
    struct Game
    {
        inline void add_render_output(std::shared_ptr<view::ViewportHandle> handle)
        {
            // This lambda will be called from the view context that
            // delivered the ViewportHandle. Game has no knowledge of
            // the overall rendering order and should use the render_handle
            // function to draw all game related content.
            handle->m_render = [handle, this](SDL_Renderer* renderer) {
                this->render_handle(handle, renderer);
            };

            m_render_outputs.push_back(std::move(handle));
        };

        void render_handle(
            std::shared_ptr<view::ViewportHandle> handle,
            SDL_Renderer* renderer);

        private:
            std::vector<std::shared_ptr<view::ViewportHandle>> m_render_outputs;
    };
}

#endif  // GAME_GAME_HPP

