#include "RM_List.h"
#include "util/IHandlerRenderablesManager.h"
#include <graphics/model/instance/ModelInstance.h>
#include <graphics/model/instance/NodeInstance.h>
#include <graphics/model/Node.h>
#include <graphics/model/Model.h>
#include <graphics/model/INodePart.h>
#include <graphics/model/NodePart.h>
#include <graphics/renderer/renderPhase/IRenderPhase.h>
#include <graphics/renderer/renderablesManager/IRenderablesManager.h>
#include <graphics/renderer/renderPhase/util/IRenderable.h>
#include <graphics/renderer/geometryManager/util/Entry_GeometryManagerBucket.h>
#include <graphics/renderer/geometryManager/IGeometryManager.h>
#include <cassert>

using namespace graphics;

RM_List::RM_List()
	: super()
{
	//void
}

void RM_List::add(ModelInstance* modelInstance) {
	throw LogicException(LOC);//asd_r_1a;// implement new method.
	/*if (modelInstance->inRenderer_listEntry != nullptr) {
		throw LogicException(LOC);
	}

	modelInstance->inRenderer_listEntry = modelInstancesList.appendDirect(modelInstance);
	modelInstance->handlingRenderer = this;

	dispatchAddNodePartsRecursive(modelInstance->nodesList);*/
}

void RM_List::remove(ModelInstance* modelInstance) {
	if (modelInstance->inRenderer_listEntry == nullptr) {
		throw LogicException(LOC);
	}

	throw LogicException(LOC);//asd_r_1a;// implement new method.
	/*dispatchRemoveNodePartsRecursive(modelInstance->nodesList);

	modelInstance->inRenderer_listEntry->remove();
	modelInstance->inRenderer_listEntry = nullptr;*/
}

void RM_List::add(Node* node) {
	throw LogicException(LOC);//asd_r_1a;// implement new method.
	/*if (node->inRenderer_listEntry != nullptr) {
		throw LogicException(LOC);
	}

	node->inRenderer_listEntry = nodesList.appendDirect(node);
	node->handlingRenderer = this;

	dispatchAddNodePartsRecursive(node);*/
}

void RM_List::remove(Node* node) {
	if (node->inRenderer_listEntry == nullptr) {
		throw LogicException(LOC);
	}

	throw LogicException(LOC);//asd_r;// implement when needed;
	/*dispatchRemoveNodePartsRecursive(node);

	node->inRenderer_listEntry->remove();
	node->inRenderer_listEntry = nullptr;*/
}

void RM_List::add(IRenderable* renderable) {
	if (renderable->getEntry_renderablesManagerRef() != nullptr) {
		throw LogicException(LOC);
	}

	ListDL<Entry_RenderablesManager>::Entry* newEntry = renderablesList.appendEmplace(renderable, this);
	newEntry->data.inRenderablesManager_listEntry = newEntry;
	renderable->getEntry_renderablesManagerRef() = &newEntry->data;

	dispatchAddRenderable(
		renderable,
		false/*usesInsertAfterMode*/, nullptr/*insertAfterNodePartTarget*/, newEntry->data/*entryRM*/
	);
}

void RM_List::insertAfter(IRenderable* renderable, IRenderable* insertAfterTarget) {
	if (renderable->getEntry_renderablesManagerRef() != nullptr) {
		throw LogicException(LOC);
	}

	ListDL<Entry_RenderablesManager>::Entry* newEntry = renderablesList.prependEmplace(renderable, this);
	newEntry->data.inRenderablesManager_listEntry = newEntry;
	renderable->getEntry_renderablesManagerRef() = &newEntry->data;

	dispatchAddRenderable(renderable, true/*usesInsertAfterMode*/, insertAfterTarget/*insertAfterNodePartTarget*/, newEntry->data/*entryRM*/);
}

