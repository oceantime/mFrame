#include "WmCommandDecoder.hpp"
#include <codecvt>

namespace wmcanvas
{

std::string U16StrToU8Str( char16_t* u16char )
{
    std::u16string u16Str(u16char);
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    return converter.to_bytes(u16Str);
}

} //namespace wmcanvas
