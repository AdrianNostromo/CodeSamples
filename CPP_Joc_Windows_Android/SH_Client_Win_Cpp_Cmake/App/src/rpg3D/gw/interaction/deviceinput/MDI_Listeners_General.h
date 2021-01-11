#pragma once

#include <base/gh.h>
#include "MDI_ListenersBase.h"

namespace rpg3D {
class MDI_Listeners_General : public MDI_ListenersBase {priv typedef MDI_ListenersBase super; pub dCtor(MDI_Listeners_General);
	priv std::function<void(
		std::shared_ptr<base::SensorEvent>& sensorEvent)> cb_onEntityControlInputs;

    pub explicit MDI_Listeners_General(base::IGameWorld* gw);

	pub void setCb_onEntityControlInputs(std::function<void(
		std::shared_ptr<base::SensorEvent>& sensorEvent)> cb_onEntityControlInputs) override;
	
	prot void dispatch_onEntityControlInputs(
		std::shared_ptr<base::SensorEvent>& sensorEvent);

	pub ~MDI_Listeners_General() override;
};
};
