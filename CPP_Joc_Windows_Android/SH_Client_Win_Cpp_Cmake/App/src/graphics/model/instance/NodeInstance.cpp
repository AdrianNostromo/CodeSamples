#include "NodeInstance.h"
#include <base/math/util/BoolUtil.h>
#include <graphics/model/instance/ModelInstance.h>
#include <graphics/model/Node.h>

using namespace graphics;

const unsigned int NodeInstance::InvalidationFlags::anyChildRequest = BoolUtil::setBitAtIndexDirect(0, 0);

const unsigned int NodeInstance::InvalidationFlags::localTransform = BoolUtil::setBitAtIndexDirect(0, 1);
const unsigned int NodeInstance::InvalidationFlags::globalTransform = BoolUtil::setBitAtIndexDirect(0, 2);

const unsigned int NodeInstance::InvalidationFlags::syncAllChildrenMask = globalTransform;

NodeInstance::NodeInstance(
	Node* node, 
	ModelInstance* modelInstance, NodeInstance* parent)
	: super(),
	node(node),
	modelInstance(modelInstance), parent(parent)
{
	//void
}

void NodeInstance::invalidate(unsigned int invalidationBitMap) {
	if (this->invalidationBitMap == 0) {
		if (parent != nullptr) {
			parent->invalidate(InvalidationFlags::anyChildRequest);
		} else {
			// The parent is the ModelInstance.
			modelInstance->invalidate(InvalidationFlags::anyChildRequest);
		}
	}

	this->invalidationBitMap |= invalidationBitMap;
}

void NodeInstance::sync(unsigned int upperInvalidationBitMap) {
	if (invalidationBitMap & InvalidationFlags::localTransform) {
		// The local transform is in the Node and is already update.
		// This is caused by the Node invalidation and is only needed to update the global transform.

		invalidationBitMap |= InvalidationFlags::globalTransform;
	}
	if ((invalidationBitMap | upperInvalidationBitMap) & InvalidationFlags::globalTransform) {
		// Update global transform.
		if (parent != nullptr) {
			globalTransform.set(parent->globalTransform);
		}else if(modelInstance != nullptr) {
			globalTransform.set(modelInstance->globalTransform);
		} else {
			globalTransform.idt();
		}
		globalTransform.mul(node->localTransform);

		globalNormalTransform.set(*tmpMat4.set(globalTransform)->invUnchecked()->transpose());
	}

	bool syncAllChildren = (invalidationBitMap | upperInvalidationBitMap) & InvalidationFlags::syncAllChildrenMask;
	if (syncAllChildren || invalidationBitMap & InvalidationFlags::anyChildRequest) {
		unsigned int combinedInvalidationBitMap = upperInvalidationBitMap | invalidationBitMap;

		for (int i = 0; i < childrensList.count(); i++) {
			NodeInstance* nodeInstance = childrensList.getDirect(i);

			if (syncAllChildren || nodeInstance->invalidationBitMap != 0) {
				nodeInstance->sync(combinedInvalidationBitMap);
			}
		}
	}

	invalidationBitMap = 0;
}

NodeInstance::~NodeInstance() {
	//void
}
