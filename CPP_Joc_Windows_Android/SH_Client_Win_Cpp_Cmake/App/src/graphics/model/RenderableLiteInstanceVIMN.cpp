#include "RenderableLiteInstanceVIMN.h"
#include <graphics/model/instance/NodeInstance.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>
#include <graphics/renderer/renderablesManager/util/Entry_RenderablesManager.h>

using namespace graphics;

RenderableLiteInstanceVIMN::RenderableLiteInstanceVIMN()
	: super(),
	vertexAttributesList(nullptr),
	verticesUnmanaged(nullptr), vertices_count(0),
	indicesUnmanaged(nullptr), indices_count(0),
	material(nullptr)
{
	//void
}

RenderableLiteInstanceVIMN::RenderableLiteInstanceVIMN(
	std::shared_ptr<VertexAttributesList> vertexAttributesList,
	void* vertices, unsigned int vertices_count,
	unsigned short* indices, unsigned int indices_count,
	std::shared_ptr<Material> material)
	: super(),
	verticesUnmanaged(vertices), vertices_count(vertices_count),
	indicesUnmanaged(indices), indices_count(indices_count),
	material(material)
{
	initVertexAttributesList(vertexAttributesList);
}

void* RenderableLiteInstanceVIMN::getVerticesData() {
	return verticesUnmanaged;
}

VertexBufferObject* RenderableLiteInstanceVIMN::getVerticesVBO() {
	// This renderable can be used only after it is combined. There is no vbo.
	throw LogicException(LOC);
}

int RenderableLiteInstanceVIMN::getVerticesCount() {
	return vertices_count;
}

void* RenderableLiteInstanceVIMN::getIndicesData() {
	return indicesUnmanaged;
}

IndexBufferObject* RenderableLiteInstanceVIMN::getIndicesIBO() {
	// This renderable can be used only after it is combined. There is no vbo.
	throw LogicException(LOC);
}

int RenderableLiteInstanceVIMN::getIndicesOffset() {
	return 0;
}

int RenderableLiteInstanceVIMN::getIndicesCount() {
	return indices_count;
}

UniformBufferObject* RenderableLiteInstanceVIMN::getModelTransformsUBO() {
	return nullptr;
}

UniformBufferObject* RenderableLiteInstanceVIMN::getRemapingModelTransformsIndicesUBOOptional() {
	return nullptr;
}

UniformBufferObject* RenderableLiteInstanceVIMN::getModelNormalTransformsUBO() {
	return nullptr;
}

Matrix4* RenderableLiteInstanceVIMN::getWorldTransformOptional() {
	return worldTransform;
}

Matrix4* RenderableLiteInstanceVIMN::getWorldTransformMustExist() {
	if (worldTransform == nullptr) {
		throw LogicException(LOC);
	}

	return worldTransform;
}

Matrix4* RenderableLiteInstanceVIMN::getNormalWorldTransformOptional() {
	return normalWorldTransform;
}

Matrix4* RenderableLiteInstanceVIMN::getNormalWorldTransformMustExist() {
	if (normalWorldTransform == nullptr) {
		throw LogicException(LOC);
	}

	return normalWorldTransform;
}

void RenderableLiteInstanceVIMN::invalidate(
	int invalidationBitMask_Vertices, int invalidationBitMask_Indices,
	int invalidationBitMask_ModelTransforms, int invalidationBitMask_ModelNormalTransforms)
{
	if (invalidationBitMask_Vertices != 0) {
		IGeometryProvider_Vertices::invalidate(invalidationBitMask_Vertices);
	}
	if (invalidationBitMask_Indices != 0) {
		IGeometryProvider_Indices::invalidate(invalidationBitMask_Indices);
	}
	if (invalidationBitMask_ModelTransforms != 0) {
		IGeometryProvider_ModelTransforms::invalidate(invalidationBitMask_ModelTransforms);
	}
}

std::shared_ptr<VertexAttributesList> RenderableLiteInstanceVIMN::getVertexAttributesList() {
	return vertexAttributesList;
}

void RenderableLiteInstanceVIMN::initVertexAttributesList(std::shared_ptr<VertexAttributesList> vertexAttributesList) {
	if (this->vertexAttributesList != nullptr || vertexAttributesList == nullptr) {
		throw LogicException(LOC);
	}

	this->vertexAttributesList = vertexAttributesList;

	if (this->vertexAttributesList->hasAll(VertexAttributesList::Type::ModelAndNormalTransformIndex->typeBitGroup)) {
		// Currently only 1 wTransform may exist per renderable, if needed, an array can be used and a char[vertices_count] that contains the local_wTransform for each vertex.
		worldTransform = newt Matrix4();
		worldTransform->idt();

		normalWorldTransform = newt Matrix4();
		normalWorldTransform->idt();
	}
}

long RenderableLiteInstanceVIMN::getVertexAttributesBitMask() {
	return vertexAttributesList->getTypesBitMask();
}

std::shared_ptr<graphics::Material> RenderableLiteInstanceVIMN::getMaterial() {
	return material;
}

void RenderableLiteInstanceVIMN::setMaterial(std::shared_ptr<Material> material) {
	this->material = material;
}

void RenderableLiteInstanceVIMN::removeFromRenderer() {
	if (entry_renderablesManager == nullptr) {
		throw LogicException(LOC);
	}

	entry_renderablesManager->remove();
	entry_renderablesManager = nullptr;
}

Entry_RenderablesManager*& RenderableLiteInstanceVIMN::getEntry_renderablesManagerRef() {
	return entry_renderablesManager;
}

IGeometryProvider_Vertices* RenderableLiteInstanceVIMN::getGeometryProvider_Vertices() {
	return this;
}

IGeometryProvider_Indices* RenderableLiteInstanceVIMN::getGeometryProvider_Indices() {
	return this;
}

IGeometryProvider_ModelTransforms* RenderableLiteInstanceVIMN::getGeometryProvider_ModelTransforms() {
	return this;
}

void RenderableLiteInstanceVIMN::dispose() {
	if (worldTransform != nullptr) {
		delete worldTransform;
		worldTransform = nullptr;
	}
	if (normalWorldTransform != nullptr) {
		delete normalWorldTransform;
		normalWorldTransform = nullptr;
	}

	super::dispose();
}

RenderableLiteInstanceVIMN::~RenderableLiteInstanceVIMN() {
	//void
}
