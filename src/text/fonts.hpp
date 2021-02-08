/*
 * Created on 24.01.2021 by nefchen.
 */

#ifndef TEXT_FONTS_HPP
#define TEXT_FONTS_HPP

#include <filesystem>
#include <stdexcept>
#include <memory>
#include <string>
#include <map>

#include "types/sdl.hpp"


enum struct Fonts
{
    megrim, baloo, ibm
};

inline std::filesystem::path g_font_path {"../assets/fonts"};
inline std::map<Fonts, std::string> g_font_resource{
    {Fonts::megrim, "Megrim-Regular.ttf"},
    {Fonts::baloo, "BalooTammudu2-Regular.ttf"},
    {Fonts::ibm, "IBMPlexMono-Regular.ttf"}
};


inline sdl::Font load_font(Fonts font_type, u8 font_size)
{
    auto font_path = g_font_path / g_font_resource.at(font_type);
    sdl::Font font{TTF_OpenFont(font_path.c_str(), font_size)};

    if (font == nullptr)
    {
        throw std::runtime_error(
            "Err: no font found at " + font_path.string()
        );
    }

    return font;
};


#endif  // TEXT_FONTS_HPP

