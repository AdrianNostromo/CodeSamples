#include "MADiffuseTexture.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/visual/Texture.h>
#include <base/MM.h>

using namespace graphics;

std::string MADiffuseTexture::Alias = "diffuseTexture";
long MADiffuseTexture::TypeBitGroup = registerAttribute(Alias);

std::string MADiffuseTexture::u_texture = "u_texture";

MADiffuseTexture::MADiffuseTexture()
	: super(TypeBitGroup)
{
	rawSubclassPointer = this;
}

void MADiffuseTexture::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	super::bind(attributesList, shaderProgram);

	if (texture != nullptr) {
		texture->bind(0);

		int texBoundSlot = texture->getBoundSlotMustExist();
		shaderProgram->setUniform1iOptional(u_texture, texBoundSlot);
	}
}

MADiffuseTexture* MADiffuseTexture::clone(Attribute* _o) {
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MADiffuseTexture* o = newt MADiffuseTexture();
	super::clone(o);

	o->texture = texture;

	return o;
}

MADiffuseTexture::~MADiffuseTexture() {
	//void
}
