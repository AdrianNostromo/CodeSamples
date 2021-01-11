#pragma once

#include <string>
#include <base/map/Map1D.h>
#include <base/list/ArrayList.h>
#include <base/list/Array3D.h>
#include <base/visual3d/material/Visual3DModelMaterial.h>
#include <base/container/flags/ManagedTestFlags.h>

namespace battlezones_a_v1 {
class ZoneTemplate {
public:
	static Map1D<std::string, ZoneTemplate*> entriesMap;

public:
	class GroundPatchSpawnInfo {
	public:
		std::string scenarioNamespace;
		std::string id;

		Vector3 pos;

		Quaternion rot;

		Array3D<signed char>* voxelsGrid;
		ArrayList<Visual3DModelMaterial*>* materialsList;

	public:
		GroundPatchSpawnInfo(GroundPatchSpawnInfo const&) = delete;
		GroundPatchSpawnInfo(GroundPatchSpawnInfo &&) = default;
		GroundPatchSpawnInfo& operator=(GroundPatchSpawnInfo const&) = delete;
		GroundPatchSpawnInfo& operator=(GroundPatchSpawnInfo &&) = default;

		explicit GroundPatchSpawnInfo(std::string& scenarioNamespace, std::string& id, Vector3& pos, Quaternion& rot, Array3D<signed char>* voxelsGrid, ArrayList<Visual3DModelMaterial*>* materialsList);

		virtual ~GroundPatchSpawnInfo();
	};
	class EntitySpawnInfo {
	public:
		std::string scenarioNamespace;
		std::string id;

		Vector3 pos;

		Quaternion rot;

		std::shared_ptr<std::string> instanceId;

		// These flags need to pass against the zone_flags in order for this entity to be spawned.
		// The test re-occurs on zone_flags change.
		base::ManagedTestFlags spawnTestFlags{};

	public:
		EntitySpawnInfo(EntitySpawnInfo const&) = delete;
		EntitySpawnInfo(EntitySpawnInfo &&) = default;
		EntitySpawnInfo& operator=(EntitySpawnInfo const&) = delete;
		EntitySpawnInfo& operator=(EntitySpawnInfo &&) = default;

		explicit EntitySpawnInfo(std::string& scenarioNamespace, std::string& id, Vector3& pos, Quaternion& rot, std::shared_ptr<std::string> instanceId);

		virtual ~EntitySpawnInfo();
	};

public:
	std::string id;

	ArrayList<GroundPatchSpawnInfo> groundPatchesList{};
	ArrayList<EntitySpawnInfo> environmentEntitiesList{};

public:
	ZoneTemplate(ZoneTemplate const&) = delete;
	ZoneTemplate(ZoneTemplate &&) = default;
	ZoneTemplate& operator=(ZoneTemplate const&) = delete;
	ZoneTemplate& operator=(ZoneTemplate &&) = default;

	explicit ZoneTemplate(std::string& id);

	EntitySpawnInfo* getEnvironmentEntityWithInstanceId(std::shared_ptr<std::string>& instanceId);

	virtual ~ZoneTemplate();
};
};
