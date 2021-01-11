#include "Model.h"
#include <graphics/model/Node.h>
#include <graphics/geometry/Mesh.h>
#include <base/exceptions/LogicException.h>

using namespace graphics;

Model::Model()
	: super()
{
	//void
}

Node* Model::appendNode(Node* node) {
	node->model = this;

	nodesList.appendDirect(node);

	return node;
}

Mesh* Model::appendMesh(Mesh* mesh) {
	meshesList.appendDirect(mesh);

	return mesh;
}

ModelInstance* Model::createInstance() {
	ListDL<ModelInstance>::Entry* listEntry = instancesList.appendEmplace(this);
	ModelInstance* modelInstance = &listEntry->data;
	modelInstance->inModel_listEntry = listEntry;

	// Create a duplicate of the Node-s structure (With no NodeParts, those are handled by the RPhase-s).
	for (int i = 0; i < nodesList.count(); i++) {
		Node* node = nodesList.getDirect(i);

		NodeInstance* newNodeInstance = node->createInstance(modelInstance, nullptr/*parentNode*/);

		modelInstance->nodesList.appendDirect(newNodeInstance);
	}

	return modelInstance;
}

void Model::deleteInstance(ModelInstance* modelInstance) {
	if (modelInstance->inModel_listEntry == nullptr) {
		throw LogicException(LOC);
	}

	modelInstance->inModel_listEntry->remove();
	modelInstance->inModel_listEntry = nullptr;

	modelInstance->reservedDispose();
	delete modelInstance;
}

void Model::dispose() {
	if (instancesList.count() > 0) {
		// Model instances still use this Model resource.
		throw LogicException(LOC);
	}

	if (nodesList.count() > 0) {
		for (int i = 0; i < nodesList.count(); i++) {
			Node* node = nodesList.getDirect(i);

			node->reservedDisposeMain();
			delete node;
		}

		nodesList.clear();
	}
	if (meshesList.count() > 0) {
		for (int i = 0; i < meshesList.count(); i++) {
			Mesh* mesh = meshesList.getDirect(i);

			mesh->reservedDisposeMain();
			delete mesh;
		}

		meshesList.clear();
	}

	super::dispose();
}

Model::~Model() {
	//void
}
