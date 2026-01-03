/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#ifndef WMCANVAS_WMSHADERMANAGER_H
#define WMCANVAS_WMSHADERMANAGER_H

#include <iostream>
#include <map>

#include "export.h"

class WmShader;

class WmShaderManager
{
public:

    // Android depend constructor, set it public
    API_EXPORT WmShaderManager();
    ~WmShaderManager();

    static WmShaderManager *getSingleton();
    static void release();

    void addProgram(const std::string &key, WmShader *program);
    WmShader *programForKey(const std::string &key);
    
    bool isAllShaderCompleted() { return allShaderCompleted; }

private:
    void LoadDefaultShaders();

    std::map< std::string, WmShader * > mProgramCaches;
    static WmShaderManager *mShaderManager;
    bool allShaderCompleted;
};

#endif /* WMCANVAS_WMSHADERMANAGER_H */
