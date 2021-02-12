/*
 * Created on 06.02.2021 by nefchen.
 */

#ifndef TYPES_SDL_HPP
#define TYPES_SDL_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>


namespace sdl
{
    inline auto _TTF_Font_deleter{
        [](TTF_Font* font) {
            if (font != nullptr)
            {
                TTF_CloseFont(font);
            }
        }
    };

    inline auto _Renderer_deleter{
        [](SDL_Renderer* renderer) {
            if (renderer != nullptr)
            {
                SDL_DestroyRenderer(renderer);
            }
        }
    };

    inline auto _Window_deleter{
        [](SDL_Window* window) {
            if (window != nullptr)
            {
                SDL_DestroyWindow(window);
            }
        }
    };

    inline auto _Surface_deleter{
        [](SDL_Surface* surface) {
            if (surface != nullptr)
            {
                SDL_FreeSurface(surface);
            }
        }
    };

    inline auto _Texture_deleter{
        [](SDL_Texture* texture) {
            if (texture != nullptr)
            {
                SDL_DestroyTexture(texture);
            }
        }
    };

    using Font = std::unique_ptr<TTF_Font, decltype(_TTF_Font_deleter)>;
    using Renderer = std::unique_ptr<SDL_Renderer, decltype(_Renderer_deleter)>;
    using Window = std::unique_ptr<SDL_Window, decltype(_Window_deleter)>;
    using Surface = std::unique_ptr<SDL_Surface, decltype(_Surface_deleter)>;
    using Texture = std::unique_ptr<SDL_Texture, decltype(_Texture_deleter)>;
}

#endif  // TYPES_SDL_HPP

