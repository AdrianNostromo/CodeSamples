#pragma once

#include <base/gh.h>
#include <base/opengl/util/ManagedGlResource.h>
#include <base/list/CompositesBuffer.h>
#include <base/opengl/IGL.h>

class Texture;

namespace graphics {
class FrameBufferObject : public ManagedGlResource {priv typedef ManagedGlResource super;pub dCtor(FrameBufferObject);
    // This is the default fbo that always exists with a gHandle of 0.
    pub static FrameBufferObject* DefaultFBO;

    priv GLenum drawBuffer;
    priv GLenum readBuffer;

    priv Texture* depthMapTexture;
    // This is used as a small optimisation to avoid 1 extra bind when the framebuffer is created.
    priv int depthMapTextureAutoBindSlot;
    
    priv bool isDefaultFBO = false;

    pub explicit FrameBufferObject(
        bool isDefaultFBO);
    pub explicit FrameBufferObject(
        GLenum drawBuffer, GLenum readBuffer, Texture* depthMapTexture, int depthMapTextureAutoBindSlot);

    prot void onLoadToGl() override;
    prot void onUnloadFromGl() override;

    pub void onBind(int slot, graphics::ShaderProgram* shader) override;
    pub void onUnbind(int slot, graphics::ShaderProgram* shader) override;

    pub ~FrameBufferObject() override;
};
};
