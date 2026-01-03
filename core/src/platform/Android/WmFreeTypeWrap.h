/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef WMCANVAS_WMFREETYPEWRAP_H
#define WMCANVAS_WMFREETYPEWRAP_H


#define WMFONT_LOAD_BY_FREETYPE

#ifdef WMFONT_LOAD_BY_FREETYPE
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftstroke.h>
#include <freetype/ftbitmap.h>
#include <freetype/ftglyph.h>
#endif


namespace wmcanvas
{


    bool WmFT_InitLibrary(FT_Library *library);


    bool WmFT_LoadFace(FT_Library library, FT_Face *face,
                      const char *filename, float width = 0, float height = 0);


    bool WmFT_SetFaceCharSize(FT_Face face, float newWidth, float newHeight);


    bool WmFT_IsCharInFontFile(FT_Library library, const wchar_t charCode,
                              const char *filename, float width, float height);


    bool WmFT_IsCharInFace(FT_Face face, const wchar_t charCode);


    void WmFT_DisposeLibrarySafe(FT_Library face);


    void WmFT_DisposeFaceSafe(FT_Face face);


    void WmFT_DisposeGlyphSafe(FT_Glyph glyph);


    void WmFT_DisposeStrokeSafe(FT_Stroker stroker);

}



#endif //WMCANVAS_WMFREETYPEWRAP_H
