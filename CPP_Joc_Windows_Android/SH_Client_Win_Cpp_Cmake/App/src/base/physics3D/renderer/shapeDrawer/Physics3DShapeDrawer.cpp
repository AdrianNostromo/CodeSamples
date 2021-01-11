#include "Physics3DShapeDrawer.h"
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <graphics/geometry/data/IndexBufferObject.h>
#include <base/MM.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace physics3D;

Physics3DShapeDrawer::Physics3DShapeDrawer() {
	//void
}

void Physics3DShapeDrawer::create() {
	std::shared_ptr<graphics::VertexAttributesList> vao = std::make_shared<graphics::VertexAttributesList>();
	vao->pushAttribute(graphics::VertexAttributesList::Type::Position3D);
	vao->pushAttribute(graphics::VertexAttributesList::Type::DiffuseColor);
	vao->lock();

	vbo = new graphics::VertexBufferObject(
		vao,
		graphics::VertexBufferObject::UpdateMethod::SingleRegion,
		40000/*initialCapacity*/, 40000/*minCapacity*/);
	vbo->getBuffer().lockSize();
	vbo->reservedCreate();
}

void Physics3DShapeDrawer::clear() {
	vbo->getBuffer().clear();

	vbo->invalidateUsedRegion();
	if (ibo != nullptr) {
		ibo->invalidateUsedRegion();
	}
}

void Physics3DShapeDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
	GLVertex vert[2]{
		GLVertex(graphics::GLTypes::Vect3(from.x(), from.y(), from.z()), graphics::GLTypes::Col4(color.x(), color.y(), color.z(), 1.0f)),
		GLVertex(graphics::GLTypes::Vect3(to.x(), to.y(), to.z()), graphics::GLTypes::Col4(color.x(), color.y(), color.z(), 1.0f))
	};
	vbo->getBuffer().append(vert, 2, sizeof(GLVertex));
}

void Physics3DShapeDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
	throw LogicException(LOC); // Not implemented. Implement when needed.
}

void Physics3DShapeDrawer::reportErrorWarning(const char* warningString) {
	GlobalAccessLogHandler::devLogHandler->post_debug("Physics engine warning: %s", warningString);
}

void Physics3DShapeDrawer::draw3dText(const btVector3& location, const char* textString) {
	throw LogicException(LOC); // Not implemented. Implement when needed.
}

void Physics3DShapeDrawer::setDebugMode(int debugMode) {
	this->debugMode = debugMode;
}

int Physics3DShapeDrawer::getDebugMode() const {
	return debugMode;
}

Physics3DShapeDrawer::~Physics3DShapeDrawer() {
	if (vbo != nullptr) {
		vbo->reservedDispose();
		delete vbo;
		vbo = nullptr;
	}
}
