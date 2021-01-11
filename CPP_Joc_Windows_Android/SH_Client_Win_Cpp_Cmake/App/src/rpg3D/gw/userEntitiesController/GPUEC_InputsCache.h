#pragma once

#include <base/gh.h>
#include "GPUEC_CustomFilters.h"
#include <base/input/sensor/event/SensorEvent.h>

namespace rpg3D {
class GPUEC_InputsCache : public GPUEC_CustomFilters {priv typedef GPUEC_CustomFilters super;pub dCtor(GPUEC_InputsCache);
	priv ArrayList<std::shared_ptr<base::SensorEvent>> cachedInputsList{};

	prot bool isDirtyControlType = false;

    pub explicit GPUEC_InputsCache(base::IGameWorld* gw);
	prot void createMain() override;

	priv void onEntityControlInputs(std::shared_ptr<base::SensorEvent>& sensorEvent);

	prot virtual bool processCacheInput(std::shared_ptr<base::SensorEvent>& input);

	pub void onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) override;

	prot void onEntityControlTypeChanged() override;
	pub void clearInput() override;
	
    pub ~GPUEC_InputsCache() override;
};
};
