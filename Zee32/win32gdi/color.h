#pragma once
#include "core/core.h"
//#include <d2d1helper.h> color constants refernce.

namespace zee {
namespace win32gdi {
	
	struct color {
		union {
			struct {
				byte r, g, b, a;
			};
			uint32 rgba;
		};

		color(byte new_r, byte new_g, byte new_b, byte new_a) noexcept
			: r(new_r), g(new_g), b(new_b), a(new_a) {
		}

		color(uint32 new_rgba) noexcept 
			: rgba(new_rgba) {
		}

		color() noexcept 
			: color(0) {
		}

        operator uint32() const { return rgba; }
	};

namespace colors {
    extern const color 
            AliceBlue               ,
            AntiqueWhite           ,
            Aqua                   ,
            Aquamarine             ,
            Azure                  ,
            Beige                  ,
            Bisque                 ,
            Black                  ,
            BlanchedAlmond         ,
            Blue                   ,
            BlueViolet             ,
            Brown                  ,
            BurlyWood              ,
            CadetBlue              ,
            Chartreuse             ,
            Chocolate              ,
            Coral                  ,
            CornflowerBlue         ,
            Cornsilk               ,
            Crimson                ,
            Cyan                   ,
            DarkBlue               ,
            DarkCyan               ,
            DarkGoldenrod          ,
            DarkGray               ,
            DarkGreen              ,
            DarkKhaki              ,
            DarkMagenta            ,
            DarkOliveGreen         ,
            DarkOrange             ,
            DarkOrchid             ,
            DarkRed                ,
            DarkSalmon             ,
            DarkSeaGreen           ,
            DarkSlateBlue          ,
            DarkSlateGray          ,
            DarkTurquoise          ,
            DarkViolet             ,
            DeepPink               ,
            DeepSkyBlue            ,
            DimGray                ,
            DodgerBlue             ,
            Firebrick              ,
            FloralWhite            ,
            ForestGreen            ,
            Fuchsia                ,
            Gainsboro              ,
            GhostWhite             ,
            Gold                   ,
            Goldenrod              ,
            Gray                   ,
            Green                  ,
            GreenYellow            ,
            Honeydew               ,
            HotPink                ,
            IndianRed              ,
            Indigo                 ,
            Ivory                  ,
            Khaki                  ,
            Lavender               ,
            LavenderBlush          ,
            LawnGreen              ,
            LemonChiffon           ,
            LightBlue              ,
            LightCoral             ,
            LightCyan              ,
            LightGoldenrodYellow   ,
            LightGreen             ,
            LightGray              ,
            LightPink              ,
            LightSalmon            ,
            LightSeaGreen          ,
            LightSkyBlue           ,
            LightSlateGray         ,
            LightSteelBlue         ,
            LightYellow            ,
            Lime                   ,
            LimeGreen              ,
            Linen                  ,
            Magenta                ,
            Maroon                 ,
            MediumAquamarine       ,
            MediumBlue             ,
            MediumOrchid           ,
            MediumPurple           ,
            MediumSeaGreen         ,
            MediumSlateBlue        ,
            MediumSpringGreen      ,
            MediumTurquoise        ,
            MediumVioletRed        ,
            MidnightBlue           ,
            MintCream              ,
            MistyRose              ,
            Moccasin               ,
            NavajoWhite            ,
            Navy                   ,
            OldLace                ,
            Olive                  ,
            OliveDrab              ,
            Orange                 ,
            OrangeRed              ,
            Orchid                 ,
            PaleGoldenrod          ,
            PaleGreen              ,
            PaleTurquoise          ,
            PaleVioletRed          ,
            PapayaWhip             ,
            PeachPuff              ,
            Peru                   ,
            Pink                   ,
            Plum                   ,
            PowderBlue             ,
            Purple                 ,
            Red                    ,
            RosyBrown              ,
            RoyalBlue              ,
            SaddleBrown            ,
            Salmon                 ,
            SandyBrown             ,
            SeaGreen               ,
            SeaShell               ,
            Sienna                 ,
            Silver                 ,
            SkyBlue                ,
            SlateBlue              ,
            SlateGray              ,
            Snow                   ,
            SpringGreen            ,
            SteelBlue              ,
            Tan                    ,
            Teal                   ,
            Thistle                ,
            Tomato                 ,
            Turquoise              ,
            Violet                 ,
            Wheat                  ,
            White                  ,
            WhiteSmoke             ,
            Yellow                 ,
            YellowGreen            ;
}//namespace zee::win32gdi::colors 
}//namespace zee::win32gdi
}//namespace zee 