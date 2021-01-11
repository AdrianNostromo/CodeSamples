#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>

namespace graphics {
    class RenderableLiteInstanceVI;
};

class IRenderOrderDrawableNode2D {
    pub virtual ListDL<IRenderOrderDrawableNode2D*>::Entry*& getInStage_renderOrderDrawablesListEntryRef() = 0;
    pub virtual IRenderOrderDrawableNode2D*& getPreviousDrawableWithRenderableRef() = 0;

    pub virtual IRenderOrderDrawableNode2D* computePreviousOrParentDrawableInStageRenderOrder() = 0;
    
    pub virtual bool getHasRenderable() = 0;
    pub virtual graphics::RenderableLiteInstanceVI* getRenderData() = 0;
    pub virtual ~IRenderOrderDrawableNode2D() = default;
};
