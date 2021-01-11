#pragma once

#include <base/gh.h>

namespace graphics {
    class ModelInstance;
    class Node;
    class IRenderable;
    class Entry_RenderablesManager;
};

namespace graphics {
class IRenderablesManager {
    pub virtual void add(ModelInstance* modelInstance) = 0;
    pub virtual void remove(ModelInstance* modelInstance) = 0;

    pub virtual void add(Node* node) = 0;
    pub virtual void remove(Node* node) = 0;
    
    pub virtual void add(IRenderable* renderable) = 0;
    // NOTE. Currently this is used only for 2d fragments. When 2d ModelInstances are used, need to pass a NodeInstance* also.
    pub virtual void insertAfter(IRenderable* renderable, IRenderable* insertAfterTarget) = 0;
    pub virtual void remove(Entry_RenderablesManager* entry_renderablesManager) = 0;

    pub virtual ~IRenderablesManager() = default;
};
};
