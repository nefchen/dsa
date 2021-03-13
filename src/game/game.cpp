/*
 * Created on 01.03.2021 by nefchen.
 */

#include "game/game.hpp"
#include "game/entities/mother_ship/mother_ship.hpp"


namespace game
{
    void Game::add_render_output(RenderOutput output)
    {
        // Link handle with Viewport (or the respective source).
        output->m_render = [output, this](SDL_Renderer* renderer) {
            this->render_handle(output, renderer);
        };

        m_render_outputs.push_back(std::move(output));
    };

    void Game::render_handle(RenderOutput output, SDL_Renderer* renderer)
    {
        // All entities will be drawn in this texture.
        sdl::Texture texture_for_drawing{
            SDL_CreateTexture(
                renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                output->m_rect.w,
                output->m_rect.h
            )
        };

        // Circles with zero alpha will be drawn here to later be punched
        // into the fog of war. This way only the area around the entity
        // is visible.
        sdl::Texture texture_for_punch_circles{
            SDL_CreateTexture(
                renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                output->m_rect.w,
                output->m_rect.h
            )
        };

        SDL_SetRenderDrawColor(renderer, 4, 24, 56, 255);
        SDL_RenderFillRect(renderer, &output->m_rect);

        // Draw renderable entities onto the viewport.
        for (auto& entity: m_entities)
        {
            entity->draw(renderer);
        }
    };

    void Game::create_session()
    {
        for (int row = 0; row < 10; ++row)
        {
            for (int col = 0; col < 10; ++col)
            {
                auto entity{std::make_unique<mother_ship::MotherShip>()};
                entity->m_rect.x += 120 * col;
                entity->m_rect.y += 70 * row;
                m_entities.push_back(std::move(entity));
            }
        }
    };

    void Game::close_session()
    {};
}

