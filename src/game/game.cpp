/*
 * Created on 01.03.2021 by nefchen.
 */

#include "game/game.hpp"
#include "game/entities/mother_ship/mother_ship.hpp"


namespace game
{
    void Game::render_handle(
        std::shared_ptr<view::ViewportHandle> handle, SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, 4, 24, 56, 255);
        SDL_RenderFillRect(renderer, &handle->m_rect);

        // Draw renderable entities onto the viewport.
        for (auto& renderable: m_renderables)
        {
            (*renderable.second.m_draw)(renderer);
        }
    };

    void Game::create_session()
    {
        auto entity{std::make_unique<mother_ship::MotherShip>()};
        entity->m_id = 1;
        m_renderables.emplace(std::make_pair(entity->m_id, take<proxy::Renderable>(entity.get())));
        m_entities.emplace(std::make_pair(entity->m_id, std::move(entity)));
    };

    void Game::close_session()
    {};
}

