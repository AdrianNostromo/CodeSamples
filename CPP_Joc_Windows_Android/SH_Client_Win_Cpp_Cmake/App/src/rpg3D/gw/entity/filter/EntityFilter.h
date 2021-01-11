#pragma once

#include <base/gh.h>
#include "IEntityFilter.h"
#include <functional>
#include <base/math/Vector2.h>
#include <base/list/ArrayList.h>

namespace rpg3D {
class EntityFilter : virtual public IEntityFilter {pub dCtor(EntityFilter);
	priv class InstanceIdsRequirement {pub dCtor(InstanceIdsRequirement);
		pub std::shared_ptr<std::string> instanceId;
		pub sp<Array1D<std::shared_ptr<std::string>>> instanceIdsList;
		
		pub bool mustOrMustNotEqual;

		pub explicit InstanceIdsRequirement(
			std::shared_ptr<std::string> instanceId, sp<Array1D<std::shared_ptr<std::string>>> instanceIdsList,
			bool mustOrMustNotEqual)
			: instanceId(instanceId), instanceIdsList(instanceIdsList), mustOrMustNotEqual(mustOrMustNotEqual)
		{
			//void
		}
	};
	pub int flagsBitMap = 0;
	
	pub ArrayList<EntityTagConfig> entityTagsList{};
	
	// This is for the new int id system.
	pub ArrayList<int> entityModuleRequirementsList{};

	pub ArrayList<InstanceIdsRequirement> instanceIdsRequirementsList{};

	priv std::function<bool(IWorldEntity* entity)> cb_entityCustomTest = nullptr;
	priv std::function<bool(Vector3& worldPos)> cb_worldPosTest = nullptr;

    pub explicit EntityFilter();

	pub IEntityFilter* addFlagsBitMap(int flagsBitMap) final;

	pub IEntityFilter* addEntityTag(std::string& entityTag, bool mustOrMustNotContain) final;

	pub IEntityFilter* addEntityModuleRequirement(int moduleId) final;

	pub IEntityFilter* addInstanceIdCheck(std::shared_ptr<std::string> instanceId, bool mustOrMustNotEqual) final;
	pub IEntityFilter* addInstanceIdCheck(sp<Array1D<std::shared_ptr<std::string>>> instanceIdsList, bool mustOrMustNotEqual) final;

	pub IEntityFilter* setEntityCustomTest(std::function<bool(IWorldEntity* entity)> cb_entityCustomTest) final;
	pub IEntityFilter* setWorldPosCheck(std::function<bool(Vector3& worldPos)> cb_worldPosTest) final;

	pub bool testEntity(IWorldEntity* selfEntity, IWorldEntity* otherEntity) override;

	priv bool testEntityFlags(IWorldEntity* selfEntity, IWorldEntity* otherEntity);
	priv bool testEntityTags(IWorldEntity* selfEntity, IWorldEntity* otherEntity);
	priv bool testEntityModules(IWorldEntity* selfEntity, IWorldEntity* otherEntity);
	priv bool testInstanceIds(IWorldEntity* selfEntity, IWorldEntity* otherEntity);
	priv bool test_entityCustomTest(IWorldEntity* selfEntity, IWorldEntity* otherEntity);
	
	pub bool testWorldPos(IWorldEntity* selfEntity, Vector3& worldPos) final;

    pub ~EntityFilter() override;
};
};
