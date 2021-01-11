#pragma once

#include <base/gh.h>
#include "P3DM_Dynamics.h"
#include <worldGame3D/gw/entity/util/IPositional3DHandler.h>
#include <base/math/Matrix4.h>
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>

namespace rpg3D {
class P3DM_Positional : public P3DM_Dynamics, virtual public worldGame3D::IPositional3DHandler {priv typedef P3DM_Dynamics super;pub dCtor(P3DM_Positional);
	priv static const int POSITIONAL_HEANDLER_PRIORITY;

	priv bool isMainPositionalHandler = false;

	priv Matrix4 t_mat4{};
	priv Vector3 t_v3{};
	priv Quaternion t_q{};

    pub explicit P3DM_Positional(
		IWorldEntity* entity,
		Physics3DTemplate* t,
		int physicsGroupsBitMap,
		int anyInteraction_maskBitMap, int hardImpact_maskBitMap);
	prot void createMain() override;

	pub void setMainIsPositionalHandler(bool isMainPositionalHandler) final;

	pub Vector3* getPos() final;
	pub float getX() final;
	pub float getY() final;
	pub float getZ() final;
	pub void setPos(float x, float y, float z) final;
	pub void setPos(Vector3& pos) final;
	pub void setX(float x) final;
	pub void setY(float y) final;
	pub void setZ(float z) final;

	pub Quaternion* getRot() final;
	pub void setRot(Quaternion& rot) final;
	pub void setRot(Vector3& axis, float degrees) final;
	pub void setRotEulerZYX(Vector3& axis, float degrees) final;
	
	pub void setCardinalRotation(float degrees) final;

	pub Vector3* getScale() final;
	pub void setScale(float scale) final;

    pub ~P3DM_Positional() override;
};
};
