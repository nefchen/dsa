/*
 * Created on 01.03.2021 by nefchen.
 */

#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include <vector>
#include <map>
#include <memory>
#include <optional>

#include "views/view.hpp"
#include "comm/comm.hpp"
#include "types/sdl.hpp"
#include "types/basic.hpp"
#include "types/physics.hpp"
#include "widgets/viewport_handle.hpp"
#include "game/entities/entity.hpp"
#include "game/simulation/simulation.hpp"
#include "game/properties.hpp"


namespace game
{
    using RenderOutput = std::shared_ptr<view::ViewportHandle>;

    struct Game
    {
        Game(comm::Node comm_node) : m_comm_node{std::move(comm_node)}
        {};

        // Add a RenderOutput to the game. It is a shared_ptr of a
        // ViewportHandle, ownership shared with Viewport.
        void add_render_output(RenderOutput output);

        // Render specific handle.
        void render_handle(RenderOutput& output, SDL_Renderer* renderer);

        // Starts a brand new game session.
        void create_session(SessionProperties properties);
        void close_session();

        std::optional<Id> add_player_to_game(PlayerProperties player_props);
        std::optional<Id> add_entity_to_game(
            Id player_id,
            std::unique_ptr<game::Entity>&& entity,
            Rect spawn_area = {0, 0, 500, 500}
        );

        // Get entity rect relative to a viewport from its position
        // in the simulation.
        void map_entity_into_viewport(
            std::unique_ptr<Entity>& entity,
            RenderOutput const& output_vp
        );

        // Hook user interactions (Signals) to game functions.
        void connect_user_interaction_signals();

        // Update keyboard state map and handle one time keyboard events.
        void resolve_user_keyboard_input(input::Key key, input::KeyState state);

        // Action depending on mouse button and number of clicks.
        void resolve_user_mouse_click(Point point, input::MouseButton, u8 clicks);

        // Handle dragging and non-dragging mouse-move events.
        void resolve_user_mouse_move(Point point);

        // Apply a force to the movable units currently selected
        // by the user.
        void apply_input_force(physics::Force force);

        // Selects anything *selectable*.
        void select_entity_at_point(Point point);

        // Selects anything *selectable* AND *movable* AND *from player*.
        void select_entities_in_rect(Rect rect);

        // Re-assign the action target for selected entities, target must
        // be *visible* to the user.
        // Visible = NOT behind fog-of-war AND NOT outside viewport.
        void assign_action_target_at_point(Point point);

        // Move projection of the viewport in the simulation.
        void move_viewport_by_delta(physics::Distance delta);

        // Increase/decrease the projection scope of the viewport
        // in the simulation.
        void scale_viewport_by_amount(double amount);

        comm::Node m_comm_node;
        std::vector<comm::unsafe::AutodeleteLifetime> m_lifetimes;

        private:
            std::vector<RenderOutput> m_render_outputs;
            std::vector<std::pair<Id, std::unique_ptr<Entity>>> m_entities;

            Simulation m_simulation;
            Id m_next_player_id{0};
            Id m_next_entity_id{0};
    };
}

#endif  // GAME_GAME_HPP

