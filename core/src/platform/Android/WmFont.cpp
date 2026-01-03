#include <iostream>
#include <sstream>
#include <cassert>

#include "WmFont.h"
#include "WmCanvas2dContext.h"
#include "WmCanvas.hpp"
#include "WmSystemFontInformation.h"
#include <assert.h>


void * (*WmFont::getFontCallback)(const char *fontDefinition) = nullptr;


bool (*WmFont::getFontImageCallback)(void *font, wchar_t charcode,
                                    int &ftBitmapWidth, int &ftBitmapHeight,
                                    unsigned char *&bitmapBuffer, int &left,
                                    int &top, float &advanceX,
                                    float &advanceY) = nullptr;

WmFontMetrics::WmFontMetrics() : unitsPerEM(0), ascender(0.0f), descender(0.0f) {
}

#ifdef WMFONT_LOAD_BY_FREETYPE

WmFont::WmFont(WmFontManager& fontManager, const char *fontFileName)
        : mFontManager(fontManager),
          mFontFileName(fontFileName),
          mHinting(1),
          mOutlineType(1),
          mOutlineThickness(1) {
    mPointSize = 12;
}

#else
WmFont::WmFont(const char *fontDefinition)
    : m_fontDefinition(fontDefinition), m_font(0)
{
}
#endif


std::string GetGlyphKey(std::string& fontFileName, wmcanvas::WmFontStyle* fontStyle,
        float scaleFontX, float scaleFontY) {
    std::string result = "";
    float fontSize = fontStyle->GetSize();
    // use int size
    int w = (int)(fontSize * scaleFontX);
    int h = (int)(fontSize * scaleFontY);
    result.append(fontFileName);
    result.append("_");
    // style: normal / italic
    std::string style = "normal";
    if (fontStyle->GetStyle() == wmcanvas::WmFontStyle::Style::ITALIC ||
        fontStyle->GetStyle() == wmcanvas::WmFontStyle::Style::OBLIQUE) {
        style = "italic";
    }
    result.append(style);
    // if smallcap, add to key
    if (fontStyle->GetVariant() == wmcanvas::WmFontStyle::Variant::SMALL_CAPS) {
        result.append("_smallcap");
    }
    // weight
    result.append("_");
    result.append(std::to_string((int)fontStyle->GetWeight()));

    result.append("_");
    result.append(std::to_string(w));
    result.append("_");
    result.append(std::to_string(h));
    return result;
}



WmFont::~WmFont() {
    if (mFace != nullptr) {
        wmcanvas::WmFT_DisposeFaceSafe(mFace);
        mFace = nullptr;
    }
    mLibrary = nullptr;
}


void WmFont::SetFtLibrary(FT_Library library) {
    this->mLibrary = library;
}


void WmFont::DrawText(WmCanvasContext *context, wchar_t text, float &x, float y,
                     WmColorRGBA color, float sx, float sy, bool isStroke) {
    bool needDrawShadow = context->NeedDrawShadow();
    const WmGlyphs *glyph = GetOrLoadGlyph(context->mCurrentState->mFont, text, isStroke, sx, sy,
    context->mCurrentState->mLineWidth,context->GetDevicePixelRatio());
    if (glyph != nullptr) {
        DrawGlyph(context, glyph, x, y, sx, sy, color,needDrawShadow);
        x += glyph->advanceX / sx;
    }
}

/**
 * deprecated
 */
void WmFont::DrawText(WmCanvasContext *context, const wchar_t *text, float &x,
                     float y, WmColorRGBA color, float sx, float sy, bool isStroke) {
    if (text == nullptr || wcslen(text) == 0) {
        return;
    }
    bool needDrawShadow = context->NeedDrawShadow();
    for (size_t i = 0; i < wcslen(text); ++i) {
        const WmGlyphs *glyph = GetOrLoadGlyph(context->mCurrentState->mFont, text[i], isStroke, sx, sy,
        context->mCurrentState->mLineWidth,context->GetDevicePixelRatio());
        if (glyph != nullptr) {
            DrawGlyph(context, glyph, x, y, sx, sy, color,needDrawShadow);
            x += glyph->advanceX / sx;
        }
    }
}


