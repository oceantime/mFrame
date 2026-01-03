/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#ifndef WMCANVAS_WMSHADER_H
#define WMCANVAS_WMSHADER_H

#include "WmGL.h"
#include "WmPoint.h"
#include "WmTransform.h"
#include "../../support/Log.h"

#include <vector>

class WmShader
{
public:
    WmShader(const char *name, const char *vertexShaderSrc,
            const char *fragmentShaderSrc);

    virtual ~WmShader();


#ifdef ANDROID

    bool initWithPreCompiledProgramByteArray(const char *shaderName,
                                             const GLchar *vShaderByteArray,
                                             const GLchar *fShaderByteArray);

#endif

    void Bind();

    const std::string &GetName() { return mName; }

    virtual void RestoreShaderState() {}

    virtual void SetTextSampler(int value) {}

    virtual void SetHasTexture(bool value) {}

    virtual void SetOverideTextureColor(int value) {}
    
    WmLint GetTexcoordSlot() { return mTexcoordSlot; }
    
    WmLint GetPositionSlot() { return mPositionSlot; }
    
    WmLint GetColorSlot() { return mColorSlot; }
    
    WmLint GetTransformSlot() { return mTransfromSlot; }

    virtual void SetRepeatMode(const std::string &pattern) {}

    virtual void SetRange(const GPoint &start, const GPoint &end) {}

    virtual void SetRange(const float *start, const float *end) {}

    virtual void SetColorStopCount(int count) {}

    virtual void SetColorStop(float *pack_color, float stop, int id) {}

    virtual void SortColorStop() {}

    virtual void SetTextureSize(float width, float height) {}

    void SetTransform(const WmTransform &trans);

    virtual void SetDelta(float x, float y) {}

    virtual void SetWeight(float w[], int count) {}

    std::vector<WmCanvasLog>& GetErrorVector(){ return mErrVec;}
    
    void ClearErrorVector() { mErrVec.clear(); }
    
    bool IsShaderCompleted() { return isCompleted; }


    static void TraceErrorIfHas(WmShader* shader, WmCanvasHooks* hook, const std::string& contextId);


protected:
    virtual void calculateAttributesLocations();

    WmLuint compileShader(const char *shader, GLenum shaderType);

    WmLuint mHandle;
    std::string mName;
    
    WmLint mTexcoordSlot;
    WmLint mPositionSlot;
    WmLint mColorSlot;
    
    WmLint mTransfromSlot;
    WmTransform mShaderTransform;
    bool mIsFirstCommit;

    std::vector<WmCanvasLog> mErrVec;
    bool isCompleted;
};

class DefaultShader : public WmShader
{
public:
    DefaultShader(const char *name, const char *vertexShaderSrc,
                  const char *fragmentShaderSrc);

    ~DefaultShader();

    void SetTextSampler(int value)
    {
        glUniform1i(mTextureSamplerSlot, value);
    }

    void SetHasTexture(bool value)
    {
        if (mHasTextureFlag != value)
        {
            mHasTextureFlag = value;
            glUniform1i(mHasTextureSlot, value);

        }
    }

    void SetOverideTextureColor(int value)
    {
        mOverrideTextureColorFlag = value;
        glUniform1i(mOverrideTextureColorSlot, value);
    }

    void RestoreShaderState()
    {
        SetHasTexture(mHasTextureFlag);
        SetOverideTextureColor(mOverrideTextureColorFlag);
    }

protected:
    void calculateAttributesLocations();

private:
    WmLint mTextureSamplerSlot;
    WmLint mHasTextureSlot;
    WmLint mOverrideTextureColorSlot;

    bool mHasTextureFlag;
    bool mOverrideTextureColorFlag;
};

class TextureShader : public WmShader
{
public:
    TextureShader(const char *name, const char *vertexShaderSrc,
                  const char *fragmentShaderSrc);

    ~TextureShader() = default;

    void SetTextSampler(int value)
    {
        glUniform1i(mTextureSamplerSlot, value);
    }
    
    void SetPremultipliedAlpha(bool value)
    {
        glUniform1i(mPremultipliedAlphaSlot, value);
    }

protected:
    void calculateAttributesLocations();

private:
    WmLint mTextureSamplerSlot;
    WmLint mPremultipliedAlphaSlot;
};

class ShadowShader : public WmShader
{
public:
    ShadowShader(const char *name, const char *vertexShaderSrc,
                  const char *fragmentShaderSrc);

    ~ShadowShader() = default;

    void SetTextSampler(int value)
    {
        glUniform1i(mTextureSamplerSlot, value);
    }
    
    void SetShadowColor(float *color)
    {
        glUniform4f(mShadowColorSlot, color[0], color[1], color[2], color[3]);
    }

protected:
    void calculateAttributesLocations();

private:
    WmLint mTextureSamplerSlot;
    WmLint mShadowColorSlot;
};

class BlurShader : public WmShader
{
public:
    BlurShader(const char *name, const char *vertexShaderSrc,
                 const char *fragmentShaderSrc);

    ~BlurShader() = default;


    void SetDelta(float x, float y)
    {
        glUniform1f(mXDeltaSlot, x);
        glUniform1f(mYDeltaSlot, y);
    }


