#pragma once

#include <base/gh.h>
#include "TMA_UnitContacts.h"
#include <base/MM.h>

class btRigidBody;
class btCollisionShape;
class btDiscreteDynamicsWorld;
class IContainer3D;
namespace base {
	class WrappedWorldEntity;
};
namespace rpg3D {
	class IPBodyUpdateHandler;
	class PhysicsActor;
	class PhysicsCollissionConfig;
	class IPhysicsEntityCallbackFilter;
};

namespace rpg3D {
class TMA_Physics : public TMA_UnitContacts {priv typedef TMA_UnitContacts super;pub dCtor(TMA_Physics);
	priv class ImpactedEntityInfo {pub dCtor(ImpactedEntityInfo);
		pub std::shared_ptr<base::WrappedWorldEntity> wEntity;
		// This is used so that OnlyFirstContact impacts don't use the reImpactTimerS on later impacts.
		pub int contactsCount = 0;
		pub bool isInContact = false;
		pub float reImpactTimerS = 0;

		pub explicit ImpactedEntityInfo(std::shared_ptr<base::WrappedWorldEntity> wEntity)
			: wEntity(wEntity)
		{}
	};

	prot rpg3D::PhysicsCollissionConfig* pCollisionConfig;

    prot btRigidBody* pBody = nullptr;
	prot btCollisionShape* pShape = nullptr;
	prot IPBodyUpdateHandler* pBodyUpdateHandler = nullptr;
	prot void* ptr_pBodyUpdateHandler = nullptr;

	prot rpg3D::PhysicsActor* physicsActor = nullptr;

	priv IPhysicsEntityCallbackFilter* activeTargetUnitFilter = nullptr;
	priv int targetImpactsCount = 0;

	prot btDiscreteDynamicsWorld* pWorld = nullptr;

	priv ListDL<ImpactedEntityInfo> impactedEntitiesInfosList{LOC};

	pub explicit TMA_Physics(ToolConfigMelleArea* config, std::shared_ptr<ExtraData> extraData);

	prot void onEntityChangePre() override;
	prot void onEntityChanged() override;

	priv ImpactedEntityInfo* getContactedEntityInfo(std::shared_ptr<base::WrappedWorldEntity> wEntity);
	priv ListDL<ImpactedEntityInfo>::Entry* getContactedEntityInfoListEntry(IWorldEntity* otherEntity);

	prot void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	prot void disconnectFromGamePartsPre() override;

	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

	prot void onTriggeringStart() override;

	pub void onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) override;

	priv void onTargetUnitContactChange(IPhysicsEntityCallbackFilter* filter, IWorldEntity* otherEntity, bool isContact);

	priv ImpactedEntityInfo* processTargetEntityImpact(IWorldEntity* otherEntity, bool isSourceReImpactTimer);

	prot void disposePBodyIfAny();

	prot void disposeMain() override;
    pub ~TMA_Physics() override;
};
};
