/*
 * Created on 01.03.2021 by nefchen.
 */

#include "game/game.hpp"


namespace game
{
    void Game::render_handle(
        std::shared_ptr<view::ViewportHandle> handle, SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &handle->m_rect);
    };
}