void RM_List::remove(Entry_RenderablesManager* entry_renderablesManager) {
	if (entry_renderablesManager->inRenderablesManager_listEntry == nullptr || entry_renderablesManager->renderablesManager != this) {
		throw LogicException(LOC);
	}
	
	/// Remove the geometry managers entries.
	while (entry_renderablesManager->gManagerBucketEntriesList.count() > 0) {
		ListDL<Entry_GeometryManagerBucket*>::Entry* entry = entry_renderablesManager->gManagerBucketEntriesList.getFirst();
		Entry_GeometryManagerBucket* gManEntry = entry->data;

		gManEntry->remove();

		entry->remove();
	}

	/// Remove the local entry.
	entry_renderablesManager->renderable = nullptr;

	entry_renderablesManager->renderablesManager = nullptr;

	// The remove() call must be the last object access because it gets deleted during the remove() call.
	entry_renderablesManager->inRenderablesManager_listEntry->remove();
	//entry_renderablesManager->inRenderablesManager_listEntry = nullptr;
}

void RM_List::dispatchAddNodePartsRecursive(NodeInstance* nodeInstance) {
	throw LogicException(LOC);
	//asd_01;
	//for (int j = 0; j < nodeInstance->node->partsList.count(); j++) {
	//	INodePart* nodePart = nodeInstance->node->partsList.getDirect(j);

	//	// usesInsertAfterMode is not needed because the renderables are added in order.
	//	//asd_01;
	//	//handler->onRenderableAdded(
	//	//	nodeInstance, nodePart,
	//	//	false/*usesInsertAfterMode*/, nullptr/*insertAfterNodePartTarget*/
	//	//);
	//}
	//
	//dispatchAddNodePartsRecursive(nodeInstance->childrensList);
}

void RM_List::dispatchAddNodePartsRecursive(Node* node) {
	throw LogicException(LOC);
	//asd_01;
	//for (int j = 0; j < node->partsList.count(); j++) {
	//	INodePart* nodePart = node->partsList.getDirect(j);

	//	throw LogicException(LOC);
	//	//asd_01;
	//	//handler->onRenderableAdded(
	//	//	nullptr/*nodeInstance*/, nodePart,
	//	//	false/*usesInsertAfterMode*/, nullptr/*insertAfterNodePartTarget*/
	//	//);
	//}
	//
	// usesInsertAfterMode is not needed because the renderables are added in order.
	//dispatchAddNodePartsRecursive(node->childrensList);
}

void RM_List::dispatchAddRenderable(IRenderable* renderable, bool usesInsertAfterMode, IRenderable* insertAfterNodePartTarget, Entry_RenderablesManager& entryRM) {
	handler->getGeometryManager()->addRenderable(
		renderable, entryRM.gManagerBucketEntriesList,
		usesInsertAfterMode, insertAfterNodePartTarget
	);
	if (entryRM.gManagerBucketEntriesList.count() == 0) {
		// NodePart was not added to any render_pase.
		throw LogicException(LOC);
	}
}

void RM_List::dispatchAddNodePartsRecursive(ArrayList<NodeInstance*>& nodeInstancesList) {
	int ct = nodeInstancesList.count();
	for (int i = 0; i < ct; i++) {
		NodeInstance* nodeInstance = nodeInstancesList.getDirect(i);

		dispatchAddNodePartsRecursive(nodeInstance);
	}
}

void RM_List::dispatchAddNodePartsRecursive(ArrayList<Node*>& nodesList) {
	int ct = nodesList.count();
	for (int i = 0; i < ct; i++) {
		Node* node = nodesList.getDirect(i);

		dispatchAddNodePartsRecursive(node);
	}
}

void RM_List::syncModels() {
	int asd_01 = 0;
	//throw LogicException(LOC);//asd_r_1a;// implement new method.
	/*for (ListDL<ModelInstance*>::Entry* entry = modelInstancesList.getFirst(); entry != nullptr; entry = entry->next) {
		if (entry->data->invalidationBitMap != 0) {
			entry->data->sync(0);
		}
	}*/
}

RM_List::~RM_List() {
	if (renderablesList.count() > 0) {
		// Renderables must be removed before the renderer gets disposed.
		assert(false);
	}
}
