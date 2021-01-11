#pragma once

#include <base/gh.h>
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>
#include <string>
#include <memory>
#include <base/list/ArrayList.h>
#include <base/memory/SharedPointer.h>
#include <base/container/flags/ManagedTestFlags.h>

namespace rpg3D {
	class EffectPayload;
	class ILogicBlock;
};

namespace worldGame3D {
class WorldEntitySpawnConfig {pub dCtor(WorldEntitySpawnConfig);
	pub Vector3 pos;
	pub Quaternion rot;

	pub std::string entityTemplateNSpace;
	pub std::string entityTemplateId;

	pub sp<int> team;

	pub std::shared_ptr<ArrayList<std::shared_ptr<rpg3D::EffectPayload>>> impactPayloadsList;
	
	pub rpg3D::ILogicBlock* logicBlock;
	pub void* logicBlockInterfaceRawPtr;

    pub explicit WorldEntitySpawnConfig(
		Vector3& pos, Quaternion& rot, 
		std::string& entityTemplateNSpace, std::string& entityTemplateId,
		sp<int> team,
		std::shared_ptr<ArrayList<std::shared_ptr<rpg3D::EffectPayload>>>& impactPayloadsList,
		rpg3D::ILogicBlock* logicBlock, void* logicBlockInterfaceRawPtr);

    pub virtual ~WorldEntitySpawnConfig();
};
};
