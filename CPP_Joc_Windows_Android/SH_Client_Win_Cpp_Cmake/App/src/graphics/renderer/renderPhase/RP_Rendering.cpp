#include "RP_Rendering.h"
#include <base/opengl/IGL.h>
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/pool/ShadersPool.h>
#include <graphics/shader/IShader.h>
#include <graphics/geometry/Mesh.h>
#include <graphics/model/NodePart.h>
#include <graphics/model/Node.h>
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <graphics/geometry/data/IndexBufferObject.h>
#include <graphics/environment/IEnvironment.h>
#include <graphics/camera/Camera.h>
#include <graphics/geometry/data/MeshPart.h>
#include <graphics/material/Material.h>
#include <graphics/visual/Texture.h>
#include <graphics/visual/TextureArray.h>
#include <base/math/Math.h>
#include <base/screen/IScreen.h>
#include <graphics/attribute/CombinedAttributesList.h>
#include "../renderablesManager/RenderablesManager.h"
#include <graphics/renderer/geometryManager/GeometryManager.h>
#include <graphics/renderer/geometryManager/combinedRenderable/CombinedRenderable.h>
#include <graphics/geometry/data/UniformBufferObject.h>
#include <iostream>
#include <graphics/geometry/uniformAttribute/UniformAttributesList.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Indices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_ModelTransforms.h>
#include <graphics/renderer/geometryManager/bucket/IGeometryBucket.h>

using namespace graphics;

std::string RP_Rendering::u_modelMatrix = "u_modelMatrix";
std::string RP_Rendering::uniformBlockName_ModelTransforms = "ModelTransforms";
//asd_01;// this will require some extra logic. Maybe a uniform slots global list to keep all uniform blocks bound permanently or minimise binding calls.
int RP_Rendering::uniformBlockIndex_ModelTransforms = 1;

std::string RP_Rendering::u_modelNormalMatrix = "u_modelNormalMatrix";
std::string RP_Rendering::uniformBlockName_ModelNormalTransforms = "ModelNormalTransforms";
//asd_01;// this will require some extra logic. Maybe a uniform slots global list to keep all uniform blocks bound permanently or minimise binding calls.
int RP_Rendering::uniformBlockIndex_ModelNormalTransforms = 2;

std::string RP_Rendering::uniformBlockName_RemapingModelTransformsIndices = "RemapingModelTransformsIndices";
//asd_01;// this will require some extra logic. Maybe a uniform slots global list to keep all uniform blocks bound permanently or minimise binding calls.
int RP_Rendering::uniformBlockIndex_RemapingModelTransformsIndices = 3;

RP_Rendering::RP_Rendering(
	int renderOrder, bool useDepthTest,
	base::IScreen* screen,
	long supportedAutomaticAttributesBitMask, long ignorredAutomaticAttributesBitMask)
	: super(renderOrder, useDepthTest),
	screen(screen),
	supportedAutomaticAttributesBitMask(supportedAutomaticAttributesBitMask), ignorredAutomaticAttributesBitMask(ignorredAutomaticAttributesBitMask)
{
	//void
}

void RP_Rendering::createMain() {
	super::createMain();

	if (!capabilities.isInit) {
		// The capabilities must be initialised in a constructor.
		throw LogicException(LOC);
	}
}

void RP_Rendering::render(graphics::IEnvironment* environment) {
	// Pool the size each time to avoid implementing a size listener. Implement when needed.
	if (viewportSizeOverride.x > 0 && viewportSizeOverride.y > 0) {
		// Custom viewort size.
		CallGLChecked(GLUtil::gl->viewport(0, 0, viewportSizeOverride.x, viewportSizeOverride.y));
	} else {
		CallGLChecked(GLUtil::gl->viewport(0, 0, screen->getWindowResolutionX(), screen->getWindowResolutionY()));
	}

	IShader* activeShader = nullptr;

	for (ListDL<GeometryBucketUser>::Entry* entry = geometryBucketUsers.getFirst(); entry != nullptr; entry = entry->next) {
		IGeometryBucket* bucket = entry->data.bucket;

		int index = 0;
		for (ListDL<CombinedRenderable>::Entry* entry = bucket->getCombinedRenderablesList().getFirst(); entry != nullptr; entry = entry->next) {
			renderRenderable(&entry->data, activeShader, environment);

			index++;
		}
	}

	if (activeShader != nullptr) {
		activeShader->end();
	}

	// Check if there are any errors.
	unsigned int errorCode = GLUtil::gl->getError();
	if (errorCode != IGL::NO_ERROR) {
		throw LogicException(LOC);
	}
}

