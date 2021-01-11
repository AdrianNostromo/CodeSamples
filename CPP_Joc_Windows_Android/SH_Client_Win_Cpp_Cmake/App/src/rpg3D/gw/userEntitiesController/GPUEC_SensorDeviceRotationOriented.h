#pragma once

#include <base/gh.h>
#include "GPUEC_LockedCursorMoved.h"

namespace rpg3D {
class GPUEC_SensorDeviceRotationOriented : public GPUEC_LockedCursorMoved {priv typedef GPUEC_LockedCursorMoved super;pub dCtor(GPUEC_SensorDeviceRotationOriented);
	pub explicit GPUEC_SensorDeviceRotationOriented(base::IGameWorld* gw);

	prot bool processCacheInput(std::shared_ptr<base::SensorEvent>& input) override;
    priv void processInput_RotationPitchRollOriented(float pitchDeg, float rollDeg_delta);

	pub ~GPUEC_SensorDeviceRotationOriented() override;
};
};
