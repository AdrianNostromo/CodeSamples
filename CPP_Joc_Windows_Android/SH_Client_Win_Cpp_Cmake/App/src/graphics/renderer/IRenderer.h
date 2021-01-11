#pragma once

#include <base/gh.h>
#include <memory>
#include <base/rendering/util/IRenderableRenderer.h>
#include "renderPhase/IRenderPhase.h"

namespace graphics {
    class IRenderablesManager;
    class GeometryManager;
};

namespace graphics {
class IRenderer : public virtual IRenderableRenderer {
    pub virtual void addRenderPhase(IRenderPhase* renderPhase) = 0;
    pub virtual void removeRenderPhaseMustExist(int type) = 0;
    pub virtual void removeRenderPhaseMustExist(IRenderPhase* renderPhase) = 0;
    pub virtual IRenderPhase* getRenderPhase(int rPhaseType, bool mustExist) = 0;

    pub template <typename T>
    T getRenderPhaseAs(int validationRPhaseType, bool mustExist);

    pub virtual IRenderablesManager* getRenderablesManager() = 0;

    pub ~IRenderer() override = default;
};

template <typename T>
T IRenderer::getRenderPhaseAs(int validationRPhaseType, bool mustExist) {
    IRenderPhase* rPhase = getRenderPhase(validationRPhaseType, mustExist);
    if (rPhase == nullptr) {
        if (mustExist) {
            throw LogicException(LOC);
        }

        return nullptr;
    }

    T cRPhase = rPhase->castAs<T>(validationRPhaseType);

    return cRPhase;
}

};
