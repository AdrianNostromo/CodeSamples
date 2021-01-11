#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/ammunitionCooldownTool/AmmunitionCooldownTool.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/rangeInertialProjectile/config/ToolConfigRangeInertialProjectile.h>
#include <base/map/ExtraData.h>
#include <base/audio/sound/ISound.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/groups/IToolWeapon.h>

namespace rpg3D {
class TRIP_Base : public AmmunitionCooldownTool, virtual public IToolWeapon {priv typedef AmmunitionCooldownTool super;pub dCtor(TRIP_Base);
	pub static int ID;
	pub int getId() final;

	prot ToolConfigRangeInertialProjectile* config;

	prot std::shared_ptr<ExtraData> extraData;

	prot base::audio::ISound* activationSound = nullptr;

	pub explicit TRIP_Base(ToolConfigRangeInertialProjectile* config, std::shared_ptr<ExtraData> extraData);
	prot void create(IAssetsManager* assetsManager) override;

	prot ToolConfigRangeInertialProjectile* getLocalConfig();

	pub ~TRIP_Base() override;
};
};
