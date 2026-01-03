/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#include "WmShaderManager.h"
#include "WmShader.h"
#include "shaders/default.glsl"
#include "shaders/grad.glsl"
#include "shaders/pattern.glsl"
#include "shaders/radiation.glsl"
#include "shaders/texture.glsl"
#include "shaders/blur.glsl"
#include "shaders/shadow.glsl"

#ifdef ANDROID
#include "WmPreCompiledShaders.h"
bool g_use_pre_compile = false;
#endif


WmShaderManager *WmShaderManager::mShaderManager = nullptr;

WmShaderManager *WmShaderManager::getSingleton()
{
    if (nullptr == mShaderManager)
    {
        mShaderManager = new WmShaderManager();
    }
    return mShaderManager;
}

void WmShaderManager::release()
{
    if (mShaderManager != nullptr)
    {
        delete mShaderManager;
        mShaderManager = nullptr;
    }
}


void WmShaderManager::addProgram(const std::string &key, WmShader *program)
{
    std::map< std::string, WmShader * >::iterator iter =
        mProgramCaches.find(key);
    if (iter == mProgramCaches.end())
    {
        mProgramCaches.insert(
            std::pair< std::string, WmShader * >(key, program));
    }
    else if (iter->second != program)
    {
        delete (iter->second);
        iter->second = program;
    }
}

WmShader *WmShaderManager::programForKey(const std::string &key)
{
    std::map< std::string, WmShader * >::iterator iter =
        mProgramCaches.find(key);
    if (iter == mProgramCaches.end())
    {
        return nullptr;
    }

    return iter->second;
}

WmShaderManager::WmShaderManager()
{
#ifdef ANDROID
    if (g_use_pre_compile)
    {
        const char *extString = (const char *)glGetString(GL_EXTENSIONS);
        bool isPreCompileSupported =
            (strstr(extString, "GL_OES_get_program_binary") != 0);

        if (isPreCompileSupported)
        {
            WmPreCompiledShaders::getInstance()->SetSupportPreCompiledShaders(
                    true);
        }
    }
#endif

    LoadDefaultShaders();

#ifdef ANDROID
    if (g_use_pre_compile)
    {
        if (WmPreCompiledShaders::getInstance()
                ->GetSupportPreCompiledShaders())
        {
            WmPreCompiledShaders::getInstance()->SavePreCompiledShaders();
        }
    }
#endif
}

WmShaderManager::~WmShaderManager()
{
    std::map< std::string, WmShader * >::iterator iter =
        mProgramCaches.begin();
    for (; iter != mProgramCaches.end(); ++iter)
    {
        delete iter->second;
        iter->second = nullptr;
    }
}


void WmShaderManager::LoadDefaultShaders() {
    allShaderCompleted = true;
    
    WmShader *program =
        new DefaultShader(DEFAULT_SHADER, DEFAULT_SHADER_VS, DEFAULT_SHADER_PS);
    addProgram(DEFAULT_SHADER, program);
    if (!program->IsShaderCompleted()) {
        allShaderCompleted = false;
    }

    program =
        new TextureShader(TEXTURE_SHADER, TEXTURE_SHADER_VS, TEXTURE_SHADER_PS);
    addProgram(TEXTURE_SHADER, program);
    if (!program->IsShaderCompleted()) {
        allShaderCompleted = false;
    }
    
    program =
        new ShadowShader(SHADOW_SHADER, SHADOW_SHADER_VS, SHADOW_SHADER_PS);
    addProgram(SHADOW_SHADER, program);
    if (!program->IsShaderCompleted()) {
        allShaderCompleted = false;
    }

    program =
            new BlurShader(BLUR_SHADER, BLUR_SHADER_VS, BLUR_SHADER_PS);
    addProgram(BLUR_SHADER, program);
    if (!program->IsShaderCompleted()) {
        allShaderCompleted = false;
    }

    program =
            new PatternShader(PATTERN_SHADER, PATTERN_SHADER_VS, PATTERN_SHADER_PS);
    addProgram(PATTERN_SHADER, program);
    if (!program->IsShaderCompleted()) {
        allShaderCompleted = false;
    }

    program = new LinearGradientShader(LINEAR_SHADER, LINEAR_SHADER_VS,
                                       LINEAR_SHADER_PS);
    addProgram(LINEAR_SHADER, program);
    if (!program->IsShaderCompleted()) {
        allShaderCompleted = false;
    }

    program = new RadialGradientShader(RADIAL_SHADER, RADIAL_SHADER_VS,
                                       RADIAL_SHADER_PS);
    addProgram(RADIAL_SHADER, program);
    if (!program->IsShaderCompleted()) {
        allShaderCompleted = false;
    }
}
