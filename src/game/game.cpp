/*
 * Created on 01.03.2021 by nefchen.
 */

#include "game/game.hpp"
#include "game/entities/creation.hpp"
#include "game/simulation/positioning.hpp"


namespace game
{
    void Game::add_render_output(RenderOutput output)
    {
        // Link handle with Viewport (or the respective source).
        output->m_render = [output, this](SDL_Renderer* renderer) mutable {
            this->render_handle(output, renderer);
        };

        m_render_outputs.push_back(std::move(output));
    };

    void Game::render_handle(RenderOutput& output, SDL_Renderer* renderer)
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
        Rect vp_rect{
            .x = 0,
            .y = 0,
            .w = output->m_rect.w,
            .h = output->m_rect.h
        };
        for (auto& entity: m_entities)
        {
            map_entity_into_viewport(entity.second, output);

            // Draw only if entity appears in viewport.
            // Assume entity is always smaller than the viewport.
            if (small_rect_crosses_large_rect(entity.second->m_rect, vp_rect))
            {
                entity.second->draw(renderer);
            }
        }
    };

    void Game::map_entity_into_viewport(
        std::unique_ptr<Entity>& entity,
        RenderOutput const& output_vp)
    {
        auto pos_relative_to_vp{entity->m_simul_position - output_vp->m_simul_position};
        entity->m_rect = {
            .x = static_cast<int>(pos_relative_to_vp.m_x * output_vp->m_scale),
            .y = static_cast<int>(pos_relative_to_vp.m_y * output_vp->m_scale),
            .w = static_cast<int>(entity->m_simul_size.m_x * output_vp->m_scale),
            .h = static_cast<int>(entity->m_simul_size.m_y * output_vp->m_scale)
        };
    };

    void Game::create_session(SessionProperties properties)
    {
        close_session();

        // Define map size and divide in regions to accomodate
        // initial distribution of player fleets.
        m_simulation.m_size = properties.m_map_size;
        Rect map_rect{
            .x = 0,
            .y = 0,
            .w = static_cast<int>(properties.m_map_size.m_x),
            .h = static_cast<int>(properties.m_map_size.m_y)
        };
        auto fleet_map_regions{
            create_sparse_regions_in_rect(map_rect, properties.m_players.size())
        };

        // Add each player to the game, create initial fleets and
        // place them at the centers of the precomputed regions.
        for (int i = 0; i < properties.m_players.size(); ++i)
        {
            auto& player_props{properties.m_players[i]};
            auto player_id{add_player_to_game(player_props)};

            if (player_id == std::nullopt)
            {
                continue;
            }

            for (auto& entity_t: player_props.m_starting_fleet)
            {
                auto entity{create_entity_from_type(entity_t)};
                add_entity_to_game(
                    player_id.value(), std::move(entity), fleet_map_regions.at(i)
                );
            }
        }
    };

    void Game::close_session()
    {
        m_simulation.reset_to_initial_state();
        m_entities.clear();
        m_render_outputs.clear();

        m_next_player_id = 0;
        m_next_entity_id = 0;
    };

    std::optional<Id> Game::add_player_to_game(PlayerProperties player_props)
    {
        // TODO: do we need to save more player information?
        return std::make_optional(m_next_player_id++);
    };

    std::optional<Id> Game::add_entity_to_game(
        Id player_id, std::unique_ptr<Entity>&& entity, Rect spawn_area)
    {
        auto entity_id{m_next_entity_id++};
        entity->m_id = entity_id;

        if (!m_simulation.add_entity_to_simulation(entity, spawn_area))
        {
            // Rollback id increment.
            m_next_player_id--;

            return std::nullopt;
        }

        m_entities.emplace_back(std::make_pair(entity_id, std::move(entity)));

        return std::make_optional(entity_id);
    };
}

