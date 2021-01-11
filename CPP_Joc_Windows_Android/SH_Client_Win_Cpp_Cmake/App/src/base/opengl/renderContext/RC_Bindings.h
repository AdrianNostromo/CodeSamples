#pragma once

#include <base/gh.h>
#include "RC_Props.h"
#include <base/list/Array2D.h>

class ManagedGlResource;

namespace opengl {
class RC_Bindings : public RC_Props {priv typedef RC_Props super;pub dCtor(RC_Bindings);
    // This is a general number, should be enough for the entire requirements (buffers, textures, ...).
    priv static const int MAX_BINDING_RESOURCES_COUNT;

    priv Array2D<ManagedGlResource*> boundGLResourcesList;

    pub explicit RC_Bindings();

    pub void bindRequest(ManagedGlResource* glResource, bool isStorageAllocated, int slot = 0, graphics::ShaderProgram* shader = nullptr) final;
    pub void unbindRequest(ManagedGlResource* glResource) final;

    pub void unbindShaderDependants(graphics::ShaderProgram* shader) final;

    pub void onGlContextDestroyedPre() override;

    pub ~RC_Bindings() override;
};
};
