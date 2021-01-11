#include "RP3DFO_Base.h"
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <graphics/material/attributeTypes/MACastColor.h>
#include <graphics/material/attributeTypes/MAReceiveShadow.h>
#include <graphics/material/attributeTypes/MABlending.h>
#include <graphics/material/attributeTypes/MADepthTest.h>
#include <graphics/material/attributeTypes/MAInt.h>
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>
#include <graphics/material/attributeTypes/MACastShadow.h>
#include <graphics/material/attributeTypes/EAColorCamera.h>
#include <graphics/material/attributeTypes/EAWorldFocusPosition3D.h>
#include <graphics/material/attributeTypes/MADirectionalLightsList.h>
#include <graphics/material/attributeTypes/MAPointLightsList.h>
#include <graphics/material/attributeTypes/MASpotLightsList.h>
#include <graphics/material/attributeTypes/MABool.h>
#include <graphics/material/attributeTypes/MAShadowMapLight3D.h>
#include <graphics/shader/s3D/Shader3DDefault.h>
#include <graphics/util/GraphicsUtil.h>
#include <graphics/renderer/geometryManager/bucket/config/GeometryBucketConfig.h>

using namespace graphics;

IShader* RP3DFO_Base::ShadersPoolLocal::createNewShader(
	IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList)
{
	//asd_r;// use configs for the array sizes.
	Shader3DDefault* shader = new Shader3DDefault();
	shader->init(renderable, environment, rendererAttributesList);

	return shader;
}

RP3DFO_Base::RP3DFO_Base(
	int renderOrder,
	base::IScreen* screen)
	: super(
		renderOrder/*renderOrder*/, true/*useDepthTest*/,
		screen,
		MAInt::TypeBitGroup_FrontFace | MAInt::TypeBitGroup_CullFace | MADepthTest::TypeBitGroup | MADiffuseTexturesArray::TypeBitGroup | MACastColor::TypeBitGroup | MAReceiveShadow::TypeBitGroup | EAColorCamera::TypeBitGroup | EAWorldFocusPosition3D::TypeBitGroup | MADirectionalLightsList::TypeBitGroup | MAPointLightsList::TypeBitGroup | MASpotLightsList::TypeBitGroup | MABool::TypeBitGroup_LightingStatus | MAShadowMapLight3D::TypeBitGroup/*supportedAutomaticAttributesBitMask*/,
		MACastShadow::TypeBitGroup/*ignorredAutomaticAttributesBitMask*/
	)
{
	capabilities.init(
		graphics::MABlending::TypeBitGroup/*materialAttributes*/,
		true/*normalMatrix*/
	);

	geometryBucketUsers.appendEmplace(sp<GeometryBucketConfig>(new GeometryBucketConfig(
		GraphicsUtil::VertexAttributesList_3DGenericTexture, GraphicsUtil::material_3DGenericTexture_solid
	)));
}

void RP3DFO_Base::create() {
	super::create();

	shadersPool = new ShadersPoolLocal();
}

void RP3DFO_Base::dispose() {
	if (shadersPool != nullptr) {
		shadersPool->reservedDispose();
		delete shadersPool;
		shadersPool = nullptr;
	}

	super::dispose();
}

RP3DFO_Base::~RP3DFO_Base() {
	//void
}
