#pragma once

#include <base/gh.h>
#include <base/opengl/IGL.h>

class ManagedGlResource;
namespace graphics {
    class ShaderProgram;
};

namespace opengl {
class IRenderContext {
    pub virtual bool getIsFlushed() = 0;
    pub virtual void flush() = 0;

    pub virtual void setDepthMask(GLboolean depthMask) = 0;
    pub virtual void setDepthTest(bool isDepthTestEnabled = false, GLenum depthFunction = 0, GLclampd depthRangeNear = 0.0f, GLclampd depthRangeFar = 1.0f) = 0;
    pub virtual void setBlending(bool isBlendingEnabled = false, GLenum blendingSFactor = IGL::ONE, GLenum blendingDFactor = IGL::ZERO) = 0;
    pub virtual void setCullFace(bool isCullFaceEnabled = false, GLenum cullFaceMode = IGL::BACK) = 0;
    pub virtual void setFrontFace(GLenum frontFaceMode = IGL::CCW) = 0;

    pub virtual void bindRequest(ManagedGlResource* glResource, bool isStorageAllocated, int slot=0, graphics::ShaderProgram* shader=nullptr) = 0;
    pub virtual void unbindRequest(ManagedGlResource* glResource) = 0;

    // This is called when a shader is unloaded.
    pub virtual void unbindShaderDependants(graphics::ShaderProgram* shader) = 0;

    pub virtual ~IRenderContext() = default;
};
};
