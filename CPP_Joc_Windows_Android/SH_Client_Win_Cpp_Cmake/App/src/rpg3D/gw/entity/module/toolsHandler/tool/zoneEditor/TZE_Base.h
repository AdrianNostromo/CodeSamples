#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
#include <base/map/ExtraData.h>

namespace base {
	namespace audio {
		class ISound;
	}
};

namespace rpg3D {
	class ToolConfigZoneEditor;
};

namespace rpg3D {
class TZE_Base : public Tool {priv typedef Tool super;pub dCtor(TZE_Base);
	pub static int ID;
	pub int getId() final;

	prot static const float EDIT_RANGE;

	prot ToolConfigZoneEditor* config;

	prot std::shared_ptr<ExtraData> extraData;

	prot base::audio::ISound* activationSound = nullptr;

	prot bool isInitExtras = false;

	priv float triggerCooldownS = 0.0f;

    pub explicit TZE_Base(ToolConfigZoneEditor* config, std::shared_ptr<ExtraData> extraData);
	prot void create(IAssetsManager* assetsManager) override;

	pub bool canTrigger() final;

	pub void onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) override;

	prot void onEntityChanged() override;

	prot virtual void onCooldownStateChanged();

	prot void chargeCooldown();

	pub ~TZE_Base() override;
};
};
