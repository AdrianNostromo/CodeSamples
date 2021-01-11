#pragma once

#include <base/gh.h>
#include "RC_Base.h"
#include <base/opengl/IGL.h>

namespace opengl {
class RC_Props : public RC_Base {priv typedef RC_Base super;pub dCtor(RC_Props);
    priv bool isFlushed = true;

    // glDepthMask specifies whether the depth buffer is enabled for writing.If flag is GL_FALSE, depth buffer writing is disabled.Otherwise, it is enabled.Initially, depth buffer writing is enabled
    priv GLboolean depthMask = true;
    priv GLboolean depthMask_target = true;

    // >0; Enabled;
    // 0; disabled
    priv bool isDepthTestEnabled = false;
    priv GLenum depthFunction = 0;
    priv GLclampd depthRangeNear = 0.0f;
    priv GLclampd depthRangeFar = 1.0f;

    priv bool isDepthTestEnabled_target = false;
    priv GLenum depthFunction_target = 0;
    priv GLclampd depthRangeNear_target = 0.0f;
    priv GLclampd depthRangeFar_target = 1.0f;

    priv bool isBlendingEnabled = false;
    priv GLenum blendingSFactor = IGL::ONE;
    priv GLenum blendingDFactor = IGL::ZERO;
    
    priv bool isBlendingEnabled_target = false;
    priv GLenum blendingSFactor_target = IGL::ONE;
    priv GLenum blendingDFactor_target = IGL::ZERO;

    priv bool isCullFaceEnabled = false;
    priv bool isCullFaceEnabled_target = false;
    priv GLenum cullFaceMode = IGL::BACK;
    priv GLenum cullFaceMode_target = IGL::BACK;

    priv GLenum frontFaceMode = IGL::CCW;
    priv GLenum frontFaceMode_target = IGL::CCW;

    pub explicit RC_Props();

    pub void onGlContextCreated() override;

    pub bool getIsFlushed() final;
    pub void flush() final;

    pub void setDepthMask(GLboolean depthMask) final;
    pub void setDepthTest(bool isDepthTestEnabled = false, GLenum depthFunction = 0, GLclampd depthRangeNear = 0.0f, GLclampd depthRangeFar = 1.0f) final;
    pub void setBlending(bool isBlendingEnabled = false, GLenum blendingSFactor = IGL::ONE, GLenum blendingDFactor = IGL::ZERO) final;
    pub void setCullFace(bool isCullFaceEnabled = false, GLenum cullFaceMode = IGL::BACK) final;
    pub void setFrontFace(GLenum frontFaceMode = IGL::CCW) final;

    pub ~RC_Props() override;
};
};
