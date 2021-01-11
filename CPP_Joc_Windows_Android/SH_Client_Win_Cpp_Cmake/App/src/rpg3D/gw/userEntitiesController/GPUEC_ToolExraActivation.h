#pragma once

#include <base/gh.h>
#include "GPUEC_SensorDeviceRotationOriented.h"

namespace rpg3D {
class GPUEC_ToolExraActivation : public GPUEC_SensorDeviceRotationOriented {priv typedef GPUEC_SensorDeviceRotationOriented super;pub dCtor(GPUEC_ToolExraActivation);
    pub explicit GPUEC_ToolExraActivation(base::IGameWorld* gw);

	prot bool processCacheInput(std::shared_ptr<base::SensorEvent>& input) override;

	pub ~GPUEC_ToolExraActivation() override;
};
};
