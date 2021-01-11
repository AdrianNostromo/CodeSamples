#include "Mesh.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/data/IndexBufferObject.h>

using namespace graphics;

Mesh::Mesh(VertexBufferObject* vertexData, IndexBufferObject* indexData)
	: super(),
	vertexData(vertexData), indexData(indexData)
{
	//void
}

//asd_r;// use or remove this; the new system doesn't use it.
void Mesh::render(ShaderProgram* shader) {
	int finalIndicesToRender = indexData->getActiveIndicesCount() >= 0 ? indexData->getActiveIndicesCount() : indexData->getBuffer().count();
	if (finalIndicesToRender > 0) {
		CallGLChecked(GLUtil::gl->drawElements(
			IGL::TRIANGLES,
			finalIndicesToRender, IGL::UNSIGNED_SHORT, nullptr
		));// nullptr because the ibo is already bound.
	}
}

void Mesh::bind(ShaderProgram* shader) {
	vertexData->bind(shader);
	indexData->bind();
}

void Mesh::unbind() {
	vertexData->unbind();
	indexData->unbind();
}

void Mesh::dispose() {
	if (vertexData != nullptr) {
		vertexData->reservedDisposeMain();
		delete vertexData;

		vertexData = nullptr;
	}

	if (indexData != nullptr) {
		indexData->reservedDisposeMain();
		delete indexData;

		indexData = nullptr;
	}

	super::dispose();
}

Mesh::~Mesh() {
	//void
}
