#pragma once

#include <base/gh.h>
#include "GPUEC_ScreenTouch2D.h"

namespace rpg3D {
class GPUEC_DirectMovement2D : public GPUEC_ScreenTouch2D {priv typedef GPUEC_ScreenTouch2D super;pub dCtor(GPUEC_DirectMovement2D);
	priv bool isData_moveDirInput = false;
	priv Vector2 data_moveDirInput{};

	priv std::shared_ptr<Vector2> lastDispatch_moveInput = nullptr;
	priv std::shared_ptr<Vector3> lastDispatch_cameraDir = nullptr;

	priv Vector2 lastDispatched_worldSpaceMoveDirVector{};

	priv float lastDispatched_strafeMovement_strafeLeftRight = 0.0f;
	priv float lastDispatched_strafeMovement_backFront = 0.0f;

    pub explicit GPUEC_DirectMovement2D(base::IGameWorld* gw);

	prot bool processCacheInput(std::shared_ptr<base::SensorEvent>& input) override;

	pub void onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) override;

	priv void tick_cameraDirectionMovement(bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraDir, Vector2& moveDirInput, const Vector3& cameraDir);
	priv void tick_strafeMovement(bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraDir, Vector2& moveDirInput, const Vector3& cameraDir);

	pub void clearInput() override;

	pub ~GPUEC_DirectMovement2D() override;
};
};
