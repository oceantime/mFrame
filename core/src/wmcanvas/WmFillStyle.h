/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef WMCANVAS_WMFILLSTYLE_H
#define WMCANVAS_WMFILLSTYLE_H

#include "WmConvert.h"

class WmFillStyle
{
public:
    enum Style
    {
        STYLE_PATTERN,
        STYLE_LINEAR_GRADIENT,
        STYLE_RADIAL_GRADIENT,
        STYLE_UNDEFINED
    };

    WmFillStyle(Style style) : mStyle(style) {}

    virtual int GetTextureListID() { return -1; }
    virtual WmFillStyle *Clone() { return nullptr; }
    virtual bool AddColorStop(float pos, const std::string &color_name){ return false;}

    bool IsPattern() { return mStyle == STYLE_PATTERN; }
    bool IsLinearGradient() { return mStyle == STYLE_LINEAR_GRADIENT; }
    bool IsRadialGradient() { return mStyle == STYLE_RADIAL_GRADIENT; }
    bool IsDefault() { return mStyle == STYLE_UNDEFINED; }

    virtual ~WmFillStyle(){};

private:
    Style mStyle;
};

class FillStylePattern : public WmFillStyle
{
public:
    FillStylePattern(int texture_list_id, const std::string &pattern)
        : WmFillStyle(STYLE_PATTERN), mPattern(pattern), mTextureListId(texture_list_id)
    {
    }
    
    FillStylePattern(int texture_list_id, short textureWidth, short textHeight, const std::string &pattern)
    : WmFillStyle(STYLE_PATTERN),mTextureWidth(textureWidth), mTextureHeight(textHeight),
    mPattern(pattern), mTextureListId(texture_list_id)
    {
    }

    virtual ~FillStylePattern() { mPattern.clear(); }

    const std::string &GetPattern() const { return mPattern; }
    int GetTextureListID() { return mTextureListId; }
    short GetTextureWidth() { return mTextureWidth; }
    short GetTextureHeight() { return mTextureHeight; }


    WmFillStyle *Clone()
    {
        FillStylePattern *ptr =
            new FillStylePattern(mTextureListId, mPattern);
        return ptr;
    }

private:
    std::string mPattern;
    int mTextureListId;
    short mTextureWidth;
    short mTextureHeight;
};

class FillStyleLinearGradient : public WmFillStyle
{
public:
    static const int MAX_STOP_NUM = 5;
    struct ColorStop
    {
        float pos;
        WmColorRGBA color;
    };

    FillStyleLinearGradient(const WmPoint &start_pos, const WmPoint &end_pos)
        : WmFillStyle(STYLE_LINEAR_GRADIENT), mStartPos(start_pos),
          mEndPos(end_pos), mStopCount(0)
    {
    }

    bool AddColorStop(float pos, const std::string &color_name)
    {
        if (mStopCount < MAX_STOP_NUM)
        {
            mStops[mStopCount].pos = pos;
            mStops[mStopCount].color = wmcanvas::StrValueToColorRGBA(color_name.c_str());
            mStopCount++;
            return true;
        }
        return false;
    }

    WmFillStyle *Clone()
    {
        FillStyleLinearGradient *ptr =
            new FillStyleLinearGradient(mStartPos, mEndPos);
        *ptr = *this;
        return ptr;
    }

    const WmPoint &GetStartPos() { return mStartPos; }
    const WmPoint &GetEndPos() { return mEndPos; }
    int GetColorStopCount() const { return mStopCount; }
    const ColorStop *GetColorStop(int id)
    {
        if (id >= 0 && id < mStopCount)
        {
            return &mStops[id];
        }
        return nullptr;
    }

private:
    WmPoint mStartPos, mEndPos;
    int mStopCount;
    ColorStop mStops[MAX_STOP_NUM];
};

class FillStyleRadialGradient : public WmFillStyle
{
public:
    static const int MAX_STOP_NUM = 5;
    struct ColorStop
    {
        float pos;
        WmColorRGBA color;
    };

    FillStyleRadialGradient(const float *start, const float *end)
        : WmFillStyle(STYLE_RADIAL_GRADIENT), mStopCount(0)
    {
        mStart[0] = start[0];
        mStart[1] = start[1];
        mStart[2] = start[2];
        mEnd[0] = end[0];
        mEnd[1] = end[1];
        mEnd[2] = end[2];
    }

    bool AddColorStop(float pos, const std::string &color_name)
    {
        if (mStopCount < MAX_STOP_NUM)
        {
            mStops[mStopCount].pos = pos;
            mStops[mStopCount].color = wmcanvas::StrValueToColorRGBA(color_name.c_str());
            mStopCount++;
            return true;
        }
        return false;
    }

    const float *GetStartPos() { return mStart; }
    const float *GetEndPos() { return mEnd; }
    int GetColorStopCount() const { return mStopCount; }
    const ColorStop *GetColorStop(int id)
    {
        if (id >= 0 && id < mStopCount)
        {
            return &mStops[id];
        }
        return nullptr;
    }

    WmFillStyle *Clone()
    {
        FillStyleRadialGradient *ptr =
            new FillStyleRadialGradient(mStart, mEnd);
        *ptr = *this;
        return ptr;
    }

private:
    float mStart[3], mEnd[3];
    int mStopCount;
    ColorStop mStops[MAX_STOP_NUM];
};

using GFillStyle = WmFillStyle;

#endif /* WMCANVAS_WMFILLSTYLE_H */