void RP_Rendering::renderRenderable(IRenderable* renderable, IShader*& activeShader, graphics::IEnvironment* environment) {
	IGeometryProvider_Vertices* geometryProvider_Vertices = renderable->getGeometryProvider_Vertices();
	IGeometryProvider_Indices* geometryProvider_Indices = renderable->getGeometryProvider_Indices();
	IGeometryProvider_ModelTransforms* geometryProvider_ModelTransforms = renderable->getGeometryProvider_ModelTransforms();

	if (geometryProvider_Indices->getIndicesCount() <= 0) {
		return ;
	}

	if (!activeShader || !activeShader->canRender(renderable, environment, &rendererAttributesList)) {
		activeShader = shadersPool->getOrCreateShader(renderable, environment, &rendererAttributesList);
		if (activeShader == nullptr) {
			throw LogicException(LOC);
		}

		activeShader->begin();
	}

	std::shared_ptr<graphics::Material> material = geometryProvider_Vertices->getMaterial();

	long commonAttributes = material->getBitMask() & environment->getBitMask();
	if (commonAttributes != 0) {
		// Overlapp of attributes in the material and environment are currently not allowed.
		throw LogicException(LOC);
	}

	CombinedAttributesList combinedAttributes{ material.get(), environment };

	combinedAttributes.bind(
		activeShader->getProgram(),
		supportedAutomaticAttributesBitMask, ignorredAutomaticAttributesBitMask
	);

	//asd_r;// use some set. maybe this is the place where to set the combined material,environment attributes.

	prepareShaderUniforms(
		activeShader, environment,
		geometryProvider_Vertices, geometryProvider_Indices,
		geometryProvider_ModelTransforms
	);

	//asd_r;// this is only check to set for the 3d colorOut shader.
	if (capabilities.normalMatrix) {
		activeShader->getProgram()->setUniformVec3B("u_cameraPos", *environment->peekCamera()->getPosition(), false/*mustExist*/);

		//asd_r_1;// att vAttributes for the material extra props.
		//activeShader->getProgram()->setUniformIntB("material.specular", 1, false/*mustExist*/);
		activeShader->getProgram()->setUniformFloatB("material.shininess", 32.0f, false/*mustExist*/);
	}

	geometryProvider_Vertices->getVerticesVBO()->bind(activeShader->getProgram());
	geometryProvider_Indices->getIndicesIBO()->bind();

	GLUtil::renderContext->flush();

	activeShader->getProgram()->checkedValidateProgramIfNeeded();

	unsigned int count = geometryProvider_Indices->getIndicesCount();
	if (count > 65536) {
		// The indices exceed the unsigned short limit, need to split into multiple calls or something.
		throw LogicException(LOC);
	}
	// Note. indices is a offset from a buffer start, there is no data in it. It is probably nullptr always.
	GLvoid* indices = (void*)(geometryProvider_Indices->getIndicesOffset() * geometryProvider_Indices->getIndicesIBO()->getBuffer().stride());

	CallGLChecked(GLUtil::gl->drawElements(
		IGL::TRIANGLES,
		count/*count*/, IGL::UNSIGNED_SHORT,
		indices
	));

	combinedAttributes.reservedDispose();

	// Don't unbind because this may reduce the number of bindings.
	/*geometryProvider_Vertices->getVerticesVBO()->unbind();
	geometryProvider_Indices->getIndicesIBO()->unbind();
	if (geometryProvider_ModelTransforms != nullptr) {
		geometryProvider_ModelTransforms->getModelTransformsUBO()->unbind();
	}
	if (geometryProvider_ModelTransforms != nullptr && geometryProvider_ModelTransforms->getModelNormalTransformsUBO() != nullptr) {
		geometryProvider_ModelTransforms->getModelNormalTransformsUBO()->unbind();
	}*/

	// This set to the default values.
	GLUtil::renderContext->setDepthTest();
	GLUtil::renderContext->setCullFace();
	GLUtil::renderContext->setFrontFace();
	GLUtil::renderContext->setBlending();
}

