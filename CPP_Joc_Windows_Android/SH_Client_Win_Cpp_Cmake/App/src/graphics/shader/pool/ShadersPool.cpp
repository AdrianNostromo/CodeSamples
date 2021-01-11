#include "ShadersPool.h"
#include <assert.h>
#include <graphics/util/Renderable.h>
#include <graphics/model/NodePart.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <graphics/geometry/data/MeshPart.h>
#include <graphics/geometry/Mesh.h>
#include <graphics/geometry/data/VertexBufferObject.h>

using namespace graphics;

ShadersPool::ShadersPool()
	: super()
{
	//shadersList.reserve(4);
}

IShader* ShadersPool::getOrCreateShader(
	IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList)
{
	for (int i = 0; i < shadersList.size(); i++) {
		IShader* localShader = *shadersList.getPointer(i);

		bool canRender = localShader->canRender(renderable, environment, rendererAttributesList);
		if (canRender) {
			return localShader;
		}
	}

	IShader* localShader = createNewShader(renderable, environment, rendererAttributesList);

	shadersList.appendReference(localShader);

	return localShader;
}

void ShadersPool::dispose() {
	for (int i = 0; i < shadersList.size(); i++) {
		IShader* localShader = shadersList.removeAndGetDirect(i);

		localShader->reservedDisposeMain();
		delete localShader;
	}

	super::dispose();
}

ShadersPool::~ShadersPool() {
	//void
}
