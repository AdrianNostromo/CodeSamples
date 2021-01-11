#pragma once

#include <base/gh.h>
#include "OGPCE_Base.h"
#include <base/container/TreeAVLKeyValue.h>
#include <string>

class EntityRemoveReason;
namespace base {
	class GWEvent;
	class FlagsEvent;
};
namespace worldGame3D {
	class ConditionalWorldEntitySpawnConfig;
};
namespace rpg3D {
	class IOGPZoneFlags;
};

namespace rpg3D {
class OGPConditionalEntities : public OGPCE_Base {priv typedef OGPCE_Base super;pub dCtor(OGPConditionalEntities);
    priv class ConditionalEntitySpawnInfo {pub dCtor(ConditionalEntitySpawnInfo);
		pub sp<worldGame3D::ConditionalWorldEntitySpawnConfig> spawnInfo;

		pub IWorldEntity* activeEntity = nullptr;

		pub explicit ConditionalEntitySpawnInfo(sp<worldGame3D::ConditionalWorldEntitySpawnConfig> spawnInfo)
			: spawnInfo(spawnInfo) {};

		pub virtual ~ConditionalEntitySpawnInfo() {};
	};
    priv class ListeningFlag {pub dCtor(ListeningFlag);
		pub int useCount = 0;

		pub std::shared_ptr<IEventListener> listenerEntry = nullptr;

		pub explicit ListeningFlag()
		{};

		pub virtual ~ListeningFlag() {};
	};

	priv static EntityRemoveReason *const ENT_REM_REASON_PURPOSE_FINISHED;

	priv IOGPZoneFlags* gpZoneFlags = nullptr;

	// These entities are remove/created on zone flags change (example is disable/enabled,nexus/zone teleporters).
	priv ArrayList<ConditionalEntitySpawnInfo> conditionalEntitiesSpawnInfosList{};

	priv TreeAVLKeyValue<std::string, ListeningFlag> listeningFlagsTree{};

    pub explicit OGPConditionalEntities(base::IGameWorld* gw);
	prot void createBMain() override;

	priv void onGWEvent(IEventListener& eventListener, base::GWEvent& event);
	priv void onZoneFlagEvent(IEventListener& eventListener, base::FlagsEvent& event);

	pub IWorldEntity* addConditionalEntitySpawnInfo(sp<worldGame3D::ConditionalWorldEntitySpawnConfig> spawnInfo) final;

	pub void onZoneFlagsChange(std::string& flagName);
	priv IWorldEntity* handleConditionalEntityChange(ConditionalEntitySpawnInfo* conditionalEntitySpawnInfo);

	prot void disposePre() override;
	pub ~OGPConditionalEntities() override;
};
};
