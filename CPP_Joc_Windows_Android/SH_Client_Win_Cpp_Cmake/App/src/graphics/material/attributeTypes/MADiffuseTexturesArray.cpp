#include "MADiffuseTexturesArray.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/visual/TextureArray.h>
#include <base/MM.h>

using namespace graphics;

std::string MADiffuseTexturesArray::Alias = "diffuseTexturesArray";
long MADiffuseTexturesArray::TypeBitGroup = registerAttribute(Alias);

std::string MADiffuseTexturesArray::u_textureArrayArray = "u_textureArrayArray";

MADiffuseTexturesArray::MADiffuseTexturesArray()
	: super(TypeBitGroup)
{
	rawSubclassPointer = this;
}

void MADiffuseTexturesArray::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	super::bind(attributesList, shaderProgram);

	if (globalTextures == nullptr) {
		int asd_01 = 0;
	}
	if (globalTextures != nullptr) {
		for (int i = 0; i < globalTextures->count(); i++) {
			graphics::TextureArray* texturesArray = globalTextures->getDirect(i);

			if (texturesArray->getHardcodedTextureBindSlot() < 0) {
				// Tex_array without a hardoced bind slot cannot be used here currently because there is no method to generate a slot.
				throw LogicException(LOC);
			}
			texturesArray->bind(texturesArray->getHardcodedTextureBindSlot());

			// This bind method can be used but need to get the location for "u_textureArrayArray[0]", "u_textureArrayArray[1]", ...;
			// An alternative is below where it sets the entire array in a single call.
			//int texArrayBoundSlot = texturesArray->getBoundSlotMustExist();
			//shaderProgram->setUniform1iOptional(u_textureArrayArray, texArrayBoundSlot);
		}

		// Note. Use getBoundSlotOptional because the TexArrays can be empty and in that case they are not used by any vertices (guaranteed) and not bound.
		if (globalTextures->count() == 1) {
			int texArrayBoundSlot = globalTextures->getDirect(0)->getBoundSlotOptional();
			shaderProgram->setUniform1iOptional(u_textureArrayArray, texArrayBoundSlot);
		} else if (globalTextures->count() == 2) {
			int texArrayBoundSlot = globalTextures->getDirect(1)->getBoundSlotOptional();
			int vals[2]{
				globalTextures->getDirect(0)->getBoundSlotOptional(),
				globalTextures->getDirect(1)->getBoundSlotOptional()
			};

			shaderProgram->setUniform1ivOptional(
				u_textureArrayArray, 
				2, vals
			);
		} else {
			// Currently only 2 tex_arrays are implemented, implement more version when needed (up to max 4 tex_arrays).
			throw LogicException(LOC);
		}
	}
}

MADiffuseTexturesArray* MADiffuseTexturesArray::clone(Attribute* _o) {
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MADiffuseTexturesArray* o = newt MADiffuseTexturesArray();
	super::clone(o);
	
	o->globalTextures = globalTextures;

	return o;
}

MADiffuseTexturesArray::~MADiffuseTexturesArray() {
	//void
}
