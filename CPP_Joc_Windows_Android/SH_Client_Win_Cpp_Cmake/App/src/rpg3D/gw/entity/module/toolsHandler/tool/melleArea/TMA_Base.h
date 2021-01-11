#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/cooldownTool/CooldownTool.h>
#include <rpg3D/gw/projectile/ProjectileConfig.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/melleArea/config/ToolConfigMelleArea.h>
#include <base/map/ExtraData.h>
#include <base/audio/sound/ISound.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/groups/IToolWeapon.h>

namespace rpg3D {
class TMA_Base : public CooldownTool, virtual public IToolWeapon {priv typedef CooldownTool super;pub dCtor(TMA_Base);
	pub static int ID;
	pub int getId() final;

	prot ToolConfigMelleArea* config;

	prot std::shared_ptr<ExtraData> extraData;

	prot base::audio::ISound* activationSound = nullptr;
	prot base::audio::ISound* impactSound = nullptr;

	priv bool isInitExtras = false;

	prot ArrayList<std::shared_ptr<rpg3D::EffectPayload>> payloadsList{};

	pub explicit TMA_Base(ToolConfigMelleArea* config, std::shared_ptr<ExtraData> extraData);
	prot void create(IAssetsManager* assetsManager) override;

	prot void onEntityChanged() override;

	prot ToolConfigMelleArea* getLocalConfig();

	pub ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* peekPayloadsList();

	pub void addPayload(std::shared_ptr<rpg3D::EffectPayload> payload);
	pub void addPayloadsList(ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* payloadsList);

	pub ~TMA_Base() override;
};
};
