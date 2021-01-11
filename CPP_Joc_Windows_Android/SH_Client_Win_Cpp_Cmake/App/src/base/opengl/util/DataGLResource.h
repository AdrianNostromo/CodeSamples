#pragma once

#include <base/gh.h>
#include "ManagedGlResource.h"

class DataGLResource : public ManagedGlResource {priv typedef ManagedGlResource super;pub dCtor(DataGLResource);
    // This can be used when a resource size changes (eg. TextureArray)
    priv bool isInvalidAllocationBlock = false;
    // This is false at the start to avoid a initial buffer update when the resource is created.
    // Data init functions will call invalidateData and that will cause a updat eto occur if needed.
    priv bool isInvalidData = false;

    pub explicit DataGLResource(BindLocations::Config* bindLocation);

    prot void onLoadToGl() override;
    prot void onUnloadFromGl() override;

    pub void updateGLBufferIfNeeded(graphics::ShaderProgram* shader) override;

    // Call this when data is written in loadToGL;
    prot void dataValidatedOnLoadToGl();
    pub void invalidAllocationBlock();
    pub virtual void invalidateData();
    
    prot virtual void onValidateData();

    pub ~DataGLResource() override;
};
