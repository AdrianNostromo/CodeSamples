#pragma once

#include <base/gh.h>
#include <graphics/util/GraphicsUtil.h>
#include <memory>
#include <base/math/Vector3.h>

class Color;
namespace graphics {
	class Model;
	class VertexBufferObject;
	class IndexBufferObject;
	class Material;
	class Node;
};

namespace graphics {
class ModelBuilder {
	// Attach to targetModel if exists.
	// Note. If a standalong Node if created, it will have the computeNormalTransform set.
	pub static Node* CreateCubeNode(
		float size, 
		std::shared_ptr<graphics::Material> material0, Color& vertexAttributeColor,
		Model* targetModel=nullptr,
		Vector3* vertexTranslation =nullptr);
	pub static Node* CreateBoxNode(
		float sizeX, float sizeY, float sizeZ,
		std::shared_ptr<graphics::Material> material0, Color& vertexAttributeColor,
		Model* targetModel = nullptr,
		Vector3* vertexTranslation=nullptr);

};
};
