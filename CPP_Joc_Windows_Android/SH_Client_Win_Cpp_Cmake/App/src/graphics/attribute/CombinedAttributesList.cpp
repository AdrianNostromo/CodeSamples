#include "CombinedAttributesList.h"
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>
#include <graphics/material/attributeTypes/MADiffuseTexture.h>
#include <graphics/material/attributeTypes/MAInt.h>
#include <graphics/material/attributeTypes/MADepthTest.h>
#include <graphics/material/attributeTypes/MABlending.h>
#include <graphics/material/attributeTypes/MADirectionalLightsList.h>
#include <graphics/material/attributeTypes/MAPointLightsList.h>
#include <graphics/material/attributeTypes/MASpotLightsList.h>
#include <graphics/material/attributeTypes/light/DirectionalLight.h>
#include <graphics/material/attributeTypes/light/PointLight.h>
#include <graphics/material/attributeTypes/light/SpotLight.h>
#include <graphics/material/attributeTypes/MAShadowMapLight3D.h>
#include <graphics/material/attributeTypes/MACastShadow.h>
#include <graphics/material/attributeTypes/MAReceiveShadow.h>
#include <graphics/material/attributeTypes/EAColorCamera.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <base/opengl/util/GLUtil.h>
#include <graphics/material/Material.h>
#include <graphics/visual/Texture.h>
#include <graphics/visual/TextureArray.h>
#include <graphics/camera/OrthographicCamera.h>

using namespace graphics;

CombinedAttributesList::CombinedAttributesList(IAttributesList* list1, IAttributesList* list2)
	: super(),
	lists{ list1, list2 }
{
	bitMask = list1->getBitMask() | list2->getBitMask();
}

long CombinedAttributesList::getBitMask() {
	return bitMask;
}

ArrayList<Attribute*>* CombinedAttributesList::peekList() {
	// Not allowed. This function is only for the sub attribute lists.
	throw LogicException(LOC);
}

void CombinedAttributesList::bind(
	ShaderProgram* shaderProgram,
	long supportedAutomaticAttributesBitMask, long ignorredAutomaticAttributesBitMask)
{
	bool isTexture = false;
	bool isTextureArray = false;

	for (int i = 0; i < 2; i++) {
		IAttributesList* attributesList = lists[i];
		ArrayList<Attribute*>* list = attributesList->peekList();

		for (int j = 0; j < list->count(); j++) {
			Attribute* attribute = list->getDirect(j);

			if ((supportedAutomaticAttributesBitMask & attribute->typeBitGroup) == attribute->typeBitGroup) {
				attribute->bind(this, shaderProgram);
			} else if ((ignorredAutomaticAttributesBitMask & attribute->typeBitGroup) == attribute->typeBitGroup) {
				//void
			} else {
				// Unconfigured ttribute, add it to supported or ignorred attributes mask.
				int asd_t_1 = 0;// throw LogicException(LOC);
			}
		}
	}
}

Attribute* CombinedAttributesList::getOptional(long typeBitGroup) {
	Attribute* attribute = nullptr;

	for (int i = 0; i < 2; i++) {
		attribute = lists[i]->getOptional(typeBitGroup);
		if (attribute != nullptr) {
			break;
		}
	}

	return attribute;
}

CombinedAttributesList::~CombinedAttributesList() {
	//void
}
