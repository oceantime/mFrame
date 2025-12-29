/**
* Created by G-Canvas Open Source Team.
* Copyright (c) 2017, Alibaba, Inc. All rights reserved.
*
* This source code is licensed under the Apache Licence 2.0.
* For the full copyright and license information, please view
* the LICENSE file in the root directory of this source tree.
*/

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
