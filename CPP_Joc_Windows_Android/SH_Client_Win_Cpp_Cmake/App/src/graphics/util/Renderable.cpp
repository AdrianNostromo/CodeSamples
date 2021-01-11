#include "Renderable.h"
#include <graphics/geometry/data/MeshPart.h>
#include <graphics/material/Material.h>
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>

using namespace graphics;

Renderable::Renderable(Mesh* mesh, Material* material)
	: mesh(mesh), material(material)
{
	maDiffuseTexturesArray = material->getOptionalAs<MADiffuseTexturesArray*>(MADiffuseTexturesArray::TypeBitGroup);
}

Renderable::~Renderable() {
	if (material != nullptr) {
		material->reservedDispose();
		delete material;
		material = nullptr;
	}
}
