#pragma once

#include <base/gh.h>
#include <string>
#include <base/sp.h>
#include <base/list/Array1D.h>
#include <functional>
#include <base/math/Vector3.h>

class IWorldEntity;

namespace rpg3D {
class IEntityFilter {
    pub class Flags {
		pub static const int NotSelf;
		pub static const int HasInstanceId;
		pub static const int Team_Same;
		pub static const int Team_Different;
		pub static const int Team_Same_Or_NoTeamModule;
		// Has living module and currentHp>0.0f;
		pub static const int Alive;

		pub static const int LAST_FLAG_ID;
	};
	pub class EntityTagConfig {pub dCtor(EntityTagConfig);
		pub std::string tag;

		pub bool mustOrMustNotContain;
		
		pub explicit EntityTagConfig(std::string& tag, bool mustOrMustNotContain)
			: tag(tag), mustOrMustNotContain(mustOrMustNotContain) {};

		pub virtual ~EntityTagConfig() {};
	};

	pub virtual IEntityFilter* addFlagsBitMap(int flagsBitMap) = 0;
	
	pub virtual IEntityFilter* addEntityTag(std::string& entityTag, bool mustOrMustNotContain) = 0;
	
	pub virtual IEntityFilter* addEntityModuleRequirement(int moduleId) = 0;
	
	pub virtual IEntityFilter* addInstanceIdCheck(std::shared_ptr<std::string> instanceId, bool mustOrMustNotEqual) = 0;
	pub virtual IEntityFilter* addInstanceIdCheck(sp<Array1D<std::shared_ptr<std::string>>> instanceIdsList, bool mustOrMustNotEqual) = 0;

	pub virtual IEntityFilter* setEntityCustomTest(std::function<bool(IWorldEntity* entity)> cb_entityCustomTest) = 0;
	pub virtual IEntityFilter* setWorldPosCheck(std::function<bool(Vector3& worldPos)> cb_worldPosTest) = 0;

	pub virtual bool testEntity(IWorldEntity* selfEntity, IWorldEntity* otherEntity) = 0;
	pub virtual bool testWorldPos(IWorldEntity* selfEntity, Vector3& worldPos) = 0;

    pub virtual ~IEntityFilter() = default;
};
};
