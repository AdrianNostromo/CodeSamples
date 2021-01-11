#include "RenderPhase2DFinalOutput.h"
#include <graphics/geometry/data/UniformBufferObject.h>
#include <graphics/geometry/uniformAttribute/UniformAttributesList.h>
#include <graphics/geometry/uniformAttribute/UniformAttribute.h>
#include <graphics/environment/IEnvironment.h>
#include <graphics/camera/Camera.h>
#include <graphics/geometry/data/FrameBufferObject.h>

using namespace graphics;

std::string RenderPhase2DFinalOutput::uniformBlockName_Matrices = "Matrices";

RenderPhase2DFinalOutput::RenderPhase2DFinalOutput(
	int renderOrder,
	base::IScreen* screen)
	: super(renderOrder,
		screen)
{
	rawSubclassPointer = (IRenderPhase2DFinalOutput*)this;
}

void RenderPhase2DFinalOutput::createMain() {
	super::createMain();

	{
		std::shared_ptr<UniformAttributesList> uniformAttributesList = std::make_shared<UniformAttributesList>();
		uniformAttributesList->pushAttribute(UniformAttributesList::Type::projViewMatrix);
		uniformAttributesList->lock();

		uboMatrices = new UniformBufferObject(
			uniformAttributesList,
			graphics::UniformBufferObject::UpdateMethod::SingleRegion,
			1/*initialCapacity*/, 1/*minCapacity*/
		);
		uboMatrices->getBuffer().lockSize();
		uboMatrices->reservedCreate();

		uboMatrices->getBuffer().append(nullptr/*srcData*/, 1/*srcCount*/, sizeof(Matrix4));
	}
}

void RenderPhase2DFinalOutput::render(graphics::IEnvironment* environment) {
	// This is added because future dev may use multiple fbo-s.
	FrameBufferObject::DefaultFBO->bind();

	super::render(environment);
}

void RenderPhase2DFinalOutput::prepareShaderUniforms(
	IShader* shader, graphics::IEnvironment* environment,
	IGeometryProvider_Vertices* geometryProvider_Vertices, IGeometryProvider_Indices* geometryProvider_Indices,
	IGeometryProvider_ModelTransforms* geometryProvider_ModelTransforms)
{
	super::prepareShaderUniforms(shader, environment, geometryProvider_Vertices, geometryProvider_Indices, geometryProvider_ModelTransforms);

	{
		Matrix4 const* projViewMat = environment->peekCamera()->getCombined();

		//asd_r;// this will require some extra logic. Maybe a uniform slots global list to keep all uniform blocks bound permanently or minimise binding calls.
		int uniformBindSlot = 0;

		memcpy(uboMatrices->getBuffer().getData(), projViewMat, sizeof(Matrix4));
		uboMatrices->invalidateUsedRegion();

		uboMatrices->bind(uniformBindSlot);

		shader->getProgram()->uniformBlockBinding(uniformBlockName_Matrices, uniformBindSlot);
	}
}

void RenderPhase2DFinalOutput::dispose() {
	if (uboMatrices != nullptr) {
		uboMatrices->reservedDispose();
		delete uboMatrices;
		uboMatrices = nullptr;
	}

	super::dispose();
}

RenderPhase2DFinalOutput::~RenderPhase2DFinalOutput() {
	//void
}
