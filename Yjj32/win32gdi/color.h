#pragma once
#include "core/core.h"
//#include <d2d1helper.h> color constants refernce.

namespace yjj {
namespace win32gdi {
	
	struct color {
        static constexpr byte get_r(const uint32 v) noexcept{ return ((v >> 16) & 0xFF); }
        static constexpr byte get_g(const uint32 v) noexcept{ return ((v >> 8 ) & 0xFF); }
        static constexpr byte get_b(const uint32 v) noexcept{ return ((v >> 0 ) & 0xFF); }

        static constexpr uint32 make_rgb(const byte r, const byte g, const byte b) noexcept {
            return (uint32(r) << 16) | (uint32(g) << 8) | uint32(b);
        }

        static constexpr color make_color(const byte r, const byte g, const byte b) noexcept {
            return color(r, g, b);
        }

		constexpr color(byte new_r, byte new_g, byte new_b) noexcept
			: r(new_r), g(new_g), b(new_b), unused_padd_(0){
		}

        constexpr color(uint32 new_rgb) noexcept
            : r(get_r(new_rgb)), g(get_g(new_rgb)), b(get_b(new_rgb)), unused_padd_(0) {
		}

        constexpr color() noexcept
			: color(0) {
		}

        constexpr operator uint32() const { return make_rgb(r, g, b); }

        byte r, g, b, unused_padd_;
	};

namespace colors {
    constexpr color
            alice_blue              = 0xFFF8F0,
            antique_white           = 0xD7EBFA,
            aqua                    = 0xFFFF00,
            aquamarine              = 0xD4FF7F,
            azure                   = 0xFFFFF0,
            beige                   = 0xDCF5F5,
            bisque                  = 0xC4E4FF,
            black                   = 0x000000,
            blanched_almond         = 0xCDEBFF,
            blue                    = 0xFF0000,
            blue_violet             = 0xE22B8A,
            brown                   = 0x2A2AA5,
            burly_wood              = 0x87B8DE,
            cadet_blue              = 0xA09E5F,
            chartreuse              = 0x00FF7F,
            chocolate               = 0x1E69D2,
            coral                   = 0x507FFF,
            cornflower_blue         = 0xED9564,
            cornsilk                = 0xDCF8FF,
            crimson                 = 0x3C14DC,
            cyan                    = 0xFFFF00,
            dark_blue               = 0x8B0000,
            dark_cyan               = 0x8B8B00,
            dark_goldenrod          = 0x0B86B8,
            dark_gray               = 0xA9A9A9,
            dark_green              = 0x006400,
            dark_khaki              = 0x6BB7BD,
            dark_magenta            = 0x8B008B,
            dark_olive_green        = 0x2F6B55,
            dark_orange             = 0x008CFF,
            dark_orchid             = 0xCC3299,
            dark_red                = 0x00008B,
            dark_salmon             = 0x7A96E9,
            dark_sea_green          = 0x8FBC8F,
            dark_slate_blue         = 0x8B3D48,
            dark_slate_gray         = 0x4F4F2F,
            dark_turquoise          = 0xD1CE00,
            dark_violet             = 0xD30094,
            deep_pink               = 0x9314FF,
            deep_sky_blue           = 0xFFBF00,
            dim_gray                = 0x696969,
            dodger_blue             = 0xFF901E,
            firebrick               = 0x2222B2,
            floral_white            = 0xF0FAFF,
            forest_green            = 0x228B22,
            fuchsia                 = 0xFF00FF,
            gainsboro               = 0xDCDCDC,
            ghost_white             = 0xFFF8F8,
            gold                    = 0x00D7FF,
            goldenrod               = 0x20A5DA,
            gray                    = 0x808080,
            green                   = 0x008000,
            green_yellow            = 0x2FFFAD,
            honeydew                = 0xF0FFF0,
            hot_pink                = 0xB469FF,
            indian_red              = 0x5C5CCD,
            indigo                  = 0x82004B,
            ivory                   = 0xF0FFFF,
            khaki                   = 0x8CE6F0,
            lavender                = 0xFAE6E6,
            lavender_blush          = 0xF5F0FF,
            lawn_green              = 0x00FC7C,
            lemon_chiffon           = 0xCDFAFF,
            light_blue              = 0xE6D8AD,
            light_coral             = 0x8080F0,
            light_cyan              = 0xFFFFE0,
            light_goldenrod_yellow  = 0xD2FAFA,
            light_green             = 0x90EE90,
            light_gray              = 0xD3D3D3,
            light_pink              = 0xC1B6FF,
            light_salmon            = 0x7AA0FF,
            light_sea_green         = 0xAAB220,
            light_sky_blue          = 0xFACE87,
            light_slate_gray        = 0x998877,
            light_steel_blue        = 0xDEC4B0,
            light_yellow            = 0xE0FFFF,
            lime                    = 0x00FF00,
            lime_green              = 0x32CD32,
            linen                   = 0xE6F0FA,
            magenta                 = 0xFF00FF,
            maroon                  = 0x000080,
            medium_aquamarine       = 0xAACD66,
            medium_blue             = 0xCD0000,
            medium_orchid           = 0xD355BA,
            medium_purple           = 0xDB7093,
            medium_sea_green        = 0x71B33C,
            medium_slate_blue       = 0xEE687B,
            medium_spring_green     = 0x9AFA00,
            medium_turquoise        = 0xCCD148,
            medium_violet_red       = 0x8515C7,
            midnight_blue           = 0x701919,
            mint_cream              = 0xFAFFF5,
            misty_rose              = 0xE1E4FF,
            moccasin                = 0xB5E4FF,
            navajo_white            = 0xADDEFF,
            navy                    = 0x800000,
            old_lace                = 0xE6F5FD,
            olive                   = 0x008080,
            olive_drab              = 0x238E6B,
            orange                  = 0x00A5FF,
            orange_red              = 0x0045FF,
            orchid                  = 0xD670DA,
            pale_goldenrod          = 0xAAE8EE,
            pale_green              = 0x98FB98,
            pale_turquoise          = 0xEEEEAF,
            pale_violet_red         = 0x9370DB,
            papaya_whip             = 0xD5EFFF,
            peach_puff              = 0xB9DAFF,
            peru                    = 0x3F85CD,
            pink                    = 0xCBC0FF,
            plum                    = 0xDDA0DD,
            powder_blue             = 0xE6E0B0,
            purple                  = 0x800080,
            red                     = 0x0000FF,
            rosy_brown              = 0x8F8FBC,
            royal_blue              = 0xE16941,
            saddle_brown            = 0x13458B,
            salmon                  = 0x7280FA,
            sandy_brown             = 0x60A4F4,
            sea_green               = 0x578B2E,
            sea_shell               = 0xEEF5FF,
            sienna                  = 0x2D52A0,
            silver                  = 0xC0C0C0,
            sky_blue                = 0xEBCE87,
            slate_blue              = 0xCD5A6A,
            slate_gray              = 0x908070,
            snow                    = 0xFAFAFF,
            spring_green            = 0x7FFF00,
            steel_blue              = 0xB48246,
            tan                     = 0x8CB4D2,
            teal                    = 0x808000,
            thistle                 = 0xD8BFD8,
            tomato                  = 0x4763FF,
            turquoise               = 0xD0E040,
            violet                  = 0xEE82EE,
            wheat                   = 0xB3DEF5,
            white                   = 0xFFFFFF,
            white_smoke             = 0xF5F5F5,
            yellow                  = 0x00FFFF,
            yellow_green            = 0x32CD9A;
}//namespace yjj::win32gdi::colors 
}//namespace yjj::win32gdi
}//namespace yjj 