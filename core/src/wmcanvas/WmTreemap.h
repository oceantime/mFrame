/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef WMCANVAS_WMTREEMAP_H
#define WMCANVAS_WMTREEMAP_H

#include <new>

struct WmSize
{
    WmSize(int w, int h) : width(w), height(h)
    {

    }

    int width;
    int height;
};

using GSize = WmSize;

using GSize = WmSize;

struct WmRect
{
    WmRect(int x = 0, int y = 0, int w = 0, int h = 0) : x(x), y(y), width(w), height(h)
    {

    }

    WmRect(int x, int y, const WmSize &size) : x(x), y(y), width(size.width), height(size.height)
    {

    }

    void Set(int x, int y, const WmSize &size)
    {
        new (this) WmRect(x, y, size);
    }

    void SetPosition(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    void SetSize(const WmSize &size)
    {
        width = size.width;
        height = size.height;
    }

    int x;
    int y;
    int width;
    int height;
};

using GRect = WmRect;
using GRect = WmRect;


class WmTreemap
{
public:
    WmTreemap(unsigned int w, unsigned int h);

    ~WmTreemap()
    {};

    inline unsigned int GetWidth()
    { return mWidth; };

    inline unsigned int GetHeight()
    { return mHeight; };

    bool Add(const WmSize &size, WmRect &rect);

    void Clear();

private:
    unsigned int mWidth;
    unsigned int mHeight;
    int mLineLast;
    int mVerticalLast;
    int mCurrentLineHeight;
};

using GTreemap = WmTreemap;

#endif /* WMCANVAS_WMTREEMAP_H */
