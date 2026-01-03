#ifndef WMCANVAS_WMGL_H
#define WMCANVAS_WMGL_H

#ifndef _WIN32

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <GLES2/gl2ext.h>
#include <export.h>

#elif IOS
#include <OpenGLES/gltypes.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#elif  __linux__
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>
#include <EGL/egl.h>
#include <GLES2/gl2ext.h>
#include <string.h>
#include "FontTool.hpp"
#endif

#else
#include <GLES2/gl2.h>
#endif

using WmLuint = GLuint;
using WmLint = GLint;
using WmLenum = GLenum;
using WmLubyte = GLubyte;

#endif /* WMCANVAS_WMGL_H */
