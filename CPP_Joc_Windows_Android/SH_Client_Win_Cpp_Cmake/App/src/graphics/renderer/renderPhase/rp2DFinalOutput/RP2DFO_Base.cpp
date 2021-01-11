#include "RP2DFO_Base.h"
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <graphics/material/attributeTypes/MACastColor.h>
#include <graphics/material/attributeTypes/MABlending.h>
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>
#include <graphics/material/attributeTypes/EAColorCamera.h>
#include <graphics/shader/s2D/Shader2DDefault.h>
#include <base/memory/SharedPointer.h>
#include <graphics/util/GraphicsUtil.h>
#include <graphics/renderer/geometryManager/bucket/config/GeometryBucketConfig.h>

using namespace graphics;

IShader* RP2DFO_Base::ShadersPoolLocal::createNewShader(
	IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList)
{
	//asd_r;// use configs for the array sizes.
	Shader2DDefault* shader = new Shader2DDefault();
	shader->init(renderable, environment, rendererAttributesList);

	return shader;
}

RP2DFO_Base::RP2DFO_Base(
	int renderOrder,
	base::IScreen* screen)
	: super(
		renderOrder/*renderOrder*/, false/*useDepthTest*/,
		screen,
		MABlending::TypeBitGroup | MACastColor::TypeBitGroup | MADiffuseTexturesArray::TypeBitGroup | EAColorCamera::TypeBitGroup/*supportedAutomaticAttributesBitMask*/,
		0/*ignorredAutomaticAttributesBitMask*/
	)
{
	capabilities.init(
		graphics::MABlending::TypeBitGroup/*materialAttributes*/,
		false/*normalMatrix*/
	);

	geometryBucketUsers.appendEmplace(sp<GeometryBucketConfig>(new GeometryBucketConfig(
		GraphicsUtil::VertexAttributesList_2DGenericTexture, GraphicsUtil::material_2DGenericTexture
	)));
}

void RP2DFO_Base::create() {
	super::create();

	shadersPool = new ShadersPoolLocal();
}

void RP2DFO_Base::dispose() {
	if (shadersPool != nullptr) {
		shadersPool->reservedDispose();
		delete shadersPool;
		shadersPool = nullptr;
	}

	super::dispose();
}

RP2DFO_Base::~RP2DFO_Base() {
	//void
}
