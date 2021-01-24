/*
 * Created on 28.11.2020 by nefchen.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>
#include <variant>
#include <algorithm>

#include "comm.hpp"
#include "window.hpp"


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

void process_sdl_events(SDL_Event* event, comm::AppControlNode& app_node)
{
    while (SDL_PollEvent(event))
    {
        switch (event->type)
        {
            case SDL_QUIT:
                app_node.app_exit_request.emit();
                break;
            case SDL_WINDOWEVENT:
                if (event->window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    app_node.destroy_window_request.emit(
                        event->window.windowID
                    );
                }
            default:
                break;
        }
    }
}

void process_user_app_exit_request(bool& application_should_run)
{
    application_should_run = false;
}

std::vector<comm::Lifetime> connect_control_signals(
    std::shared_ptr<comm::AppControlNode> app_node,
    bool& application_should_run)
{
    std::vector<comm::Lifetime> lifetimes;

    // Exit signal.
    lifetimes.push_back(
        app_node->app_exit_request.connect(
            [&application_should_run]() {
                process_user_app_exit_request(application_should_run);
            }
        )
    );

    return lifetimes;
}

std::vector<comm::Lifetime> connect_window_management(
    std::shared_ptr<comm::AppControlNode> app_node)
{
    std::vector<comm::Lifetime> lifetimes;
    auto windows{std::make_shared<std::vector<win::Window>>()};

    lifetimes.push_back(
        app_node->create_window_request.connect(
            [windows](std::shared_ptr<comm::AppControlNode> app_node) {
                auto wind{win::Window{app_node}};
                windows->push_back(std::move(wind));
            }
        )
    );

    lifetimes.push_back(
        app_node->destroy_window_request.connect(
            [windows, app_node](Id wid) {
                windows->erase(
                    std::remove_if(
                        windows->begin(),
                        windows->end(),
                        [wid](auto& win) {
                            return wid == SDL_GetWindowID(win.m_sdl_window);
                        }
                    )
                );

                if (windows->empty())
                {
                    app_node->app_exit_request.emit();
                }
            }
        )
    );

    return lifetimes;
}

int main()
{
    initialize_sdl_environment();

    bool application_should_run{true};
    SDL_Event current_sdl_event;

    auto main_dispatcher{std::make_shared<comm::Dispatcher>()};
    auto app_node{std::make_shared<comm::AppControlNode>(main_dispatcher)};

    // lifetimes *must not* be discarded.
    auto control_lifetimes{
        connect_control_signals(app_node, application_should_run)
    };
    auto win_lifetimes{connect_window_management(app_node)};

    // Create main window.
    app_node->create_window_request.emit(app_node);

    while (application_should_run)
    {
        process_sdl_events(&current_sdl_event, *app_node);

        main_dispatcher->emit();
    }

    return 0;
}

