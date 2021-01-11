#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>

class Texture;

namespace graphics {
	class Mesh;
	class MeshPart;
	class Material;
	class MADiffuseTexturesArray;
};

//asd_r;// remove this class when new system implemented.
namespace graphics {
class Renderable {pub dCtor(Renderable);
	pub Mesh* mesh;

	pub Material* material;
	pub MADiffuseTexturesArray* maDiffuseTexturesArray;

	pub MeshPart* meshPart;

    pub explicit Renderable(Mesh* mesh, Material* material);

    pub virtual ~Renderable();
};
};
