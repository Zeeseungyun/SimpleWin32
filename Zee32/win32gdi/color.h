#pragma once
#include "core/core.h"
//#include <d2d1helper.h> color constants refernce.

namespace zee {
namespace win32gdi {
	
	struct color {
        static constexpr byte get_r(const uint32 v) noexcept{ return ((v >> 16) & 0xFF); }
        static constexpr byte get_g(const uint32 v) noexcept{ return ((v >> 8 ) & 0xFF); }
        static constexpr byte get_b(const uint32 v) noexcept{ return ((v >> 0 ) & 0xFF); }
        static constexpr uint32 make_rgb(const byte r, const byte g, const byte b) noexcept {
            return (uint32(r) << 16) | (uint32(g) << 8) | uint32(b);
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
            AliceBlue               =      0xFFF8F0,
            AntiqueWhite            =      0xD7EBFA,
            Aqua                    =      0xFFFF00,
            Aquamarine              =      0xD4FF7F,
            Azure                   =      0xFFFFF0,
            Beige                   =      0xDCF5F5,
            Bisque                  =      0xC4E4FF,
            Black                   =      0x000000,
            BlanchedAlmond          =      0xCDEBFF,
            Blue                    =      0xFF0000,
            BlueViolet              =      0xE22B8A,
            Brown                   =      0x2A2AA5,
            BurlyWood               =      0x87B8DE,
            CadetBlue               =      0xA09E5F,
            Chartreuse              =      0x00FF7F,
            Chocolate               =      0x1E69D2,
            Coral                   =      0x507FFF,
            CornflowerBlue          =      0xED9564,
            Cornsilk                =      0xDCF8FF,
            Crimson                 =      0x3C14DC,
            Cyan                    =      0xFFFF00,
            DarkBlue                =      0x8B0000,
            DarkCyan                =      0x8B8B00,
            DarkGoldenrod           =      0x0B86B8,
            DarkGray                =      0xA9A9A9,
            DarkGreen               =      0x006400,
            DarkKhaki               =      0x6BB7BD,
            DarkMagenta             =      0x8B008B,
            DarkOliveGreen          =      0x2F6B55,
            DarkOrange              =      0x008CFF,
            DarkOrchid              =      0xCC3299,
            DarkRed                 =      0x00008B,
            DarkSalmon              =      0x7A96E9,
            DarkSeaGreen            =      0x8FBC8F,
            DarkSlateBlue           =      0x8B3D48,
            DarkSlateGray           =      0x4F4F2F,
            DarkTurquoise           =      0xD1CE00,
            DarkViolet              =      0xD30094,
            DeepPink                =      0x9314FF,
            DeepSkyBlue             =      0xFFBF00,
            DimGray                 =      0x696969,
            DodgerBlue              =      0xFF901E,
            Firebrick               =      0x2222B2,
            FloralWhite             =      0xF0FAFF,
            ForestGreen             =      0x228B22,
            Fuchsia                 =      0xFF00FF,
            Gainsboro               =      0xDCDCDC,
            GhostWhite              =      0xFFF8F8,
            Gold                    =      0x00D7FF,
            Goldenrod               =      0x20A5DA,
            Gray                    =      0x808080,
            Green                   =      0x008000,
            GreenYellow             =      0x2FFFAD,
            Honeydew                =      0xF0FFF0,
            HotPink                 =      0xB469FF,
            IndianRed               =      0x5C5CCD,
            Indigo                  =      0x82004B,
            Ivory                   =      0xF0FFFF,
            Khaki                   =      0x8CE6F0,
            Lavender                =      0xFAE6E6,
            LavenderBlush           =      0xF5F0FF,
            LawnGreen               =      0x00FC7C,
            LemonChiffon            =      0xCDFAFF,
            LightBlue               =      0xE6D8AD,
            LightCoral              =      0x8080F0,
            LightCyan               =      0xFFFFE0,
            LightGoldenrodYellow    =      0xD2FAFA,
            LightGreen              =      0x90EE90,
            LightGray               =      0xD3D3D3,
            LightPink               =      0xC1B6FF,
            LightSalmon             =      0x7AA0FF,
            LightSeaGreen           =      0xAAB220,
            LightSkyBlue            =      0xFACE87,
            LightSlateGray          =      0x998877,
            LightSteelBlue          =      0xDEC4B0,
            LightYellow             =      0xE0FFFF,
            Lime                    =      0x00FF00,
            LimeGreen               =      0x32CD32,
            Linen                   =      0xE6F0FA,
            Magenta                 =      0xFF00FF,
            Maroon                  =      0x000080,
            MediumAquamarine        =      0xAACD66,
            MediumBlue              =      0xCD0000,
            MediumOrchid            =      0xD355BA,
            MediumPurple            =      0xDB7093,
            MediumSeaGreen          =      0x71B33C,
            MediumSlateBlue         =      0xEE687B,
            MediumSpringGreen       =      0x9AFA00,
            MediumTurquoise         =      0xCCD148,
            MediumVioletRed         =      0x8515C7,
            MidnightBlue            =      0x701919,
            MintCream               =      0xFAFFF5,
            MistyRose               =      0xE1E4FF,
            Moccasin                =      0xB5E4FF,
            NavajoWhite             =      0xADDEFF,
            Navy                    =      0x800000,
            OldLace                 =      0xE6F5FD,
            Olive                   =      0x008080,
            OliveDrab               =      0x238E6B,
            Orange                  =      0x00A5FF,
            OrangeRed               =      0x0045FF,
            Orchid                  =      0xD670DA,
            PaleGoldenrod           =      0xAAE8EE,
            PaleGreen               =      0x98FB98,
            PaleTurquoise           =      0xEEEEAF,
            PaleVioletRed           =      0x9370DB,
            PapayaWhip              =      0xD5EFFF,
            PeachPuff               =      0xB9DAFF,
            Peru                    =      0x3F85CD,
            Pink                    =      0xCBC0FF,
            Plum                    =      0xDDA0DD,
            PowderBlue              =      0xE6E0B0,
            Purple                  =      0x800080,
            Red                     =      0x0000FF,
            RosyBrown               =      0x8F8FBC,
            RoyalBlue               =      0xE16941,
            SaddleBrown             =      0x13458B,
            Salmon                  =      0x7280FA,
            SandyBrown              =      0x60A4F4,
            SeaGreen                =      0x578B2E,
            SeaShell                =      0xEEF5FF,
            Sienna                  =      0x2D52A0,
            Silver                  =      0xC0C0C0,
            SkyBlue                 =      0xEBCE87,
            SlateBlue               =      0xCD5A6A,
            SlateGray               =      0x908070,
            Snow                    =      0xFAFAFF,
            SpringGreen             =      0x7FFF00,
            SteelBlue               =      0xB48246,
            Tan                     =      0x8CB4D2,
            Teal                    =      0x808000,
            Thistle                 =      0xD8BFD8,
            Tomato                  =      0x4763FF,
            Turquoise               =      0xD0E040,
            Violet                  =      0xEE82EE,
            Wheat                   =      0xB3DEF5,
            White                   =      0xFFFFFF,
            WhiteSmoke              =      0xF5F5F5,
            Yellow                  =      0x00FFFF,
            YellowGreen             =      0x32CD9A;
}//namespace zee::win32gdi::colors 
}//namespace zee::win32gdi
}//namespace zee 