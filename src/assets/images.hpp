/*
 * Created on 13.03.2021 by nefchen.
 */

#ifndef ASSETS_IMAGES_HPP
#define ASSETS_IMAGES_HPP

#include <SDL2/SDL_image.h>
#include <filesystem>

#include "types/sdl.hpp"
#include "types/basic.hpp"


inline std::filesystem::path g_images_path {"../assets/images"};

inline sdl::Texture load_image(SDL_Renderer* renderer, std::string relative_img_path)
{
    auto img_path{g_images_path / relative_img_path};
    sdl::Texture img_texture{IMG_LoadTexture(renderer, img_path.c_str())};

    return img_texture;
};

struct Image
{
    sdl::Texture m_texture;
    Rect m_rect;
};

#endif  // ASSETS_IMAGES_HPP

