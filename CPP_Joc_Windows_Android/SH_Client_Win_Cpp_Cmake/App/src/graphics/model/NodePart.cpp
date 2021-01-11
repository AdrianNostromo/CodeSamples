#include "NodePart.h"
#include <graphics/geometry/data/MeshPart.h>
#include <graphics/geometry/Mesh.h>
#include <graphics/material/Material.h>
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/data/IndexBufferObject.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/model/instance/NodeInstance.h>
#include <graphics/model/Node.h>

using namespace graphics;

NodePart::NodePart(MeshPart* meshPart, std::shared_ptr<Material> material)
	: super(),
	meshPart(meshPart), material(material)
{
	//void
}

Node* NodePart::getNode() {
	return node;
}

MeshPart* NodePart::getMeshPartOptional() {
	return meshPart;
}

void* NodePart::getVerticesData() {
	return meshPart->mesh->vertexData->getBuffer().getData(meshPart->verticesOffset, meshPart->verticesCount);
}

VertexBufferObject* NodePart::getVerticesVBO() {
	return meshPart->mesh->vertexData;
}

int NodePart::getVerticesCount() {
	return meshPart->verticesCount;
}

void* NodePart::getIndicesData() {
	return meshPart->mesh->indexData->getBuffer().getData(meshPart->indicesOffset, meshPart->indicesCount);
}

IndexBufferObject* NodePart::getIndicesIBO() {
	return meshPart->mesh->indexData;
}

int NodePart::getIndicesOffset() {
	return meshPart->indicesOffset;
}

int NodePart::getIndicesCount() {
	return meshPart->indicesCount;
}

Matrix4* NodePart::getWorldTransformMustExist() {
	// asd. Implement when needed.
	throw LogicException(LOC);
}

Matrix4* NodePart::getNormalWorldTransformMustExist() {
	// For NodePart, the transform is in the 
	throw LogicException(LOC);
}

void NodePart::invalidate(int invalidationBitGroups) {
	throw LogicException(LOC);
	/*asd_01;
	for (ListDL<NodePartInstance>::Entry* entry = instancesList.getFirst(); entry != nullptr; entry = entry->next) {
		entry->data.invalidate(invalidationBitGroups);
	}*/
}

std::shared_ptr<VertexAttributesList> NodePart::getVertexAttributesList() {
	return meshPart->mesh->getVertexData()->getVertexAttributesList();
}

long NodePart::getVertexAttributesBitMask() {
	return meshPart->mesh->getVertexData()->getVertexAttributesList()->getTypesBitMask();
}

std::shared_ptr<graphics::Material> NodePart::getMaterial() {
	return material;
}

void NodePart::setMaterial(std::shared_ptr<Material> material) {
	this->material = material;
}

ListDL<INodePart*>::Entry*& NodePart::getInRenderer_listEntryRef() {
	return inRenderer_listEntry;
}

void NodePart::dispose() {
	//void

	super::dispose();
}

NodePart::~NodePart() {
	//void
}
