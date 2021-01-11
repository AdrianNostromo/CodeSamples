#include "CR_Base.h"
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/data/IndexBufferObject.h>
#include <graphics/geometry/data/UniformBufferObject.h>
#include <graphics/renderer/renderPhase/util/IRenderable.h>
#include <graphics/material/Material.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <graphics/model/INodePart.h>
#include <graphics/renderer/geometryManager/geometryProviders/instance/GeometryProviderInstance.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Indices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_ModelTransforms.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>
#include <graphics/geometry/uniformAttribute/UniformAttributesList.h>
#include <memory>
#include <base/list/CompositesBuffer.h>
#include <cassert>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/GeometryBufferManager.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/defragmenter/DefragmenterMigrating.h>

using namespace graphics;

CR_Base::CR_Base(
	std::shared_ptr<VertexAttributesList> vertexAttributesList,
	std::shared_ptr<graphics::Material> material,
	int maxVerticesCount, int maxIndicesCount, int maxWorldTransformsCount)
	: super(),
	vertexAttributesList(vertexAttributesList),
	material(material),
	maxVerticesCount(maxVerticesCount), maxIndicesCount(maxIndicesCount), maxWorldTransformsCount(maxWorldTransformsCount)
{
	//void
}

void CR_Base::create() {
	super::create();

	// These classes are used for gl data and are memcpy-ed directly. Make sure they don't contain extra stuff (eg. a vTable).
	if (sizeof(Matrix3) != 3 * 3 * 4 || sizeof(Matrix4) != 4 * 4 * 4) {
		assert(false);
	}

	{
		VertexBufferObject* bufferObject = new VertexBufferObject(
			vertexAttributesList,
			graphics::VertexBufferObject::UpdateMethod::IndividualRegions,
			maxVerticesCount/*initialCapacity*/, maxVerticesCount/*minCapacity*/);
		bufferObject->getBuffer().lockSize();
		bufferObject->reservedCreate();

		// trackEmptyFragmentsByCountSorting is true because the greedy_defrag uses it.
		mm_vertices = new GeometryBufferManager(
			this/*combinedRenderable*/,
			bufferObject, BlockMemoryManager::AllocationInsertionType::FirstAvailableSizeCheckedRegion/*allocationInsertionType*/, true/*trackEmptyFragmentsByCountSorting*/, 
			-1/*vaByteOffset_worldTransformIndex*/, nullptr/*bufferObjectB*/,
			1/*entriesPerGLEntry*/,
			true/*usesStableMemory*/, true/*usesRightToLeftVolatileMemory*/,
			new DefragmenterMigrating()/*defragmenter_volatile*/, new DefragmenterMigrating()/*defragmenter_stable*/
		);
	}

	{
		IndexBufferObject* bufferObject = new IndexBufferObject(
			graphics::IndexBufferObject::UpdateMethod::SingleRegion,
			maxIndicesCount/*initialCapacity*/, maxIndicesCount/*minCapacity*/
		);
		bufferObject->getBuffer().lockSize();
		bufferObject->reservedCreate();

		// Doesn't use trackEmptyFragmentsByCountSorting because allocation are in order and there is no defrag.
		mm_indices = new GeometryBufferManager(
			this/*combinedRenderable*/,
			bufferObject, BlockMemoryManager::AllocationInsertionType::FirstRegionMustHaveCapacity/*allocationInsertionType*/, false/*trackEmptyFragmentsByCountSorting*/,
			-1/*vaByteOffset_worldTransformIndex*/, nullptr/*bufferObjectB*/,
			1/*entriesPerGLEntry*/,
			false/*usesStableMemory*/, false/*usesRightToLeftVolatileMemory*/,
			nullptr/*defragmenter_volatile*/, nullptr/*defragmenter_stable*/
		);
	}

	if (vertexAttributesList->hasAll(VertexAttributesList::Type::ModelAndNormalTransformIndex->typeBitGroup)) {
		{

			std::shared_ptr<UniformAttributesList> uniformAttributesList = std::make_shared<UniformAttributesList>();
			uniformAttributesList->pushAttribute(UniformAttributesList::Type::modelTransform);
			uniformAttributesList->lock();

			UniformBufferObject* bufferObject = new UniformBufferObject(uniformAttributesList,
				graphics::UniformBufferObject::UpdateMethod::IndividualRegions,
				maxWorldTransformsCount/*initialCapacity*/, maxWorldTransformsCount/*minCapacity*/
			);
			bufferObject->getBuffer().lockSize();
			bufferObject->reservedCreate();

			int vaByteOffset_worldTransformIndex = vertexAttributesList->getByteOffset(VertexAttributesList::Type::ModelAndNormalTransformIndex);

			UniformBufferObject* bufferObject_modelNormalTransforms = nullptr;
			{
				//asd_01;// normal transforms may not be used, need some extra check for this. Use a VertexAttributesList::Type::ModelAndNormalTransform instead of ModelAndNormalTransformIndex.
				std::shared_ptr<UniformAttributesList> uniformAttributesList = std::make_shared<UniformAttributesList>();
				uniformAttributesList->pushAttribute(UniformAttributesList::Type::modelNormalTransform);
				uniformAttributesList->lock();

				bufferObject_modelNormalTransforms = new UniformBufferObject(
					uniformAttributesList,
					graphics::UniformBufferObject::UpdateMethod::IndividualRegions,
					maxWorldTransformsCount/*initialCapacity*/, maxWorldTransformsCount/*minCapacity*/
				);
				bufferObject_modelNormalTransforms->getBuffer().lockSize();
				bufferObject_modelNormalTransforms->reservedCreate();
			}

			// trackEmptyFragmentsByCountSorting is true because the greedy_defrag uses it.
			mm_modelTransforms = new GeometryBufferManager(
				this/*combinedRenderable*/,
				bufferObject, BlockMemoryManager::AllocationInsertionType::FirstAvailableSizeCheckedRegion/*allocationInsertionType*/, true/*trackEmptyFragmentsByCountSorting*/,
				vaByteOffset_worldTransformIndex/*vaByteOffset_worldTransformIndex*/, bufferObject_modelNormalTransforms/*bufferObjectB*/,
				1/*entriesPerGLEntry*/,
				true/*usesStableMemory*/, true/*usesRightToLeftVolatileMemory*/,
				nullptr/*defragmenter_volatile*/, new DefragmenterMigrating()/*defragmenter_stable*/
			);
		}

		{
			//asd_01;// normal transforms may not be used, need some extra check for this. Use a VertexAttributesList::Type::ModelAndNormalTransform instead of ModelAndNormalTransformIndex.
			int entriesPerGLEntry = 8;

			std::shared_ptr<UniformAttributesList> uniformAttributesList = std::make_shared<UniformAttributesList>();
			// Use a uvec4 instead of a uint array because all gl arrays have a array that is a multiple of 4_uints.
			uniformAttributesList->pushAttribute(UniformAttributesList::Type::uvec4);
			uniformAttributesList->lock();

			// Use a divide by entriesPerGLEntry vecase uvec4 are used and each vector component contains the information for 2 entries.
			int maxRemapingEntriesCount = Math::ceil((float)maxWorldTransformsCount / (float)entriesPerGLEntry);

			UniformBufferObject* bufferObject = new UniformBufferObject(
				uniformAttributesList,
				graphics::UniformBufferObject::UpdateMethod::SingleRegion,
				maxRemapingEntriesCount/*initialCapacity*/, maxRemapingEntriesCount/*minCapacity*/
			);
			bufferObject->getBuffer().lockSize();
			bufferObject->reservedCreate();

			// trackEmptyFragmentsByCountSorting is true because FirstAvailableSizeCheckedRegion requires it.
			mm_remapingModelTransformsIndices = new GeometryBufferManager(
				this/*combinedRenderable*/,
				bufferObject, BlockMemoryManager::AllocationInsertionType::FirstAvailableSizeCheckedRegion/*allocationInsertionType*/, true/*trackEmptyFragmentsByCountSorting*/,
				-1/*vaByteOffset_worldTransformIndex*/, nullptr/*bufferObjectB*/,
				entriesPerGLEntry/*entriesPerGLEntry*/,
				false/*usesStableMemory*/, false/*usesRightToLeftVolatileMemory*/,
				nullptr/*defragmenter_volatile*/, nullptr/*defragmenter_stable*/
			);
		}
	}
}

