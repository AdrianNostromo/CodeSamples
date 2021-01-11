#pragma once

#include <base/gh.h>
#include "V3DM_Drawables.h"
#include <worldGame3D/gw/entity/util/IPositional3DHandler.h>

namespace worldGame3D {
class V3DM_Positional : public V3DM_Drawables, virtual public IPositional3DHandler {priv typedef V3DM_Drawables super;pub dCtor(V3DM_Positional);
	priv static const int POSITIONAL_HEANDLER_PRIORITY;

	priv bool isMainPositionalHandler = false;

    pub explicit V3DM_Positional(
		IWorldEntity* entity,
		Visual3DTemplate* t,
		ArrayList<IDrawableNode3D*>* drawablesList, Map1D<std::string, IDrawableNode3D*>* drawablesMap, IDrawableNode3D* rootDrawable);
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

    pub ~V3DM_Positional() override;
};
};
