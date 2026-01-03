#ifndef WMCANVAS_WMTEXTURE_H
#define WMCANVAS_WMTEXTURE_H

#include "WmGL.h"
#include <map>
#include <vector>

// -----------------------------------------------------------
// --    Texture utility class
// --    Used by loadTexture
// -----------------------------------------------------------
class Texture
{
public:
    Texture(int glID, int w, int h)
    {
        mGlId = glID;
        mWidth = w;
        mHeight = h;
    }

    int GetGlID() const { return mGlId; }
    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }

    void Bind() const { glBindTexture(GL_TEXTURE_2D, (GLuint)mGlId); }

private:
    int mGlId;
    int mWidth;
    int mHeight;
};

class TextureGroup
{
public:
    TextureGroup(){};
    ~TextureGroup();

    bool IsSplit() const;
    void Clear();
    void Append(int glID, int width, int height);
    const Texture *Get(int id) const;

public:
    std::vector< Texture * > mVecTexture;
    int mSrcWidth;   // enable when split
    int mSrcHeight;   // enable when split
    int mTileWidth;  // enable when split
    int mTileHeight; // enable when split
};

class TextureMgr
{
public:
    TextureMgr();
    bool AppendPng(const unsigned char *buffer, unsigned int size, int textureGroupId,
                   unsigned int *widthPtr, unsigned int *heightPtr);
    void Append(int id, int glID, int width, int height);
    void Remove(int id);
    void Clear();
    const TextureGroup *Get(int id) const;

private:
    std::map< int, TextureGroup > mTextureGroupPool;
};

struct WmCanvasLog;

class WmTexture
{
public:
    WmTexture(unsigned int w, unsigned int h, WmLenum format = GL_RGBA,
              WmLubyte *pixels = nullptr, std::vector<WmCanvasLog> *errVec = nullptr);
    WmTexture(const char *filePath);
    WmTexture();
    ~WmTexture();

    void CreateTexture(WmLubyte *pixels, std::vector<WmCanvasLog> *errVec = nullptr);

    void Bind() const;
    void Unbind() const;

    unsigned int GetWidth() const { return mWidth; }
    unsigned int GetHeight() const { return mHeight; }
    WmLenum GetFormat() const { return mFormat; }
    void SetWidth(const unsigned int w) { mWidth = w; }
    void SetHeight(const unsigned int h) { mHeight = h; }
    void SetFormat(const WmLenum format) { mFormat = format; }
    WmLuint GetTextureID() const { return mTextureID; }
    void SetTextureID(const WmLuint id) { mTextureID = id; }
    void UpdateTexture(WmLubyte *pixels, int x, int y, int w, int h);

    // Caution: the memeory allocated by callback should be allocated by new []
    static void SetLoadPixelCallback(WmLubyte *(*callback)(const char *filePath,
                                                          unsigned int *w,
                                                          unsigned int *h));

    unsigned int size();

    void Detach()
    {
        mTextureID = 0;
    }

    bool IsValidate()
    {
        return mTextureID != 0;
    }

private:
    static WmLubyte *(*loadPixelCallback)(const char *filePath, unsigned int *w,
                                         unsigned int *h);
    static WmLubyte *loadPixelsFromPNG(const char *filePath, unsigned int *w,
                                      unsigned int *h);

private:
    unsigned int mWidth;
    unsigned int mHeight;
    WmLenum mFormat;
    WmLuint mTextureID;
};

using GTexture = WmTexture;

#endif /* WMCANVAS_WMTEXTURE_H */
