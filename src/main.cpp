/*
 * Created on 28.11.2020 by nefchen.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>
#include <variant>
#include <algorithm>

#include "views/start_screen.hpp"
#include "comm.hpp"
#include "window.hpp"

using DispatcherPtr = std::shared_ptr<comm::Dispatcher>;


void initialize_sdl_environment()
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

void process_sdl_window_events(
    win::Window& window, SDL_WindowEvent win_event)
{
    switch (win_event.event)
    {
        case SDL_WINDOWEVENT_RESIZED:
            window.m_comm_node->window_resized.emit(
                {win_event.data1, win_event.data2}
            );
            break;
        case SDL_WINDOWEVENT_MOVED:
            window.m_comm_node->window_moved.emit(
                {win_event.data1, win_event.data2}
            );
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            window.m_comm_node->window_hidden.emit();
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            window.m_comm_node->window_exposed.emit();
            break;
    }
}

void process_sdl_events(
    SDL_Event* event,
    comm::Node& comm_node,
    std::shared_ptr<std::vector<win::Window>>& windows)
{
    while (SDL_PollEvent(event))
    {
        switch (event->type)
        {
            case SDL_QUIT:
                comm_node->app_exit_request.emit();
                break;
            case SDL_WINDOWEVENT:
            {
                if (event->window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    comm_node->destroy_window_request.emit(
                        event->window.windowID
                    );
                    break;
                }

                // In other cases.
                for (auto& window : *windows)
                {
                    auto id{SDL_GetWindowID(window.m_sdl_window)};
                    if (id == event->window.windowID)
                    {
                        process_sdl_window_events(window, event->window);
                    }
                }
            } break;
            default:
                break;
        }
    }
}

void process_user_app_exit_request(bool& application_should_run)
{
    application_should_run = false;
}

std::vector<comm::Disconnector> connect_control_signals(
    comm::Node& comm_node,
    bool& application_should_run)
{
    std::vector<comm::Disconnector> disconnectors;

    // Exit signal.
    disconnectors.push_back(
        comm_node->app_exit_request.connect(
            [&application_should_run]() {
                process_user_app_exit_request(application_should_run);
            }
        )
    );

    return disconnectors;
}

win::Window create_window(const comm::Node& comm_node)
{
    // Each window has its own window node.
    win::Window window{comm_node};

    // Select entry view. For now assign a default one.
    window.load_view<start_screen::View>();

    return window;
};

std::vector<comm::Disconnector> connect_window_management(
    comm::Node& comm_node, std::shared_ptr<std::vector<win::Window>>& windows)
{
    std::vector<comm::Disconnector> disconnectors;

    auto on_create_request{
        [windows, comm_node]() {
            // Window creation might turn lengthy,
            // keep it as separate function.
            windows->emplace_back(
                create_window(comm_node)
            );
        }
    };
    disconnectors.push_back(
        comm_node->create_window_request.connect(std::move(on_create_request))
    );

    auto on_destroy_request{
        [windows, comm_node](Id wid) {
            windows->erase(
                std::remove_if(
                    windows->begin(),
                    windows->end(),
                    [wid](auto& win) {
                        return wid == SDL_GetWindowID(win.m_sdl_window);
                    }
                )
            );
        }
    };
    disconnectors.push_back(
        comm_node->destroy_window_request.connect(
            std::move(on_destroy_request)
        )
    );

    return disconnectors;
}

int main()
{
    initialize_sdl_environment();

    bool application_should_run{true};
    SDL_Event current_sdl_event;

    auto main_dispatcher{std::make_shared<comm::Dispatcher>()};
    comm::Node comm_node{std::make_shared<comm::_Node>(main_dispatcher)};

    // disconnectors *must not* be discarded.
    auto control_disconnectors{
        connect_control_signals(comm_node, application_should_run)
    };

    auto windows{std::make_shared<std::vector<win::Window>>()};
    auto win_disconnectors{
        connect_window_management(comm_node, windows)
    };

    // Create main window.
    comm_node->create_window_request.emit();

    while (application_should_run)
    {
        process_sdl_events(&current_sdl_event, comm_node, windows);

        main_dispatcher->emit();

        for (auto& window: *windows)
        {
            window.render();
        }
    }

    comm::disconnect_signals(control_disconnectors);
    comm::disconnect_signals(win_disconnectors);

    return 0;
}

