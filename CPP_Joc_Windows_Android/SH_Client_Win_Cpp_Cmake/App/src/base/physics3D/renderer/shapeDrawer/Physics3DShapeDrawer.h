#pragma once

#include <base/gh.h>
#include <LinearMath/btIDebugDraw.h>
#include <graphics/util/GLTypes.h>

namespace graphics {
	class ShaderProgram;
	class VertexBufferObject;
	class IndexBufferObject;
};

namespace physics3D {
class Physics3DShapeDrawer : public virtual btIDebugDraw {pub dCtor(Physics3DShapeDrawer);
	pub class GLVertex {
        pub graphics::GLTypes::Vect3 a_position3D;

        pub graphics::GLTypes::Col4 a_diffuseColor;

        pub explicit GLVertex()
            : a_position3D(), a_diffuseColor()
        {}
        pub explicit GLVertex(graphics::GLTypes::Vect3 a_position3D, graphics::GLTypes::Col4 a_diffuseColor)
            : a_position3D(a_position3D), a_diffuseColor(a_diffuseColor)
        {}
    };

	priv int debugMode = DebugDrawModes::DBG_DrawWireframe;

	pub graphics::VertexBufferObject* vbo = nullptr;
	pub graphics::IndexBufferObject* ibo = nullptr;

    pub explicit Physics3DShapeDrawer();
	pub virtual void create();

	pub void clear();

	pub void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) final;

	pub void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) final;

	pub void reportErrorWarning(const char* warningString) final;

	pub void draw3dText(const btVector3& location, const char* textString) final;

	pub void setDebugMode(int debugMode) final;

	pub int getDebugMode() const final;

    pub ~Physics3DShapeDrawer() override;
};
};