void WmFont::DrawGlyph(WmCanvasContext *context, const WmGlyphs *glyph, float x,
                      float y, float sx, float sy, WmColorRGBA color,bool needDrawShadow) {
    context->SetTexture(glyph->texture->GetTextureID());
    float x0 = x + (glyph->offsetX / sx);
    float y0 = y - (glyph->offsetY / sy);
    float w = glyph->width / sx;
    float h = glyph->height / sy;
    float s0 = glyph->s0;
    float t0 = glyph->t0;
    float s1 = glyph->s1;
    float t1 = glyph->t1;

    // context->PushRectangleFormat(x0,
    //         y0, w, h, s0, t0, s1 - s0, t1 - t0, color,
    //         context->mCurrentState->mTransform, false, nullptr, true);
    if (needDrawShadow)
    {
        std::vector<WmVertex> vec;
        context->PushRectangleFormat(x0, y0, w, h, s0, t0, s1 - s0, t1 - t0,
                                     color, context->mCurrentState->mTransform,
                                     false, &vec, true);

        WmRectf rect;
        WmPath::GetRectCoverVertex(rect, vec);
        context->DrawShadow(rect, [&] { context->PushVertexs(vec); });
        context->PushVertexs(vec);
    }
    else
    {
        context->PushRectangleFormat(x0, y0, w, h, s0, t0, s1 - s0, t1 - t0,
                                     color, context->mCurrentState->mTransform,
                                     false, nullptr, true);
    }
}


void WmFont::SetFontCallback(
        void *(*getFontCB)(const char *fontDefinition),
        bool (*getFontImageCB)(void *font, wchar_t charcode, int &ftBitmapWidth,
                               int &ftBitmapHeight, unsigned char *&bitmapBuffer,
                               int &left, int &top, float &advanceX,
                               float &advanceY)) {
    WmFont::getFontCallback = getFontCB;
    WmFont::getFontImageCallback = getFontImageCB;
}


const WmGlyphs *WmFont::GetOrLoadGlyph(wmcanvas::WmFontStyle* fontStyle, const wchar_t charCode, bool isStroke,
                              float sx, float sy,float lineWidth,float deviceRatio) {
    std::string fontKey = GetGlyphKey(mFontFileName, fontStyle, sx, sy);
    const WmGlyphs *glyph = mFontManager.mGlyphCache.GetGlyph(mFontFileName, charCode, fontKey, isStroke, false);
    if (glyph != nullptr) {
        return glyph;
    }

#ifdef WMFONT_LOAD_BY_FREETYPE

    wchar_t buffer[2] = {0, 0};
    buffer[0] = charCode;

    LoadGlyphs(fontStyle, buffer, isStroke, sx, sy,lineWidth,deviceRatio);
    UpdateCurrentTextMetrics();

    glyph = mFontManager.mGlyphCache.GetGlyph(mFontFileName, charCode, fontKey, isStroke, false);
    assert(glyph);
    return glyph;

#else
    bool found = false;
    if (nullptr == m_font && nullptr != getFontCallback) {
        m_font = getFontCallback(m_fontDefinition.c_str());
    }

    if (nullptr != m_font && getFontImageCallback != nullptr)
    {
        int ftBitmapWidth, ftBitmapHeight, left, top;
        float advanceX, advanceY;
        unsigned char *bitmapBuffer;
        found = getFontImageCallback(m_font, charcode, ftBitmapWidth,
                                     ftBitmapHeight, bitmapBuffer, left, top,
                                     advanceX, advanceY);
        if (found)
        {
            SaveGlyphToCache(charcode, ftBitmapWidth, ftBitmapHeight, bitmapBuffer,
                      left, top, advanceX, advanceY);
            iter = m_glyphs.find(charcode);
            assert(iter != m_glyphs.end());
            return &(iter->second);
        }
    }
    return nullptr;
#endif
}



#ifdef WMFONT_LOAD_BY_FREETYPE


