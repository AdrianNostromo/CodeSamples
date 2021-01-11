#pragma once

#include <base/gh.h>
#include "RM_Base.h"
#include <base/list/ListDL.h>
#include <base/list/ArrayList.h>
#include <base/MM.h>
#include <graphics/renderer/renderablesManager/util/Entry_RenderablesManager.h>

namespace graphics {
    class NodeInstance;
};

namespace graphics {
class RM_List : public RM_Base {priv typedef RM_Base super;pub dCtor(RM_List);
    priv ListDL<Entry_RenderablesManager> renderablesList{LOC};

    pub explicit RM_List();

    pub void add(ModelInstance* modelInstance) final;
    pub void remove(ModelInstance* modelInstance) final;

    pub void add(Node* node) final;
    pub void remove(Node* node) final;

    pub void add(IRenderable* renderable) final;
    pub void insertAfter(IRenderable* renderable, IRenderable* insertAfterTarget) final;
    pub void remove(Entry_RenderablesManager* entry_renderablesManager) final;
    
    priv void dispatchAddNodePartsRecursive(NodeInstance* nodeInstance);
    priv void dispatchAddNodePartsRecursive(Node* node);
    priv void dispatchAddRenderable(IRenderable* renderable, bool usesInsertAfterMode, IRenderable* insertAfterNodePartTarget, Entry_RenderablesManager& entryRM);
    priv void dispatchAddNodePartsRecursive(ArrayList<NodeInstance*>& nodeInstancesList);
    priv void dispatchAddNodePartsRecursive(ArrayList<Node*>& nodesList);

    pub void syncModels();

    pub ~RM_List() override;
};
};
