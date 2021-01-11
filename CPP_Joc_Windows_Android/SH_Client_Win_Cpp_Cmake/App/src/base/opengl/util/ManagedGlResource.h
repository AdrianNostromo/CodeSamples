#pragma once

#include <base/gh.h>
#include <base/object/ManagedObject.h>
#include "IManagedGlResource.h"
#include <base/opengl/IGL.h>


namespace graphics {
    class ShaderProgram;
};

// NONE; Gl management will not be used (eg. for textures that are part of a texture_array).
class ManagedGlResource : public base::ManagedObject, virtual public IManagedGlResource {priv typedef base::ManagedObject super;pub dCtor(ManagedGlResource);
    pub BindLocations::Config* bindLocation;

    prot unsigned int gHandle = 0;
    // This is used by the default frambuffer only because it is a default resource, not a created resource, so it has a 0 gHandle.
    prot bool resourceHasZeroGHandle = false;
    // Tex_array has optional gHandle.
    prot bool resourceHasOptionalGHandle = false;
    prot unsigned int loggedTextureSizeBytes = 0;

    // This is reset to -1 when gl is destroyed.
    priv int loadedOnGlContextIndex = -1;

    // These are managed by the RenderContext without calling bind, unbind functions.
    /// Don't use these locally. Only the RenderContext may use these.
    pub bool _isBound = false;
    // Use for textures.
    pub int _bound_slot = -1;
    // Used for vertex_buffer_object;
    pub graphics::ShaderProgram* _bound_shader = nullptr;

    pub explicit ManagedGlResource(BindLocations::Config* bindLocation);
    prot void createPost() override;

    pub void loadToGl();
    prot void unloadFromGl();
    prot virtual void onLoadToGl();
    prot virtual void onUnloadFromGl();

    // These call request functions on RenderContext and that calls onBind and onUnbindPre only if needed.
    pub void bind(bool isStorageAllocated=true);
    pub void bind(int slot, bool isStorageAllocated=true);
    pub void bind(graphics::ShaderProgram* shader, bool isStorageAllocated=true);
    pub void unbind();

    pub virtual void onBind(int slot, graphics::ShaderProgram* shader);
    pub virtual void onUnbind(int slot, graphics::ShaderProgram* shader);
    // Already bound when this is called.
    pub virtual void updateGLBufferIfNeeded(graphics::ShaderProgram* shader);

    pub int getBoundSlotOptional();
    pub int getBoundSlotMustExist();

    pub void onGlContextCreated() override;
    pub void onGlContextDestroyedPre() override;

    // Used for dev only.
    pub unsigned int getGHandle();

    prot void disposeMainPre() override;
    prot void disposeMain() override;
    pub ~ManagedGlResource() override;
};
