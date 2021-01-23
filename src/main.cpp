/*
 * Created on 28.11.2020 by nefchen.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>
#include <variant>

#include "comm.hpp"

void initialize_sdl_environment();
void process_sdl_events(SDL_Event* event);
void process_user_app_exit_request(bool& application_should_run);


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

void process_sdl_events(SDL_Event* event, comm::AppControlNode& communicator)
{
    while (SDL_PollEvent(event))
    {
        switch (event->type)
        {
            case SDL_QUIT:
                communicator.app_exit_request.emit();
                break;
            default:
                break;
        }
    }
}

void process_user_app_exit_request(bool& application_should_run)
{
    application_should_run = false;
}

int main()
{
    initialize_sdl_environment();

    bool application_should_run{true};
    SDL_Event current_sdl_event;

    // Define communication primitives and application
    // control nodes. Connect app exit signals.
    auto main_event_dispatcher{std::make_shared<comm::Dispatcher>()};
    comm::AppControlNode main_comm_node{main_event_dispatcher};

    auto lifetime = main_comm_node.app_exit_request.connect(
        [&application_should_run]() {
            process_user_app_exit_request(application_should_run);
        }
    );

    while (application_should_run)
    {
        process_sdl_events(&current_sdl_event, main_comm_node);

        main_event_dispatcher->emit();
    }

    return 0;
}

