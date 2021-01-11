#include "RenderableLiteInstanceVI.h"
#include <graphics/model/instance/NodeInstance.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>
#include <graphics/renderer/renderablesManager/util/Entry_RenderablesManager.h>

using namespace graphics;

RenderableLiteInstanceVI::RenderableLiteInstanceVI()
	: super(),
	vertexAttributesList(nullptr),
	verticesUnmanaged(nullptr), vertices_count(0),
	indicesUnmanaged(nullptr), indices_count(0),
	material(nullptr)
{
	//void
}

RenderableLiteInstanceVI::RenderableLiteInstanceVI(
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

void* RenderableLiteInstanceVI::getVerticesData() {
	return verticesUnmanaged;
}

VertexBufferObject* RenderableLiteInstanceVI::getVerticesVBO() {
	// This renderable can be used only after it is combined. There is no vbo.
	throw LogicException(LOC);
}

int RenderableLiteInstanceVI::getVerticesCount() {
	return vertices_count;
}

void* RenderableLiteInstanceVI::getIndicesData() {
	return indicesUnmanaged;
}

IndexBufferObject* RenderableLiteInstanceVI::getIndicesIBO() {
	// This renderable can be used only after it is combined. There is no vbo.
	throw LogicException(LOC);
}

int RenderableLiteInstanceVI::getIndicesOffset() {
	return 0;
}

int RenderableLiteInstanceVI::getIndicesCount() {
	return indices_count;
}

void RenderableLiteInstanceVI::invalidate(
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
		throw LogicException(LOC);
	}
	if (invalidationBitMask_ModelNormalTransforms != 0) {
		throw LogicException(LOC);
	}
}

std::shared_ptr<VertexAttributesList> RenderableLiteInstanceVI::getVertexAttributesList() {
	return vertexAttributesList;
}

void RenderableLiteInstanceVI::initVertexAttributesList(std::shared_ptr<VertexAttributesList> vertexAttributesList) {
	if (this->vertexAttributesList != nullptr || vertexAttributesList == nullptr) {
		throw LogicException(LOC);
	}

	this->vertexAttributesList = vertexAttributesList;
}

long RenderableLiteInstanceVI::getVertexAttributesBitMask() {
	return vertexAttributesList->getTypesBitMask();
}

std::shared_ptr<graphics::Material> RenderableLiteInstanceVI::getMaterial() {
	return material;
}

void RenderableLiteInstanceVI::setMaterial(std::shared_ptr<Material> material) {
	this->material = material;
}

void RenderableLiteInstanceVI::removeFromRenderer() {
	if (entry_renderablesManager == nullptr) {
		throw LogicException(LOC);
	}

	entry_renderablesManager->remove();
	entry_renderablesManager = nullptr;
}

Entry_RenderablesManager*& RenderableLiteInstanceVI::getEntry_renderablesManagerRef() {
	return entry_renderablesManager;
}

IGeometryProvider_Vertices* RenderableLiteInstanceVI::getGeometryProvider_Vertices() {
	return this;
}

IGeometryProvider_Indices* RenderableLiteInstanceVI::getGeometryProvider_Indices() {
	return this;
}

IGeometryProvider_ModelTransforms* RenderableLiteInstanceVI::getGeometryProvider_ModelTransforms() {
	return nullptr;
}

void RenderableLiteInstanceVI::dispose() {
	//void

	super::dispose();
}

RenderableLiteInstanceVI::~RenderableLiteInstanceVI() {
	//void
}
