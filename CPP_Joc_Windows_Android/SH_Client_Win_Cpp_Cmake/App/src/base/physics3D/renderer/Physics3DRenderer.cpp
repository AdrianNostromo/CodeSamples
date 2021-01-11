#include "Physics3DRenderer.h"
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <graphics/shader/Shader.h>
#include <base/opengl/IGL.h>
#include <base/opengl/util/GLUtil.h>
#include <graphics/util/GraphicsUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/camera/Camera.h>
#include <base/exceptions/LogicException.h>
#include <base/app/IApp.h>
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <graphics/geometry/data/IndexBufferObject.h>

using namespace physics3D;

std::string Physics3DRenderer::u_colorCameraProjectionViewMatrix = "u_colorCameraProjectionViewMatrix";

Physics3DRenderer::Physics3DRenderer(IApp* app, btDiscreteDynamicsWorld* pWorld, std::shared_ptr<graphics::ICinematicController> cinematicController)
	: app(app),
	pWorld(pWorld), cinematicController(cinematicController)
{
	//void
}

void Physics3DRenderer::create() {
	shapeDrawer.create();

	pWorld->setDebugDrawer(&shapeDrawer);
}

void Physics3DRenderer::setIsClearDepthBuffer(bool isClearDepthBuffer) {
	this->isClearDepthBuffer = isClearDepthBuffer;
}

void Physics3DRenderer::prepareForRendering(graphics::IEnvironment* environment) {
	//void
}

void Physics3DRenderer::render(graphics::IEnvironment* environment) {
	// Activate shader and set the camera uniform.
	if (shader == nullptr) {
		shader = graphics::GraphicsUtil::CreateFilesSourcesShader(
			"shaders/physics3D.vert",
			"shaders/physics3D.frag"
		);
	}

	GLUtil::renderContext->setDepthTest(true, IGL::LESS/*depthFunction*/);
	if (isClearDepthBuffer) {
		GLUtil::gl->clear(IGL::DEPTH_BUFFER_BIT);
	}

	CallGLChecked(GLUtil::gl->viewport(0, 0, app->getScreen()->getWindowResolutionX(), app->getScreen()->getWindowResolutionY()));

	shader->begin();

	// Do the transform calculation in the vShader because it is faster.
	shader->getProgram()->setUniformMatrix4Optional(u_colorCameraProjectionViewMatrix, cinematicController->peekCamera()->getCombined());
	
	shapeDrawer.clear();

	pWorld->debugDrawWorld();

	shapeDrawer.vbo->bind(shader->getProgram());
	if (shapeDrawer.ibo != nullptr) {
		shapeDrawer.ibo->bind();
	}

	shader->getProgram()->checkedValidateProgramIfNeeded();

	GLUtil::renderContext->flush();

	if (shapeDrawer.ibo != nullptr) {
		CallGLChecked(GLUtil::gl->drawElements(
			IGL::TRIANGLES,
			shapeDrawer.ibo->getBuffer().count()/*count*/, IGL::UNSIGNED_SHORT,
			0/*indicesByteOffset*/
		));

		throw LogicException(LOC);// this is untested but should work, remove this and test the code.
	}else {
		GLUtil::gl->drawArrays(IGL::LINES, 0/*first*/, shapeDrawer.vbo->getBuffer().count()/*count*/);
	}

	GLUtil::renderContext->setDepthTest();
}

Physics3DRenderer::~Physics3DRenderer() {
	if (shader != nullptr) {
		shader->reservedDispose();
		delete shader;
		shader = nullptr;
	}
}