float CR_Base::getFillRatePercent() {
	return fillRatePercent;
}

std::shared_ptr<graphics::Material> CR_Base::getMaterial() {
	return material;
}

std::shared_ptr<VertexAttributesList> CR_Base::getVertexAttributesList() {
	return mm_vertices->getBufferObject_asVBO()->vertexAttributesList;
}

long CR_Base::getVertexAttributesBitMask() {
	return mm_vertices->getBufferObject_asVBO()->getVertexAttributesList()->getTypesBitMask();
}

void* CR_Base::getVerticesData() {
	return mm_vertices->data;
}

VertexBufferObject* CR_Base::getVerticesVBO() {
	return mm_vertices->getBufferObject_asVBO();
}

int CR_Base::getVerticesCount() {
	// This should not be needed. Implement when needed but that should never occur.
	throw LogicException(LOC);
}

void* CR_Base::getIndicesData() {
	return mm_indices->data;
}

IndexBufferObject* CR_Base::getIndicesIBO() {
	return mm_indices->getBufferObject_asIBO();
}

int CR_Base::getIndicesOffset() {
	return 0;
}

UniformBufferObject* CR_Base::getModelTransformsUBO() {
	if (mm_modelTransforms == nullptr) {
		return nullptr;
	}

	return mm_modelTransforms->getBufferObject_asUBO();
}

