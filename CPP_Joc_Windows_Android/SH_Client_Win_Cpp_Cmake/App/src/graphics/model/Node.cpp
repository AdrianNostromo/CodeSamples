#include "Node.h"
#include "NodePart.h"
#include "Model.h"
#include <graphics/model/instance/NodeInstance.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>
#include <base/list/ListDL.h>

using namespace graphics;

Node::Node(bool computeNormalTransform)
	: super()
{
	if (computeNormalTransform) {
		localNormalTransform = newt Matrix3();
	} else {
		localNormalTransform = nullptr;
	}
}

NodePart* Node::appendNodePart(NodePart* nodePart) {
	nodePart->node = this;

	partsList.appendDirect(nodePart);

	return nodePart;
}

NodeInstance* Node::createInstance(ModelInstance* modelInstance, NodeInstance* parentNodeInstance) {
	ListDL<NodeInstance>::Entry* listEntry = instancesList.appendEmplace(this, modelInstance, parentNodeInstance);
	NodeInstance* nodeInstance = &listEntry->data;
	nodeInstance->inNode_listEntry = listEntry;

	for (int i = 0; i < childrensList.count(); i++) {
		Node* node = childrensList.getDirect(i);

		NodeInstance* newNodeInstance = node->createInstance(modelInstance, nodeInstance);

		nodeInstance->childrensList.appendDirect(newNodeInstance);
	}

	return nodeInstance;
}

void Node::deleteInstance(NodeInstance* nodeInstance) {
	if (nodeInstance->inNode_listEntry == nullptr) {
		throw LogicException(LOC);
	}

	nodeInstance->inNode_listEntry->remove();
	nodeInstance->inNode_listEntry = nullptr;

	nodeInstance->reservedDispose();
	delete nodeInstance;
}

void Node::setTranslation(float x, float y, float z) {
	if (translation.equals(x, y, z)) {
		return;
	}

	translation.set(x, y, z);

	//asd_r_1;// use a invalidation also and the NodeInstance will validate on first sync or the Renderer will validate if fragment mode.
	localTransform.set(translation, rotation, scale);
	if (localNormalTransform != nullptr) {
		localNormalTransform->set(*tmpMat4.set(localTransform)->invUnchecked()->transpose());
	}

	invalidate(NodeInstance::InvalidationFlags::localTransform);
}

void Node::setRotation(Quaternion& rotation) {
	if (this->rotation.equals(rotation)) {
		return;
	}

	this->rotation.set(rotation);

	localTransform.set(translation, rotation, scale);
	if (localNormalTransform != nullptr) {
		localNormalTransform->set(*tmpMat4.set(localTransform)->invUnchecked()->transpose());
	}

	invalidate(NodeInstance::InvalidationFlags::localTransform);
}

void Node::setScale(float scaleX, float scaleY, float scaleZ) {
	if (scale.equals(scaleX, scaleY, scaleZ)) {
		return;
	}

	scale.set(scaleX, scaleY, scaleZ);

	localTransform.set(translation, rotation, scale);
	if (localNormalTransform != nullptr) {
		localNormalTransform->set(*tmpMat4.set(localTransform)->invUnchecked()->transpose());
	}

	invalidate(NodeInstance::InvalidationFlags::localTransform);
}

void Node::invalidate(unsigned int invalidationBitMap) {
	// Pass to the instances so they get updated.
	for (ListDL<NodeInstance>::Entry* entry = instancesList.getFirst(); entry != nullptr; entry = entry->next) {
		entry->data.invalidate(invalidationBitMap);
	}
}

void Node::dispose() {
	if (instancesList.count() > 0) {
		// Model instances still use this Model resource.
		throw LogicException(LOC);
	}

	if (childrensList.count() > 0) {
		for (int i = 0; i < childrensList.count(); i++) {
			Node* node = childrensList.getDirect(i);

			node->reservedDisposeMain();
			delete node;
		}

		partsList.clear();
	}
	if (partsList.count() > 0) {
		for (int i = 0; i < partsList.count(); i++) {
			NodePart* nodePart = partsList.getDirect(i);

			nodePart->reservedDisposeMain();
			delete nodePart;
		}

		partsList.clear();
	}

	super::dispose();
}

Node::~Node() {
	//void
}
