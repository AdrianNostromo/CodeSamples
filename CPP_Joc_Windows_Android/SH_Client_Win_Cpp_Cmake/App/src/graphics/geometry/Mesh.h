#pragma once

#include <base/gh.h>
#include <base/object/Object.h>

namespace graphics {
	class VertexBufferObject;
	class IndexBufferObject;
	class ShaderProgram;
};

namespace graphics {
class Mesh : public base::Object {priv typedef base::Object super;pub dCtor(Mesh);
	pub VertexBufferObject* vertexData = nullptr;
	pub IndexBufferObject* indexData = nullptr;

	pub explicit Mesh(VertexBufferObject* vertexData, IndexBufferObject* indexData);

	pub VertexBufferObject* getVertexData() { return vertexData; };
	pub IndexBufferObject* getIndexData() { return indexData; }

	pub void render(ShaderProgram* shader);

	pub void bind(ShaderProgram* shader);
	pub void unbind();

	pub void dispose() override;
	pub ~Mesh() override;
};
};
