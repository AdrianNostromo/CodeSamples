#pragma once

#include <base/gh.h>
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>
#include <string>
#include <memory>
#include <base/list/ArrayList.h>
#include <base/memory/SharedPointer.h>
#include <base/container/flags/ManagedTestFlags.h>

namespace worldGame3D {
class ConditionalWorldEntitySpawnConfig {pub dCtor(ConditionalWorldEntitySpawnConfig);
	pub std::string entityTemplateNSpace;
	pub std::string entityTemplateId;

	pub Vector3 pos;
	pub Quaternion rot;

	std::shared_ptr<std::string> instanceId;

	// These flags need to pass against the zone_flags in order for this entity to be spawned.
	// The test re-occurs on zone_flags change.
	base::ManagedTestFlags spawnTestFlags{};

    pub explicit ConditionalWorldEntitySpawnConfig(
		std::string& scenarioNamespace, std::string& id,
		Vector3& pos, Quaternion& rot,
		std::shared_ptr<std::string> instanceId);

    pub virtual ~ConditionalWorldEntitySpawnConfig();
};
};
