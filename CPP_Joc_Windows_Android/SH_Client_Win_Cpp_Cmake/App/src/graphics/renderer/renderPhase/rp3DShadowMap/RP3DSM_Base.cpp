#include "RP3DSM_Base.h"
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <graphics/material/attributeTypes/MACastShadow.h>
#include <graphics/material/attributeTypes/MAInt.h>
#include <graphics/material/attributeTypes/MACastColor.h>
#include <graphics/material/attributeTypes/MAReceiveShadow.h>
#include <graphics/material/attributeTypes/EAColorCamera.h>
#include <graphics/material/attributeTypes/MADepthTest.h>
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>
#include <graphics/material/attributeTypes/MAShadowMapLight3D.h>
#include <graphics/material/attributeTypes/MABool.h>
#include <graphics/material/attributeTypes/EAWorldFocusPosition3D.h>
#include <graphics/shader/s3D/Shader3DShadowDepthMap.h>
#include <graphics/material/attributeTypes/MADirectionalLightsList.h>
#include <graphics/material/attributeTypes/MAPointLightsList.h>
#include <graphics/material/attributeTypes/MASpotLightsList.h>
#include <graphics/util/GraphicsUtil.h>
#include <graphics/renderer/geometryManager/bucket/config/GeometryBucketConfig.h>

using namespace graphics;

IShader* RP3DSM_Base::ShadersPoolLocal::createNewShader(
	IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList)
{
	Shader3DShadowDepthMap* shader = new Shader3DShadowDepthMap();
	shader->init(renderable, environment, rendererAttributesList);

	return shader;
}

RP3DSM_Base::RP3DSM_Base(
	int renderOrder,
	base::IScreen* screen)
	: super(
		renderOrder/*renderOrder*/, true/*useDepthTest*/,
		screen,
		MAInt::TypeBitGroup_FrontFace | MACastShadow::TypeBitGroup | MADepthTest::TypeBitGroup | MADiffuseTexturesArray::TypeBitGroup | MAShadowMapLight3D::TypeBitGroup/*supportedAutomaticAttributesBitMask*/,
		MAInt::TypeBitGroup_CullFace | MACastColor::TypeBitGroup | MAReceiveShadow::TypeBitGroup | EAColorCamera::TypeBitGroup | EAWorldFocusPosition3D::TypeBitGroup | MADirectionalLightsList::TypeBitGroup | MAPointLightsList::TypeBitGroup | MASpotLightsList::TypeBitGroup | MABool::TypeBitGroup_LightingStatus/*ignorredAutomaticAttributesBitMask*/
	)
{
	capabilities.init(
		0/*materialAttributes*/,
		false/*normalMatrix*/
	);

	geometryBucketUsers.appendEmplace(sp<GeometryBucketConfig>(new GeometryBucketConfig(
		GraphicsUtil::VertexAttributesList_3DGenericTexture, GraphicsUtil::material_3DGenericTexture_solid
	)));
}

void RP3DSM_Base::create() {
	super::create();

	shadersPool = new ShadersPoolLocal();
}

void RP3DSM_Base::dispose() {
	if (shadersPool != nullptr) {
		shadersPool->reservedDispose();
		delete shadersPool;
		shadersPool = nullptr;
	}

	super::dispose();
}

RP3DSM_Base::~RP3DSM_Base() {
	//void
}
