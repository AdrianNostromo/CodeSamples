#pragma once

#include <base/gh.h>
#include <base/math/Vector3.h>
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>
#include <base/list/ArrayList.h>
#include <rpg3D/gw/projectile/EffectPayload.h>
#include "ITargetedProjectileModule.h"
#include <base/audio/sound/ISound.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>

namespace rpg3D {
	class TargetedProjectileTemplate;
	class IPayloadReceiverTargetModule;
};

namespace rpg3D {
class TPM_Base : public EntityModule, virtual public ITargetedProjectileModule {priv typedef EntityModule super;pub dCtor(TPM_Base);
	prot static EntityRemoveReason *const ENT_REM_REASON_PURPOSE_FINISHED;

	prot TargetedProjectileTemplate* t;

	prot ArrayList<std::shared_ptr<rpg3D::EffectPayload>> payloadsList{};

	priv base::audio::ISound* impactSound = nullptr;

    pub explicit TPM_Base(IWorldEntity* entity, TargetedProjectileTemplate* t);
	prot void createBMain() override;

	prot void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	prot void disconnectFromGamePartsPre() override;

	pub ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* peekPayloadsList();

	pub void addPayload(std::shared_ptr<rpg3D::EffectPayload> payload) final;
	pub void addPayloadsList(ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* payloadsList) final;

	pub ~TPM_Base() override;
};
};
