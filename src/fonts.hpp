/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef FONTS_HPP
#define FONTS_HPP

#include <SDL2/SDL_ttf.h>
#include <filesystem>
#include <stdexcept>
#include <memory>
#include <string>
#include <map>


enum struct Fonts
{
    megrim, baloo, ibm
};

inline std::filesystem::path g_font_path {"assets/fonts"};
inline std::map<Fonts, std::string> g_font_resource{
    {Fonts::megrim, "Megrim-Regular.ttf"},
    {Fonts::baloo, "BalooTammudu2-Regular.ttf"},
    {Fonts::ibm, "IBMPlexMono-Regular.ttf"}
};


inline auto font_deleter{
    [](TTF_Font* font) {
        if (font != nullptr)
        {
            TTF_CloseFont(font);
        }
    }
};

inline std::unique_ptr<TTF_Font, decltype(font_deleter)>
load_font(Fonts font_type, u8 font_size)
{
    auto font_path = g_font_path / g_font_resource.at(font_type);
    auto font = TTF_OpenFont(font_path.c_str(), font_size);

    if (font == nullptr)
    {
        throw std::runtime_error(
            "Err: no font found at " + font_path.string()
        );
    }

    return std::unique_ptr<TTF_Font, decltype(font_deleter)>(
        font, font_deleter);
};


#endif  // FONTS_HPP