void WmFont::LoadGlyphs(wmcanvas::WmFontStyle* fontStyle, const wchar_t *charcodes, bool isStroke,
        float sx, float sy,float lineWidth,float deviceRatio) {
    FT_Glyph ft_glyph = nullptr;
    FT_GlyphSlot slot;
    FT_Bitmap ft_bitmap;
    float fontSize = fontStyle->GetSize();

    if (!LoadFaceIfNot()) {
        LOG_E("LoadGlyphs:LoadFace fail");
        return;
    }

    PrepareLoadGlyph(fontSize, sx, sy);

    FT_Face face = mFace;
    FT_Int32 flags = 0;
    flags |= FT_LOAD_NO_BITMAP;

    if (mHinting) {
        flags |= FT_LOAD_FORCE_AUTOHINT;
    } else {
        flags |= FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
    }

    FT_Stroker ftStroker = nullptr;

    std::string glyphKey = GetGlyphKey(mFontFileName, fontStyle, sx, sy);
    for (size_t i = 0; i < wcslen(charcodes); ++i) {
        int ft_bitmap_width = 0;
        int ft_bitmap_rows = 0;
        int ft_bitmap_pitch = 0;
        int ft_glyph_top = 0;
        int ft_glyph_left = 0;
        FT_UInt glyph_index = FT_Get_Char_Index(face, charcodes[i]);
        // TODO glyph_index²»´æÔÚ£¿
        FT_Error error = FT_Load_Glyph(face, glyph_index, flags);
        if (error) {
            return;
        }

        // handle italic
        int mstyle = (int)fontStyle->GetStyle();
        if ((mstyle & (int)wmcanvas::WmFontStyle::Style::ITALIC) || (mstyle & (int)wmcanvas::WmFontStyle::Style::OBLIQUE)) {
            FT_Matrix m_ItalicMatrix;
            m_ItalicMatrix.xx = 1 << 16;
            m_ItalicMatrix.xy = 0x5800;
            m_ItalicMatrix.yx = 0;
            m_ItalicMatrix.yy = 1 << 16;
            FT_Outline_Transform(&face->glyph->outline, &m_ItalicMatrix);
        }

        // handle large weight, eg. bold
        int weight = (int)fontStyle->GetWeight();
        if (weight > static_cast<int>(wmcanvas::WmFontStyle::Weight::MEDIUM)) {
            if (mFace->glyph->format == FT_GLYPH_FORMAT_OUTLINE){
                int boldPixel = 0;
                if(mPointSize <= 35) {
                    boldPixel = 1;
                } else if(mPointSize <= 60) {
                    boldPixel = 2;
                } else if(mPointSize <= 80){
                    boldPixel = 3;
                } else if(mPointSize <= 100) {
                    boldPixel = 4;
                } else if(mPointSize <= 150){
                    boldPixel = 6;
                } else {
                    boldPixel = 9;
                }
                if (isStroke) {
                    boldPixel = boldPixel + 1;
                }
                FT_Outline_Embolden(&face->glyph->outline, 64*boldPixel);
            }
        }

        if (isStroke) {
            if (ftStroker == nullptr) {
                if (!LoadStroke(mFontFileName.data(), &ftStroker, sx, sy,lineWidth,deviceRatio)) {
                    return;
                }
            }

            FT_BitmapGlyph ft_bitmap_glyph;
            error = FT_Get_Glyph(face->glyph, &ft_glyph);
            if (error) {
                wmcanvas::WmFT_DisposeStrokeSafe(ftStroker);
                return;
            }

            if (mOutlineType == 1) {
                error = FT_Glyph_Stroke(&ft_glyph, ftStroker, 1);
            } else if (mOutlineType == 2) {
                error = FT_Glyph_StrokeBorder(&ft_glyph, ftStroker, 0, 1);
            }  else if (mOutlineType == 3) {
                error = FT_Glyph_StrokeBorder(&ft_glyph, ftStroker, 1, 1);
            }
            if (error) {
                wmcanvas::WmFT_DisposeStrokeSafe(ftStroker);
                return;
            }

            error = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_NORMAL, 0, 1);
            if (error) {
                wmcanvas::WmFT_DisposeStrokeSafe(ftStroker);
                return;
            }

            ft_bitmap_glyph = reinterpret_cast<FT_BitmapGlyph>(ft_glyph);
            ft_bitmap = ft_bitmap_glyph->bitmap;
            ft_bitmap_width = ft_bitmap.width;
            ft_bitmap_rows = ft_bitmap.rows;
            ft_bitmap_pitch = ft_bitmap.pitch;
            ft_glyph_top = ft_bitmap_glyph->top;
            ft_glyph_left = ft_bitmap_glyph->left;
        } else {
            error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
            if (error) {
                return;
            }
            slot = face->glyph;
            ft_bitmap = slot->bitmap;
            ft_bitmap_width = slot->bitmap.width;
            ft_bitmap_rows = slot->bitmap.rows;
            ft_bitmap_pitch = slot->bitmap.pitch;
            ft_glyph_top = slot->bitmap_top;
            ft_glyph_left = slot->bitmap_left;
        }

        slot = face->glyph;
        float advanceX = slot->advance.x / 64.0f;
        float advanceY = slot->advance.y / 64.0f;

        // save glyph to cache, and load it to texture
        WmGlyphs glyph;
        glyph.texture = nullptr;
        glyph.charcode = charcodes[i];

        // must copy pixels buffer from FreeType, we will delete the temp buffer after load to texture
        // cannot reuse the pointer because FreeType use it internal
        glyph.bitmapBuffer = new unsigned char[ft_bitmap_width * ft_bitmap_rows];
        memcpy(glyph.bitmapBuffer, ft_bitmap.buffer, ft_bitmap_width * ft_bitmap_rows);

        // glyph.bitmapBuffer = ft_bitmap.buffer;
        glyph.width = ft_bitmap_width;
        glyph.height = ft_bitmap_rows;
        glyph.outlineType = 0;
        glyph.outlineThickness = 0;
        glyph.offsetX = ft_glyph_left;
        glyph.offsetY = ft_glyph_top;
        glyph.advanceX = advanceX;
        glyph.advanceY = advanceY;

        mFontManager.AddGlyph(mFontFileName, glyphKey, glyph, isStroke);

//              glyphKey.data(), charcodes[i],
//              this, mFace, ft_bitmap_width, ft_bitmap_rows);

        // free stroke bitmap glyph (no need free face glyph slot, it will be reused)
        if (isStroke) {
            wmcanvas::WmFT_DisposeGlyphSafe(ft_glyph);
        }
    }

    wmcanvas::WmFT_DisposeStrokeSafe(ftStroker);
}


