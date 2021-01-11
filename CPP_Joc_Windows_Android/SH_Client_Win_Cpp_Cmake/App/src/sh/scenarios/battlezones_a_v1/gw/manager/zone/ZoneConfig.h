#pragma once

#include "TeleporterTarget.h"
#include <base/map/Map1D.h>
#include <memory>
#include <base/math/Vector3.h>
#include <base/list/ArrayList.h>
#include <base/container/flags/ManagedFlags.h>
#include <worldGame3D/gw/event/GWEvent.h>
#include <rpg3D/gw/projectile/EffectPayload.h>

namespace battlezones_a_v1 {
class ZoneConfig {
public:
	class UnitSpawnConfig {
	public:
		Vector3 pos;
		Quaternion rot;
		
		std::string entityTemplateNSpace;
		std::string entityTemplateId;

		ArrayList<std::shared_ptr<rpg3D::EffectPayload>> impactPayloadsList{};

	public:
		UnitSpawnConfig(UnitSpawnConfig const&) = delete;
		UnitSpawnConfig(UnitSpawnConfig &&) = default;
		UnitSpawnConfig& operator=(UnitSpawnConfig const&) = delete;
		UnitSpawnConfig& operator=(UnitSpawnConfig &&) = default;

		explicit UnitSpawnConfig(Vector3& pos, Quaternion& rot, std::string& entityTemplateNSpace, std::string& entityTemplateId)
			: pos(pos), rot(rot), entityTemplateNSpace(entityTemplateNSpace), entityTemplateId(entityTemplateId) {}
		
		virtual ~UnitSpawnConfig() {}
	};
public:
	std::string groupId;
	std::string zoneName;

	std::string zoneTemplateId;

	base::ManagedFlags flags{};

	// The last zone of the battle zones_group contains a GWEvent_battleZonesGroupFinished
	// This can contain the same event multiple times because the events that are added to this list don't contain any custom data, just the type is used.
	ArrayList<base::GWEvent*> zoneClearedEventsInstancePointers{};

private:
	Map1D<std::string, std::shared_ptr<TeleporterTarget>> teleporterTargetsMap{};

	ArrayList<UnitSpawnConfig> mobsSpawnsList{};

public:
    ZoneConfig(ZoneConfig const&) = delete;
    ZoneConfig(ZoneConfig &&) = default;
    ZoneConfig& operator=(ZoneConfig const&) = delete;
    ZoneConfig& operator=(ZoneConfig &&) = default;

    explicit ZoneConfig(std::string& groupId, std::string& zoneName);

	ArrayList<UnitSpawnConfig>* getMobsSpawnsList();
	UnitSpawnConfig* addUnitSpawnConfig(Vector3& pos, Quaternion& rot, std::string& entityTemplateNSpace, std::string& entityTemplateId);

	TeleporterTarget* getTeleporterTarget(std::string& teleporterEntityId);
	void addTeleporterTarget(
		std::string& teleporterEntityId,
		std::string& zonesGroupId, std::string zoneName, std::shared_ptr<std::string>& spawnLocationEntityId);

    virtual ~ZoneConfig();
};
};