UniformBufferObject* CR_Base::getRemapingModelTransformsIndicesUBOOptional() {
	if (mm_remapingModelTransformsIndices == nullptr) {
		return nullptr;
	}

	return mm_remapingModelTransformsIndices->getBufferObject_asUBO();
}

UniformBufferObject* CR_Base::getModelNormalTransformsUBO() {
	if (mm_modelTransforms == nullptr) {
		return nullptr;
	}

	// This can return nullptr if not existent.
	return mm_modelTransforms->getBufferObjectB_asUBO();
}

Matrix4* CR_Base::getWorldTransformOptional() {
	return nullptr;
}

Matrix4* CR_Base::getWorldTransformMustExist() {
	throw LogicException(LOC);
}

Matrix4* CR_Base::getNormalWorldTransformOptional() {
	return nullptr;
}

Matrix4* CR_Base::getNormalWorldTransformMustExist() {
	throw LogicException(LOC);
}

Entry_RenderablesManager*& CR_Base::getEntry_renderablesManagerRef() {
	// This is not used by CR_Base.
	throw LogicException(LOC);
}

IGeometryProvider_Vertices* CR_Base::getGeometryProvider_Vertices() {
	return this;
}

IGeometryProvider_Indices* CR_Base::getGeometryProvider_Indices() {
	return this;
}

IGeometryProvider_ModelTransforms* CR_Base::getGeometryProvider_ModelTransforms() {
	return this;
}

void CR_Base::onMemManFillRateCHanged() {
	fillRatePercent = mm_vertices->fillRatePercent >= mm_indices->fillRatePercent ? mm_vertices->fillRatePercent : mm_indices->fillRatePercent;
	if (mm_modelTransforms != nullptr && mm_modelTransforms->fillRatePercent > fillRatePercent) {
		fillRatePercent = mm_modelTransforms->fillRatePercent;
	}
}

void CR_Base::tickDefrag() {
	mm_vertices->tickDefrag();
	mm_indices->tickDefrag();
	if (mm_modelTransforms != nullptr) {
		mm_modelTransforms->tickDefrag();
	}
	if (mm_remapingModelTransformsIndices != nullptr) {
		mm_remapingModelTransformsIndices->tickDefrag();
	}
}

void CR_Base::dispose() {
	vertexAttributesList = nullptr;
	material = nullptr;

	if(mm_vertices != nullptr) {
		delete mm_vertices;
		mm_vertices = nullptr;
	}
	if (mm_indices != nullptr) {
		delete mm_indices;
		mm_indices = nullptr;
	}
	if (mm_modelTransforms != nullptr) {
		delete mm_modelTransforms;
		mm_modelTransforms = nullptr;
	}
	if (mm_remapingModelTransformsIndices != nullptr) {
		delete mm_remapingModelTransformsIndices;
		mm_remapingModelTransformsIndices = nullptr;
	}

	super::dispose();
}

CR_Base::~CR_Base() {
	//void
}
