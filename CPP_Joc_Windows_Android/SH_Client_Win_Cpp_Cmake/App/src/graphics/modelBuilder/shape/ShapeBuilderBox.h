#pragma once

#include <base/gh.h>
#include <graphics/util/GLTypes.h>
#include <base/math/Vector3.h>

class Color;
namespace graphics {
	class VertexBufferObject;
	class IndexBufferObject;
};

namespace graphics {
class ShapeBuilderBox {
	pub static void DrawCube(
		graphics::VertexBufferObject* vbo, graphics::IndexBufferObject* ibo,
		float sizeX, float sizeY, float sizeZ, Color& vertexAttributeColor,
		Vector3* vertexTranslation=nullptr);
	
	priv static void DrawQuad(
		graphics::VertexBufferObject* vbo, graphics::IndexBufferObject* ibo,
		graphics::GLTypes::Vertex_3DGenericColor v0, graphics::GLTypes::Vertex_3DGenericColor v1, graphics::GLTypes::Vertex_3DGenericColor v2, graphics::GLTypes::Vertex_3DGenericColor v3);

};
};