void RP_Rendering::prepareShaderUniforms(
	IShader* shader, graphics::IEnvironment* environment,
	IGeometryProvider_Vertices* geometryProvider_Vertices, IGeometryProvider_Indices* geometryProvider_Indices,
	IGeometryProvider_ModelTransforms* geometryProvider_ModelTransforms)
{
	if(geometryProvider_ModelTransforms != nullptr) {
		UniformBufferObject* modelTransformsUBO = geometryProvider_ModelTransforms->getModelTransformsUBO();
		if (modelTransformsUBO != nullptr) {
			if (modelTransformsUBO->getBuffer().count() == 0) {
				// Some vertices exist at this location and they use at least 1 transform but no transform provided.
				// This should never occur.
				throw LogicException(LOC);
			}

			// Bind the optional remaping indices for model transforms;
			UniformBufferObject* remapingModelTransformsIndicesUBO = geometryProvider_ModelTransforms->getRemapingModelTransformsIndicesUBOOptional();
			if (remapingModelTransformsIndicesUBO != nullptr) {
				remapingModelTransformsIndicesUBO->bind(uniformBlockIndex_RemapingModelTransformsIndices);
				shader->getProgram()->uniformBlockBinding(uniformBlockName_RemapingModelTransformsIndices, uniformBlockIndex_RemapingModelTransformsIndices);
			}

			UniformBufferObject* modelNormalTransformsUBO = geometryProvider_ModelTransforms->getModelNormalTransformsUBO();
			if (modelNormalTransformsUBO == nullptr) {
				// Currently normals transforms must be used if model transforms are used. When needed, make the normal transforms as a optional extra.
				throw LogicException(LOC);
			}
			if (geometryProvider_ModelTransforms->getWorldTransformOptional() != nullptr) {
				// Array mode and single mode detected.
				throw LogicException(LOC);
			}

			modelTransformsUBO->bind(uniformBlockIndex_ModelTransforms);
			shader->getProgram()->uniformBlockBinding(uniformBlockName_ModelTransforms, uniformBlockIndex_ModelTransforms);

			if (capabilities.normalMatrix) {
				modelNormalTransformsUBO->bind(uniformBlockIndex_ModelNormalTransforms);
				shader->getProgram()->uniformBlockBinding(uniformBlockName_ModelNormalTransforms, uniformBlockIndex_ModelNormalTransforms);
			}
		} else {
			Matrix4* worldTransform = geometryProvider_ModelTransforms->getWorldTransformOptional();

			if (worldTransform != nullptr) {
				if (geometryProvider_ModelTransforms->getModelTransformsUBO() != nullptr || geometryProvider_ModelTransforms->getModelNormalTransformsUBO() != nullptr) {
					// Array mode and single mode detected.
					throw LogicException(LOC);
				}

				Matrix4* normalWorldTransform = geometryProvider_ModelTransforms->getNormalWorldTransformOptional();
				if (normalWorldTransform == nullptr) {
					throw LogicException(LOC);
				}

				// Need to use a conversion because internaly the normal transform is a Mat4 as a optimisation because UBO with a Mat3 array is used and that requires 4_float_padding.
				Matrix3 mat3{ *normalWorldTransform };
				shader->getProgram()->setUniformMatrix4MustExist(u_modelMatrix, worldTransform);
				if (capabilities.normalMatrix) {
					shader->getProgram()->setUniformMatrix3Optional(u_modelNormalMatrix, &mat3);
				}
			}
		}
	}
}

void RP_Rendering::dispose() {
	//void

	super::dispose();
}

RP_Rendering::~RP_Rendering() {
	//void
}
