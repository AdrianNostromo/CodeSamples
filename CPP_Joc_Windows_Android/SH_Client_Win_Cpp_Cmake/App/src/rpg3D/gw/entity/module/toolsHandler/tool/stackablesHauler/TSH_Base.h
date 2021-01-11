#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>

namespace base {
namespace audio {
	class ISound;
}
};
namespace rpg3D {
	class ToolConfigStackablesHauler;
};

namespace rpg3D {
class TSH_Base : public Tool {priv typedef Tool super; pub dCtor(TSH_Base);
	pub static int ID;
	pub int getId() final;

	prot ToolConfigStackablesHauler* config;

	priv bool isInitExtras = false;

	prot base::audio::ISound* activationSound = nullptr;

	priv float triggerCooldownS = 0.0f;

    pub explicit TSH_Base(ToolConfigStackablesHauler* config);
	prot void create(IAssetsManager* assetsManager) override;

	pub bool canTrigger() final;
	
	prot void chargeCooldown();
	prot virtual void onCooldownStateChanged();

	pub void onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) override;

    pub ~TSH_Base() override;
};
};
