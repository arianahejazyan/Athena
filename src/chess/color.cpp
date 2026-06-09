#include "color.h"

namespace athena
{

Color::Color(std::string_view color) noexcept
{
    switch (std::tolower(color[0]))
    {   
        case 'r': value_ = Color::Red   ; break;
        case 'b': value_ = Color::Blue  ; break;
        case 'y': value_ = Color::Yellow; break;    
        case 'g': value_ = Color::Green ; break;
        default : value_ = Color::None  ; break;
    }
}

std::string Color::uci() const noexcept
{
    switch (value_)
    {   
        case Color::Red   : return "r";
        case Color::Blue  : return "b";
        case Color::Yellow: return "y";
        case Color::Green : return "g";
        default           : return "" ;
    }
}
    
} // namespace athena