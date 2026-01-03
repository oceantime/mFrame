#include "WmTreemap.h"


WmTreemap::WmTreemap(unsigned int w, unsigned int h) : mWidth(w), mHeight(h), mLineLast(w), mVerticalLast(h), mCurrentLineHeight(0) {

}

bool WmTreemap::Add(const WmSize &size, WmRect &rect) {
    rect.SetSize(size);
    if (size.width > mLineLast) {
        if (size.width > mWidth) {
            return false;
        } else {
            mVerticalLast -= mCurrentLineHeight;
            if (mVerticalLast < size.height) {
                mVerticalLast += mCurrentLineHeight;

                return false;
            } else {
                rect.SetPosition(0, mHeight - mVerticalLast);
                mCurrentLineHeight = size.height;
                mLineLast = mWidth - size.width;
            }
        }
    } else {
        rect.SetPosition(mWidth - mLineLast, mHeight - mVerticalLast);

        if (mCurrentLineHeight < size.height) {
            if (mVerticalLast < size.height) {
                return false;
            } else {
                mCurrentLineHeight = size.height;
                mLineLast -= size.width;
            }
        } else {
            mLineLast -= size.width;
        }
    }
    return true;
}

void WmTreemap::Clear() {
    mLineLast = mWidth;
    mVerticalLast = mHeight;
    mCurrentLineHeight = 0;
}
