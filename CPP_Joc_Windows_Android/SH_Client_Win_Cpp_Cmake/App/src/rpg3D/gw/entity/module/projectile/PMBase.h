#pragma once

#include <base/math/Vector3.h>
#include <base/gh.h>
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>
#include <base/list/ListDL.h>
#include <rpg3D/gw/entity/template/projectile/ProjectileTemplate.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IProjectileModule.h"
#include <base/audio/sound/ISound.h>
#include <base/MM.h>

namespace base {
class ISensorsInteractionHandler;
class WrappedWorldEntity;
}
namespace rpg3D {
	class ITeamModule;
	class IPayloadReceiverTargetModule;
	class IPhysics3DModule;
	class IPhysicsEntityCallbackFilter;
};

namespace rpg3D {
class PMBase : public EntityModule, virtual public IProjectileModule {priv typedef EntityModule super;pub dCtor(PMBase);
	priv class ImpactedEntityInfo {pub dCtor(ImpactedEntityInfo);
		pub std::shared_ptr<base::WrappedWorldEntity> wEntity;
		// This is used so that OnlyFirstContact impacts don't use the reImpactTimerS on later impacts.
		// These are always incremented on physics contacts even if no payload transfer occured.
		pub int contactsCount = 0;
		pub bool isInContact = false;
		pub float reImpactTimerS = 0;

		pub explicit ImpactedEntityInfo(std::shared_ptr<base::WrappedWorldEntity> wEntity)
			: wEntity(wEntity)
		{}
	};

	prot static EntityRemoveReason *const ENT_REM_REASON_PURPOSE_FINISHED;

	prot ProjectileTemplate* t;

	prot ArrayList<std::shared_ptr<rpg3D::EffectPayload>> payloadsList{};

	prot int targetImpactsCount = 0;
	prot int environmentImpactsCount = 0;

	priv ITeamModule* team = nullptr;
	
	priv rpg3D::IPhysics3DModule* physics3DModule = nullptr;
	priv IPhysicsEntityCallbackFilter* activeTargetUnitFilter = nullptr;
	priv IPhysicsEntityCallbackFilter* activeEnvironmentContactFilter = nullptr;

	priv base::audio::ISound* impactSound = nullptr;

	priv ListDL<ImpactedEntityInfo> impactedEntitiesInfosList{LOC};

	pub explicit PMBase(IWorldEntity* entity, ProjectileTemplate* t);
	prot void createBMain() override;

	priv void onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event);

	priv ImpactedEntityInfo* getContactedEntityInfo(std::shared_ptr<base::WrappedWorldEntity> wEntity);
	priv ListDL<ImpactedEntityInfo>::Entry* getContactedEntityInfoListEntry(IWorldEntity* otherEntity);

	prot void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	prot void disconnectFromGamePartsPre() override;

	pub ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* peekPayloadsList() final;

	pub void addPayload(std::shared_ptr<rpg3D::EffectPayload> payload) final;
	pub void addPayloadsList(ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* payloadsList) final;

	priv void onTargetUnitContactChange(IPhysicsEntityCallbackFilter* filter, IWorldEntity* otherEntity, bool isContact);
	priv void onEnvironmentContactChange(IPhysicsEntityCallbackFilter* filter, IWorldEntity* otherEntity, bool isContact);
	
	priv ImpactedEntityInfo* processTargetEntityImpact(IWorldEntity* otherEntity, bool isSourceReImpactTimer);

	pub ~PMBase() override;
};
};
