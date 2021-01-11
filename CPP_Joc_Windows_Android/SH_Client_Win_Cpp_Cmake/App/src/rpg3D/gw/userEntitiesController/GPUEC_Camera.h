#pragma once

#include <base/gh.h>
#include "GPUEC_InputsCache.h"

namespace rpg3D {
class GPUEC_Camera : public GPUEC_InputsCache {priv typedef GPUEC_InputsCache super;pub dCtor(GPUEC_Camera);
	priv std::shared_ptr<Vector3> lastDispatched_cameraDir = nullptr;

    pub explicit GPUEC_Camera(base::IGameWorld* gw);

	pub void onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) override;

	priv void dispatch_CameraDirection(const Vector3& camDir);

	pub void clearInput() override;

    pub ~GPUEC_Camera() override;
};
};