void WmFont::UpdateCurrentTextMetrics()  {
    this->mFontMetrics.unitsPerEM = mFace->units_per_EM;
    // 26.6 pixel format: convert it from font units
    this->mFontMetrics.ascender =
            (float) (mFace->size->metrics.ascender) / 64.0f;
    this->mFontMetrics.descender =
            (float) (mFace->size->metrics.descender) / 64.0f;
}



const std::string &WmFont::GetFontFileName() const {
    return mFontFileName;
}


bool WmFont::IsCharInFont(const wchar_t charCode) {
    if (mLibrary == nullptr) {
        return false;
    }

    bool flag = LoadFaceIfNot();
    if (!flag) {
        return false;
    }
    return wmcanvas::WmFT_IsCharInFace(mFace, charCode);
}


bool WmFont::LoadFaceIfNot() {
    bool flag;
    if (mFace == nullptr) {
        flag = wmcanvas::WmFT_LoadFace(mLibrary, &mFace, mFontFileName.data());
        if (!flag) { // load face or change face size fail,
            wmcanvas::WmFT_DisposeFaceSafe(mFace);
            mFace = nullptr;
        }
    } else {
        flag = true;
    }
    return flag;
}


bool WmFont::PrepareLoadGlyph(float fontSize, float scaleX, float scaleY) {
    if (mFace == nullptr) {
        return false;
    }

    float sizeW = fontSize * scaleX;
    float sizeH = fontSize * scaleY;

    return wmcanvas::WmFT_SetFaceCharSize(mFace, sizeW, sizeH);
}


bool WmFont::LoadStroke(const char *filename, FT_Stroker *stroker, float sx, float sy,float lineWidth,float deviceRatio) {
    assert(filename);
    if (mLibrary == nullptr) {
        return false;
    }

    FT_Error error;
    error = FT_Stroker_New(mLibrary, stroker);
    if (error) {
        FT_Stroker_Done(*stroker);
        return false;
    }

    float magicThick = 12 * sx;
    FT_Stroker_Set(*stroker, (int) (mOutlineThickness * magicThick),
                   FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
    return true;
}



#endif
