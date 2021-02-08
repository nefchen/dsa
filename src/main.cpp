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
#include "views/start_screen/start_screen.hpp"

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
using Windows = std::vector<Window>;

constexpr Rect g_initial_win_rect{0, 0, 800, 800};


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

Windows::iterator find_window(Windows& wins, Id wid)
{
    return std::find_if(
        wins.begin(), wins.end(),
        [&wid](auto& win) {
            return wid == SDL_GetWindowID(win.m_sdl_window.get());
        }
    );
}

void create_window(Windows& wins, comm::Node comm_node)
{
    sdl::Window raw_sdl_win{
        SDL_CreateWindow(
            "Deadly Space Adventures",
            g_initial_win_rect.x,
            g_initial_win_rect.y,
            g_initial_win_rect.w,
            g_initial_win_rect.h,
            SDL_WINDOW_SHOWN// | SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC
        )
    };
    if (raw_sdl_win == nullptr)
    {
        return;
    }

    sdl::Renderer raw_sdl_renderer{
        SDL_CreateRenderer(
            raw_sdl_win.get(),
            -1,
            SDL_RENDERER_ACCELERATED
        )
    };

    Window win{std::move(raw_sdl_win), std::move(raw_sdl_renderer)};

    // For now default to start screen.
    win.m_view = std::make_unique<start_screen::View>(comm_node);

    // Guarantee that resize event will be propagated
    // at least once to this window.
    win.m_view->propagate_resize({g_initial_win_rect.w, g_initial_win_rect.h});

    wins.push_back(std::move(win));
}

void destroy_window(Windows& wins, Id wid)
{
    wins.erase(
        std::remove_if(
            wins.begin(), wins.end(),
            [&wid](auto& win) {
                return wid == SDL_GetWindowID(win.m_sdl_window.get());
            }
        )
    );
}

void on_window_resize(Windows& wins, Id wid, Point new_size)
{
    auto win_it{find_window(wins, wid)};
    if (win_it == wins.end())
    {
        return;
    }

    (*win_it).m_view->propagate_resize(new_size);
}

void on_window_hidden(Windows& wins, Id wid)
{
    auto win_it{find_window(wins, wid)};
    if (win_it == wins.end())
    {
        return;
    }

    (*win_it).m_visible = false;
}

void on_window_exposed(Windows& wins, Id wid)
{
    auto win_it{find_window(wins, wid)};
    if (win_it == wins.end())
    {
        return;
    }

    (*win_it).m_visible = true;
}

void on_mouse_moved(Windows& wins, Id wid, Point pos)
{
    auto win_it{find_window(wins, wid)};
    if (win_it == wins.end())
    {
        return;
    }

    (*win_it).m_view->propagate_mouse_move(pos);
}

void on_mouse_clicked(
    Windows& wins, Id wid, Point pos, input::MouseButton button, u8 clicks)
{
    auto win_it{find_window(wins, wid)};
    if (win_it == wins.end())
    {
        return;
    }

    (*win_it).m_view->propagate_mouse_click(pos, button, clicks);
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
        case SDL_WINDOWEVENT_CLOSE:
            comm_node->destroy_window_request.emit(win_event.windowID);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            comm_node->window_resized.emit(
                win_event.windowID,
                {win_event.data1, win_event.data2}
            );
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            comm_node->window_hidden.emit(win_event.windowID);
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            comm_node->window_exposed.emit(win_event.windowID);
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
            comm_node->mouse_moved.emit(
                {event->motion.x, event->motion.y},
                event->motion.windowID
            );
            break;
        case SDL_MOUSEBUTTONUP:
            comm_node->mouse_button_clicked.emit(
                {event->button.x, event->button.y},
                static_cast<input::MouseButton>(event->button.button),
                event->button.clicks,
                event->button.windowID
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

void connect_window_signals(
    comm::Node& comm_node, Windows& wins, Lifetimes& lifetimes)
{
    // Create.
    lifetimes.push_back(
        comm_node->create_window_request.connect(
            [&wins, &comm_node]() {
                create_window(wins, comm_node);
            }
        )
    );

    // Destroy.
    lifetimes.push_back(
        comm_node->destroy_window_request.connect(
            [&wins](Id wid) {
                destroy_window(wins, wid);
            }
        )
    );

    // Resize.
    lifetimes.push_back(
        comm_node->window_resized.connect(
            [&wins](Id wid, Point new_size) {
                on_window_resize(wins, wid, new_size);
            }
        )
    );

    // Hidden.
    lifetimes.push_back(
        comm_node->window_hidden.connect(
            [&wins](Id wid) {
                on_window_hidden(wins, wid);
            }
        )
    );

    // Exposed.
    lifetimes.push_back(
        comm_node->window_exposed.connect(
            [&wins](Id wid) {
                on_window_exposed(wins, wid);
            }
        )
    );

    // Mouse moved.
    lifetimes.push_back(
        comm_node->mouse_moved.connect(
            [&wins](Point new_pos, Id wid) {
                on_mouse_moved(wins, wid, new_pos);
            }
        )
    );

    // Mouse clicked.
    lifetimes.push_back(
        comm_node->mouse_button_clicked.connect(
            [&wins](Point pos, input::MouseButton button, u8 clicks, Id wid) {
                on_mouse_clicked(wins, wid, pos, button, clicks);
            }
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

    Windows windows;

    // Connect signals, lifetimes *must not* be discarded.
    connect_control_signals(comm_node, application_should_run, lifetimes);
    connect_window_signals(comm_node, windows, lifetimes);

    // Create entry point window.
    create_window(windows, comm_node);

    while (application_should_run)
    {
        while (SDL_PollEvent(&current_sdl_event))
        {
            collect_sdl_events(&current_sdl_event, comm_node);
        }

        // Pump events to the many handlers.
        comm_dispatcher->emit();

        for (auto& win : windows)
        {
            render_window(win);
        }
    }

    return 0;
}

