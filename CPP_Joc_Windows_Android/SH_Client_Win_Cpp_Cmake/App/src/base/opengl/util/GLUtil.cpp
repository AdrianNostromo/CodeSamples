#include <iostream>
#include "GLUtil.h"

IGL* GLUtil::gl = nullptr;
opengl::IRenderContext* GLUtil::renderContext = nullptr;

void GLUtil::clearGLErrors() {
    while(GLUtil::gl->getError() != IGL::NO_ERROR) {
        //void
    }
}

bool GLUtil::logGLErrorIfAny(const char* function, const char* file, int line) {
    unsigned int errorCode = GLUtil::gl->getError();

    if(errorCode != IGL::NO_ERROR) {
        std::string errorName;
        if (errorCode == IGL::INVALID_ENUM) { errorName = "INVALID_ENUM"; }
        else if (errorCode == IGL::INVALID_VALUE) { errorName = "INVALID_VALUE"; }
        else if (errorCode == IGL::INVALID_OPERATION) { errorName = "INVALID_OPERATION"; }
        else if (errorCode == IGL::OUT_OF_MEMORY) { errorName = "OUT_OF_MEMORY"; }
        else if (errorCode == IGL::INVALID_FRAMEBUFFER_OPERATION) { errorName = "INVALID_FRAMEBUFFER_OPERATION"; }
        else {errorName = std::to_string(errorCode);}

        std::cout << "[OpenGL Error] (" << errorName << ")\nFunction: " << function << "\nFile: " << file << "\nLine " << line << std::endl;

        return false;
    }

    return true;
}
