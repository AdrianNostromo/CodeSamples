#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IWorldItemModule.h"

namespace base {
	namespace audio {
		class ISound;
	}
};
namespace rpg3D {
	class WorldItemTemplate;
};

namespace rpg3D {
class WIM_Base : public EntityModule, virtual public IWorldItemModule {priv typedef EntityModule super; pub dCtor(WIM_Base);
	pub WorldItemTemplate* t;

	prot ArrayList<std::shared_ptr<rpg3D::EffectPayload>> payloadsList{};
	
	priv base::audio::ISound* collectedSound = nullptr;

	pub WIM_Base(
		IWorldEntity* entity,
		WorldItemTemplate* t);
	prot void createBMain() override;

	prot void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	prot void disconnectFromGamePartsPre() override;

	pub ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* peekPayloadsList() final;

	pub void addPayload(std::shared_ptr<rpg3D::EffectPayload> payload) final;
	pub void addPayloadsList(ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* payloadsList) final;

	pub void onItemCollected() final;

	pub ~WIM_Base() override;
};
};
