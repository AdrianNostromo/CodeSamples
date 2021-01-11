#pragma once

#include "../IGL.h"
#include <base/exceptions/LogicException.h>
#include <base/opengl/renderContext/IRenderContext.h>
#include <base/DevUtils.h>
#include <assert.h>

class GLUtil {
    // This is init by GLBase constructor.
    pub static IGL* gl;
    pub static opengl::IRenderContext* renderContext;

    pub static void clearGLErrors();
    pub static bool logGLErrorIfAny(const char* function, const char* file, int line);

};

#define CallGLChecked(x) GLUtil::clearGLErrors();\
  x;\
  assert(GLUtil::logGLErrorIfAny(#x, __FILE_NAME__, __LINE__))
