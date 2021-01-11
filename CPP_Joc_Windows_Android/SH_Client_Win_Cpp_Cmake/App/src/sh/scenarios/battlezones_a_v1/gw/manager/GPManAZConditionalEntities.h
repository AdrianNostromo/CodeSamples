#pragma once

#include "GPManAZBase.h"

class EntityRemoveReason;

namespace battlezones_a_v1 {
class GPManAZConditionalEntities : public GPManAZBase {
private: typedef GPManAZBase super;
private:
	class ConditionalEntitySpawnInfo {
	public:
		ZoneTemplate::EntitySpawnInfo* spawnInfo;

		IWorldEntity* activeEntity = nullptr;

	public:
		ConditionalEntitySpawnInfo(ConditionalEntitySpawnInfo const&) = delete;
		ConditionalEntitySpawnInfo(ConditionalEntitySpawnInfo &&) = default;
		ConditionalEntitySpawnInfo& operator=(ConditionalEntitySpawnInfo const&) = delete;
		ConditionalEntitySpawnInfo& operator=(ConditionalEntitySpawnInfo &&) = default;

		explicit ConditionalEntitySpawnInfo(ZoneTemplate::EntitySpawnInfo* spawnInfo)
			: spawnInfo(spawnInfo) {};

		virtual ~ConditionalEntitySpawnInfo() {};
	};
private:
	static EntityRemoveReason *const ENT_REM_REASON_PURPOSE_FINISHED;

	// These entities are remove/created on zone flags change (example is disable/enabled,nexus/zone teleporters).
	ArrayList<ConditionalEntitySpawnInfo> conditionalEntitiesSpawnInfosList{};

public:
    GPManAZConditionalEntities(GPManAZConditionalEntities const&) = delete;
    GPManAZConditionalEntities(GPManAZConditionalEntities &&) = default;
    GPManAZConditionalEntities& operator=(GPManAZConditionalEntities const&) = delete;
    GPManAZConditionalEntities& operator=(GPManAZConditionalEntities &&) = default;

    explicit GPManAZConditionalEntities(base::IGameWorld* gw);

    ~GPManAZConditionalEntities() override;
protected:
	void addConditionalEntitySpawnInfo(ZoneTemplate::EntitySpawnInfo* spawnInfo);

	void onActiveZoneDeactivate_pre() override;
	void onZoneFlagsChange() override;

};
};
