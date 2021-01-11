#include "ModelInstance.h"
#include <graphics/model/Model.h>
#include <graphics/model/NodePart.h>
#include <graphics/model/instance/NodeInstance.h>
#include <base/exceptions/LogicException.h>

using namespace graphics;

ModelInstance::ModelInstance(Model* model)
	: super(),
	model(model)
{
	//void
}

Matrix4& ModelInstance::peekGlobalTransform() {
	return globalTransform;
}

void ModelInstance::invalidate(unsigned int invalidationBitMap) {
	this->invalidationBitMap |= invalidationBitMap;
}

void ModelInstance::sync(unsigned int upperInvalidationBitMap) {
	if (invalidationBitMap & NodeInstance::InvalidationFlags::localTransform) {
		// Update local transform.
		localTransform.set(translation, rotation, scale);

		invalidationBitMap |= NodeInstance::InvalidationFlags::globalTransform;
	}
	if ((invalidationBitMap | upperInvalidationBitMap) & NodeInstance::InvalidationFlags::globalTransform) {
		// Update global transform.
		globalTransform.set(localTransform);
	}

	bool syncAllChildren = (invalidationBitMap | upperInvalidationBitMap) & NodeInstance::InvalidationFlags::syncAllChildrenMask;
	if (syncAllChildren || invalidationBitMap & NodeInstance::InvalidationFlags::anyChildRequest) {
		unsigned int combinedInvalidationBitMap = upperInvalidationBitMap | invalidationBitMap;

		for (int i = 0; i < nodesList.count(); i++) {
			NodeInstance* nodeInstance = nodesList.getDirect(i);
			
			if (syncAllChildren || nodeInstance->invalidationBitMap != 0) {
				nodeInstance->sync(combinedInvalidationBitMap);
			}
		}
	}

	invalidationBitMap = 0;
}

void ModelInstance::setTranslation(float x, float y, float z) {
	if (translation.equals(x, y, z)) {
		return;
	}

	translation.set(x, y, z);
	
	invalidate(NodeInstance::InvalidationFlags::localTransform);
}

void ModelInstance::setRotation(Quaternion& rotation) {
	if (this->rotation.equals(rotation)) {
		return;
	}

	this->rotation.set(rotation);

	invalidate(NodeInstance::InvalidationFlags::localTransform);
}

void ModelInstance::setScale(float scaleX, float scaleY, float scaleZ) {
	if (scale.equals(scaleX, scaleY, scaleZ)) {
		return;
	}

	scale.set(scaleX, scaleY, scaleZ);

	invalidate(NodeInstance::InvalidationFlags::localTransform);
}

void ModelInstance::dispose() {
	if (inRenderer_listEntry != nullptr/*still_in_a_renderer*/ || inModel_listEntry/*still_connected_in_a_Model*/ != nullptr) {
		throw LogicException(LOC);
	}

	super::dispose();
}

ModelInstance::~ModelInstance() {
	//void
}
