#pragma once

#include <base/gh.h>
#include "GPManZonesChanger.h"

namespace battlezones_a_v1 {
class GPMan_PlayerUnitDeathHandling : public GPManZonesChanger {priv typedef GPManZonesChanger super;pub dCtor(GPMan_PlayerUnitDeathHandling);
	priv ArrayList<std::shared_ptr<IEventListener>> playerDeathEventsListenersList{};

	pub explicit GPMan_PlayerUnitDeathHandling(base::IGameWorld* gw);

	priv void onEvent(IEventListener& eventListener, base::GWEvent& event);

	priv void onPlayerEntityDeathEvent(IEventListener& eventListener, base::EntityEvent& event);

	pub ~GPMan_PlayerUnitDeathHandling() override;
};
};
