/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#ifndef WMCANVAS_WMSTRSEPARATOR_H
#define WMCANVAS_WMSTRSEPARATOR_H

namespace wmcanvas
{
class WmStrSeparator
{
public:
    static const short PARSESEPSTRMAXCOUNT = 50;
    static const short PARSELOCALBUFFERSIZE = 480;

    WmStrSeparator();
    short SepStrBySpace(char *str, short maxCount = -1);
    short SepStrByCharArray(char *str, const char *byteArray, short byteCount,
                            short maxCount = -1);
    char **GetSepArray() { return (char **)mPointers; }

private:
    char *mPointers[PARSELOCALBUFFERSIZE];
};

using GStrSeparator = WmStrSeparator;

}

#endif /* WMCANVAS_WMSTRSEPARATOR_H */