    void SetBlurRadius(int r) {
        glUniform1i(mRadiusSlot, r);
    }

    void SetWeight(float w[], int count)
    {
        glUniform1fv(mWeightSlot, count, w);
    }

    void SetOverideTextureColor(int value)
    {
        glUniform1i(mOverrideTextureColorSlot, value);
    }

protected:
    void calculateAttributesLocations();

private:
    WmLint mRadiusSlot;
    WmLint mXDeltaSlot;
    WmLint mYDeltaSlot;
    WmLint mWeightSlot;
    WmLint mSamplerSlot;
    WmLint mOverrideTextureColorSlot;
};

class PatternShader : public WmShader
{
public:
    PatternShader(const char *name, const char *vertexShaderSrc,
                  const char *fragmentShaderSrc);

    void SetTextSampler(int value)
    {
        glUniform1i(mTextureSamplerSlot, value);
    }

    void SetRepeatMode(const std::string &pattern)
    {
        glUniform1i(mRepeatXSlot,
                    (pattern == "repeat" || pattern == "repeat-x"));
        glUniform1i(mRepeatYslot,
                    (pattern == "repeat" || pattern == "repeat-y"));
    }

    void SetTextureSize(float width, float height)
    {
        glUniform2f(mTextureSizeSlot, width, height);
    }
    
    void SetPatternAlpha(float patternAlpha)
    {
        glUniform1f(mPatternAlphaSlot, patternAlpha);
    }

    WmLint GetTextureSamplerSlot() { return mTextureSamplerSlot; }

protected:
    void calculateAttributesLocations();
    
    WmLint mTextureSamplerSlot;
    WmLint mRepeatXSlot;
    WmLint mRepeatYslot;
    WmLint mTextureSizeSlot;
    WmLint mPatternAlphaSlot;
};


class GradientShader : public WmShader
{
public:
    GradientShader(const char *name, const char *vertexShaderSrc, const char *fragmentShaderSrc);
    
    void SetHasTexture(bool value)
    {
        if (mHasTextureFlag != value)
        {
            mHasTextureFlag = value;
            glUniform1i(mHasTextureSlot, value);
            
        }
    }
    
    void RestoreShaderState() { SetHasTexture(mHasTextureFlag); }
    
    void SetColorStopCount(int count) { glUniform1i(mStopCountSlot, count); }
    
    void SetColorStop(float *color, float stop, int id)
    {
        WmLint stopLoc;
        WmLint colorLoc;
        if (id == 0)
        {
            stopLoc = mStop0Slot;
            colorLoc = mStop0ColorSlot;
        }
        else if (id == 1)
        {
            stopLoc = mStop1Slot;
            colorLoc = mStop1ColorSlot;
        }
        else if (id == 2)
        {
            stopLoc = mStop2Slot;
            colorLoc = mStop2ColorSlot;
        }
        else if (id == 3)
        {
            stopLoc = mStop3Slot;
            colorLoc = mStop3ColorSlot;
        }
        else if (id == 4)
        {
            stopLoc = mStop4Slot;
            colorLoc = mStop4ColorSlot;
        }
        else
        {
            return;
        }
        
        glUniform1f(stopLoc, stop);
        glUniform4f(colorLoc, color[0], color[1], color[2], color[3]);
    }
    
protected:
    void calculateAttributesLocations();
    
    WmLint mTextureSamplerSlot;
    
    WmLint mStopCountSlot;
    
    WmLint mStop0ColorSlot;
    WmLint mStop1ColorSlot;
    WmLint mStop2ColorSlot;
    WmLint mStop3ColorSlot;
    WmLint mStop4ColorSlot;
    
    WmLint mStop0Slot;
    WmLint mStop1Slot;
    WmLint mStop2Slot;
    WmLint mStop3Slot;
    WmLint mStop4Slot;
    
    WmLint mHasTextureSlot;
    bool mHasTextureFlag;
};

class LinearGradientShader : public GradientShader
{
public:
    LinearGradientShader(const char *name, const char *vertexShaderSrc,
                         const char *fragmentShaderSrc);

    void SetRange(const GPoint &start, const GPoint &end)
    {
        glUniform2f(mRangeStartSlot, start.x, start.y);
        glUniform2f(mRangeEndSlot, end.x, end.y);
    }

protected:
    void calculateAttributesLocations();
    WmLint mRangeStartSlot;
    WmLint mRangeEndSlot;
};

class RadialGradientShader : public GradientShader
{
public:
    RadialGradientShader(const char *name, const char *vertexShaderSrc,
                         const char *fragmentShaderSrc);

    void SetRange(const float *start, const float *end)
    {
        glUniform3f(mStartSlot, start[0], start[1], start[2]);
        glUniform3f(mEndSlot, end[0], end[1], end[2]);
    }
    
    void SetInvertTransform(const WmTransform &trans);
    
protected:
    void calculateAttributesLocations();
    WmLint mStartSlot;
    WmLint mEndSlot;
    WmLint mInvertTransformSlot;

};

#endif /* WMCANVAS_WMSHADER_H */
