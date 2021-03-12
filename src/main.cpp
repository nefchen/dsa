/*
 * Created on 28.11.2020 by nefchen.
 */

#include <stdexcept>
#include <algorithm>
#include <vector>

#include "types/sdl.hpp"
#include "types/basic.hpp"
#include "types/input.hpp"
#include "comm/comm.hpp"
#include "views/game_screen/game_screen.hpp"
#include "views/loader.hpp"
#include "game/game.hpp"


using DispatcherPtr = std::shared_ptr<comm::Dispatcher>;
using Lifetimes = std::vector<comm::Lifetime>;

// Useful aggregate to encapsulate window dependencies.
struct Window
{
    sdl::Window m_sdl_window;
    sdl::Renderer m_sdl_renderer;
    std::unique_ptr<view::View> m_view;
    bool m_visible{true};
};

constexpr Rect g_initial_win_rect{0, 0, 1200, 800};

void initialize_sdl()
{
    if (auto res = SDL_Init(SDL_INIT_VIDEO); res != 0)
    {
        throw std::runtime_error("ERROR: in SDL initialization");
    }
    if (auto res = TTF_Init(); res != 0)
    {
        throw std::runtime_error("ERROR: in SDL_ttf initialization");
    }
}

Window create_window(comm::Node comm_node)
{
    sdl::Window raw_sdl_win{
        SDL_CreateWindow(
            "Deadly Space Adventures",
            g_initial_win_rect.x,
            g_initial_win_rect.y,
            g_initial_win_rect.w,
            g_initial_win_rect.h,
            SDL_WINDOW_SHOWN  // | SDL_WINDOW_RESIZABLE
        )
    };
    if (raw_sdl_win == nullptr)
    {
        throw std::runtime_error("ERROR: in SDL Window creation");
    }

    sdl::Renderer raw_sdl_renderer{
        SDL_CreateRenderer(
            raw_sdl_win.get(),
            -1,
            SDL_RENDERER_ACCELERATED
        )
    };

    Window win{std::move(raw_sdl_win), std::move(raw_sdl_renderer)};

    return win;
}

void render_window(Window& window)
{
    if (!window.m_visible)
    {
        return;
    }

    SDL_SetRenderDrawColor(window.m_sdl_renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(window.m_sdl_renderer.get());

    if (window.m_view != nullptr)
    {
        window.m_view->render(window.m_sdl_renderer.get());
    }

    SDL_RenderPresent(window.m_sdl_renderer.get());
}

void collect_sdl_window_events(SDL_WindowEvent win_event, comm::Node& comm_node)
{
    switch (win_event.event)
    {
        case SDL_WINDOWEVENT_RESIZED:
            comm_node->window_resized.emit({win_event.data1, win_event.data2});
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            comm_node->window_hidden.emit();
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            comm_node->window_exposed.emit();
            break;
    }
}

void collect_sdl_events(SDL_Event* event, comm::Node& comm_node)
{
    switch (event->type)
    {
        case SDL_QUIT:
            comm_node->app_exit_request.emit();
            break;
        case SDL_MOUSEMOTION:
            comm_node->mouse_moved.emit({event->motion.x, event->motion.y});
            break;
        case SDL_MOUSEBUTTONUP:
            comm_node->mouse_button_clicked.emit(
                {event->button.x, event->button.y},
                static_cast<input::MouseButton>(event->button.button),
                event->button.clicks
            );
            break;
        case SDL_WINDOWEVENT:
            collect_sdl_window_events(event->window, comm_node);
            break;
        default:
            break;
    }
}

void connect_control_signals(
    comm::Node& comm_node, bool& application_should_run, Lifetimes& lifetimes)
{
    // Exit signal.
    lifetimes.push_back(
        comm_node->app_exit_request.connect(
            [&application_should_run]() {
                // May turn more complicated later.
                application_should_run = false;
            }
        )
    );
}

void connect_window_signals(comm::Node& comm_node, Window& win, Lifetimes& lifetimes)
{
    // Resize.
    lifetimes.push_back(
        comm_node->window_resized.connect(
            [&win](Point new_size) {
                if (win.m_view != nullptr)
                {
                    win.m_view->propagate_resize(new_size);
                }
            }
        )
    );

    // Hidden.
    lifetimes.push_back(
        comm_node->window_hidden.connect(
            [&win]() { win.m_visible = false; }
        )
    );

    // Exposed.
    lifetimes.push_back(
        comm_node->window_exposed.connect(
            [&win]() { win.m_visible = true; }
        )
    );

    // Mouse moved.
    lifetimes.push_back(
        comm_node->mouse_moved.connect(
            [&win](Point pos) {
                if (win.m_view != nullptr)
                {
                    win.m_view->propagate_mouse_move(pos);
                }
            }
        )
    );

    // Mouse clicked.
    lifetimes.push_back(
        comm_node->mouse_button_clicked.connect(
            [&win](Point pos, input::MouseButton button, u8 clicks) {
                if (win.m_view != nullptr)
                {
                    win.m_view->propagate_mouse_click(pos, button, clicks);
                }
            }
        )
    );

    // Load view.
    lifetimes.push_back(
        comm_node->load_view.connect(
            [&win](view::Loader loader) {
                win.m_view = loader();

                // Guarantee that resize event will be propagated
                // at least once to this view.
                Point win_size;
                SDL_GetWindowSize(win.m_sdl_window.get(), &win_size.x, &win_size.y);
                win.m_view->propagate_resize(win_size);
            }
        )
    );
}

void connect_game_signals(
    comm::Node& comm_node, std::unique_ptr<game::Game> const& game, Lifetimes& lifetimes)
{
    // Game create.
    lifetimes.push_back(
        comm_node->create_game.connect(
            [&game] () { game->create_session(); }
        )
    );

    // Game add viewport handle.
    lifetimes.push_back(
        comm_node->add_viewport_handle_to_game.connect(
            [&game] (std::shared_ptr<view::ViewportHandle> main_render_handle) {
                game->add_render_output(std::move(main_render_handle));
            }
        )
    );

    // Game exit.
    lifetimes.push_back(
        comm_node->exit_game.connect(
            [&game] () { game->close_session(); }
        )
    );
}

int main()
{
    initialize_sdl();

    bool application_should_run{true};
    SDL_Event current_sdl_event;

    auto comm_dispatcher{std::make_shared<comm::Dispatcher>()};
    auto comm_node{std::make_shared<comm::_Node>(comm_dispatcher)};
    Lifetimes lifetimes;

    // Create unique game instance.
    auto game{std::make_unique<game::Game>()};

    // Create entry point window.
    auto window{create_window(comm_node)};

    // Connect signals, lifetimes *must not* be discarded.
    connect_control_signals(comm_node, application_should_run, lifetimes);
    connect_window_signals(comm_node, window, lifetimes);
    connect_game_signals(comm_node, game, lifetimes);

    // Load initial view.
    comm_node->load_view.emit({view::from_type<game_screen::View>{}, comm_node});

    while (application_should_run)
    {
        while (SDL_PollEvent(&current_sdl_event))
        {
            collect_sdl_events(&current_sdl_event, comm_node);
        }

        // Pump events to the many handlers.
        comm_dispatcher->emit();

        render_window(window);
    }

    return 0;
}

