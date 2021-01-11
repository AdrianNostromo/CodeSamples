#include "RenderPhase3DFinalOutput.h"
#include <graphics/geometry/data/UniformBufferObject.h>
#include <graphics/geometry/uniformAttribute/UniformAttributesList.h>
#include <graphics/geometry/uniformAttribute/UniformAttribute.h>
#include <graphics/environment/IEnvironment.h>
#include <graphics/camera/Camera.h>
#include <graphics/renderer/renderPhase/rp3DShadowMap/IRP3DShadowMap.h>
#include <graphics/renderer/IRenderer.h>
#include <graphics/visual/Texture.h>
#include <graphics/renderer/renderPhase/rp3DShadowMap/rendererAttributes/RABool.h>
#include <graphics/geometry/data/FrameBufferObject.h>

using namespace graphics;

std::string RenderPhase3DFinalOutput::uniformBlockName_Matrices = "Matrices";
//asd_01;// this will require some extra logic. Maybe a uniform slots global list to keep all uniform blocks bound permanently or minimise binding calls.
int RenderPhase3DFinalOutput::uniformBlockIndex_Matrices = 0;

std::string RenderPhase3DFinalOutput::u_shadowDepthMapTexture = "u_shadowDepthMapTexture";

RenderPhase3DFinalOutput::RenderPhase3DFinalOutput(
	int renderOrder,
	base::IScreen* screen)
	: super(renderOrder,
		screen)
{
	rawSubclassPointer = (IRenderPhase3DFinalOutput*)this;
}

void RenderPhase3DFinalOutput::createMain() {
	super::createMain();

	{
		std::shared_ptr<UniformAttributesList> uniformAttributesList = std::make_shared<UniformAttributesList>();
		uniformAttributesList->pushAttribute(UniformAttributesList::Type::projViewMatrix);
		uniformAttributesList->lock();

		uboMatrices = new UniformBufferObject(uniformAttributesList,
			graphics::UniformBufferObject::UpdateMethod::SingleRegion,
			1/*initialCapacity*/, 1/*minCapacity*/
		);
		uboMatrices->getBuffer().lockSize();
		uboMatrices->reservedCreate();

		uboMatrices->getBuffer().append(nullptr/*srcData*/, 1/*srcCount*/, sizeof(Matrix4));
	}
}

void RenderPhase3DFinalOutput::onIsEnabledChanged() {
	super::onIsEnabledChanged();

	if (!getIsEnabled()) {
		//void
	} else {
		// Create the shadow map framebuffer.
		
		if (IGL::CLAMP_TO_BORDER != 0 && IGL::TEXTURE_BORDER_COLOR != 0) {
			rendererAttributesList.removeAndDispose(RABool::TypeBitGroup_UseShaderDepthMapBorder);
		} else {
			// Feature not supported. On android, this must be done in the shader. Currently don't specify this parameter. Maybe in the future just ignore it but not now.
			rendererAttributesList.set(RABool::NewUseShaderDepthMapBorder(true));
		}
	}
}

void RenderPhase3DFinalOutput::render(graphics::IEnvironment* environment) {
	//asd_2;// need a better system, maybe the renderer can call functions on other rPhases when a rPhase is added or removed maybe call when rPhase active or innactive.
	rp3DShadowMap = renderer->getRenderPhaseAs<IRP3DShadowMap*>(IRP3DShadowMap::TYPE, false/*mustExist*/);

	// This is added because future dev may use multiple fbo-s.
	FrameBufferObject::DefaultFBO->bind();

	super::render(environment);
}

void RenderPhase3DFinalOutput::prepareShaderUniforms(
	IShader* shader, graphics::IEnvironment* environment,
	IGeometryProvider_Vertices* geometryProvider_Vertices, IGeometryProvider_Indices* geometryProvider_Indices,
	IGeometryProvider_ModelTransforms* geometryProvider_ModelTransforms)
{
	super::prepareShaderUniforms(shader, environment, geometryProvider_Vertices, geometryProvider_Indices, geometryProvider_ModelTransforms);

	{
		Matrix4 const* projViewMat = environment->peekCamera()->getCombined();

		//asd_r;// this will require some extra logic. Maybe a uniform slots global list to keep all uniform blocks bound permanently or minimise binding calls.
		memcpy(uboMatrices->getBuffer().getData(), projViewMat, sizeof(Matrix4));
		uboMatrices->invalidateUsedRegion();

		uboMatrices->bind(uniformBlockIndex_Matrices);

		shader->getProgram()->uniformBlockBinding(uniformBlockName_Matrices, uniformBlockIndex_Matrices);
	}

	// Bind the shadow map texture and sampler.
	if (rp3DShadowMap != nullptr && rp3DShadowMap->getIsEnabled()) {
		rp3DShadowMap->getShadowDepthMapTexture()->bind(ITextureUnit::TextureBindSlots::shadowDepthMap3D);
		//asd_x;// shadow may be disabled. Add a shader define for shadow or no shadow and handle this niform set.
		shader->getProgram()->setUniform1iOptional(u_shadowDepthMapTexture, ITextureUnit::TextureBindSlots::shadowDepthMap3D);
	}
}

void RenderPhase3DFinalOutput::dispose() {
	if (uboMatrices != nullptr) {
		uboMatrices->reservedDispose();
		delete uboMatrices;
		uboMatrices = nullptr;
	}

	super::dispose();
}

RenderPhase3DFinalOutput::~RenderPhase3DFinalOutput() {
	//void
}
