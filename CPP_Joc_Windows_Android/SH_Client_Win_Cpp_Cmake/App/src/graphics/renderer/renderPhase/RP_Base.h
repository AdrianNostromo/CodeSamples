#pragma once

#include <base/gh.h>
#include "IRenderPhase.h"
#include <base/object/ManagedObject.h>
#include <graphics/attribute/AttributesList.h>

namespace graphics {
    class ShadersPool;
    class ShaderProgram;
};

namespace graphics {
class RP_Base : public base::ManagedObject, public virtual IRenderPhase {priv typedef base::ManagedObject super; pub dCtor(RP_Base);
    // This is used to sort the render phases.
    // Make sure each phase in a renderer has a unique value.
    // Lower values renderer first.
    priv int renderOrder;

    prot const bool useDepthTest;

    // Disabled by default and must be enabled after create.
    priv bool isEnabled = false;

    prot IRenderer* renderer = nullptr;
    priv IListEntry* inHandlerListEntry = nullptr;

    prot ShadersPool* shadersPool = nullptr;
    prot AttributesList rendererAttributesList{};

    pub explicit RP_Base(int renderOrder, bool useDepthTest);

    pub int getRenderOrder() final;

    pub bool getIsEnabled() final;
    pub void setIsEnabled(bool isEnabled) final;
    prot virtual void onIsEnabledChanged();

    pub IListEntry*& getInHandlerListEntryRef() final;
    pub IRenderer* getRenderer() final;
    pub void setRenderer(IRenderer* renderer) final;
    prot virtual void onAddedToRenderer();
    prot virtual void onRemoveFromRendererPre();

    prot void dispose() override;
    pub ~RP_Base() override;
};
};
