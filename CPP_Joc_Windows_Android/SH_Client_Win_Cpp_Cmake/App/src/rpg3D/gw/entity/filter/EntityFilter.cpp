#include "EntityFilter.h"
#include <base/exceptions/LogicException.h>
#include <base/math/util/BoolUtil.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>

using namespace rpg3D;

EntityFilter::EntityFilter() {
	//void
}

IEntityFilter* EntityFilter::addFlagsBitMap(int flagsBitMap) {
	this->flagsBitMap = BoolUtil::combine(this->flagsBitMap, flagsBitMap);

	return this;
}

IEntityFilter* EntityFilter::addEntityTag(std::string& entityTag, bool mustOrMustNotContain) {
	entityTagsList.append_emplace(entityTag, mustOrMustNotContain);

	return this;
}

IEntityFilter* EntityFilter::addEntityModuleRequirement(int moduleId) {
	entityModuleRequirementsList.append_emplace(moduleId);

	return this;
}

IEntityFilter* EntityFilter::addInstanceIdCheck(std::shared_ptr<std::string> instanceId, bool mustOrMustNotEqual) {
	instanceIdsRequirementsList.append_emplace(instanceId, nullptr/*instanceIdsList*/, mustOrMustNotEqual);

	return this;
}

IEntityFilter* EntityFilter::addInstanceIdCheck(sp<Array1D<std::shared_ptr<std::string>>> instanceIdsList, bool mustOrMustNotEqual) {
	instanceIdsRequirementsList.append_emplace(nullptr/*instanceId*/, instanceIdsList, mustOrMustNotEqual);

	return this;
}

IEntityFilter* EntityFilter::setEntityCustomTest(std::function<bool(IWorldEntity* entity)> cb_entityCustomTest) {
	this->cb_entityCustomTest = cb_entityCustomTest;

	return this;
}

IEntityFilter* EntityFilter::setWorldPosCheck(std::function<bool(Vector3& worldPos)> cb_worldPosTest) {
	this->cb_worldPosTest = cb_worldPosTest;

	return this;
}

bool EntityFilter::testEntity(IWorldEntity* selfEntity, IWorldEntity* otherEntity) {
	if (!testEntityFlags(selfEntity, otherEntity) 
		|| !testEntityTags(selfEntity, otherEntity) 
		|| !testEntityModules(selfEntity, otherEntity)
		|| !testInstanceIds(selfEntity, otherEntity)
		|| !test_entityCustomTest(selfEntity, otherEntity)
		|| !testWorldPos(selfEntity, *otherEntity->getPos()))
	{
		return false;
	}

	return true;
}

bool EntityFilter::testEntityFlags(IWorldEntity* selfEntity, IWorldEntity* otherEntity) {
	// Do this test in case I forget to add new below tests on new flags.
	if (Flags::LAST_FLAG_ID != Flags::Alive) {
		throw LogicException(LOC);
	}

	if (BoolUtil::isBitAtIndex(flagsBitMap, Flags::NotSelf)) {
		if (selfEntity == otherEntity) {
			return false;
		}
	}
	if (BoolUtil::isBitAtIndex(flagsBitMap, Flags::HasInstanceId)) {
		if (otherEntity->getInstanceId() == nullptr) {
			return false;
		}
	}
	if (BoolUtil::isBitAtIndex(flagsBitMap, Flags::Team_Same)) {
		// Get team modules and test team index.
		rpg3D::ITeamModule* selfEntity_team = selfEntity->getComponentAs<rpg3D::ITeamModule*>(false/*mustExist*/);
		rpg3D::ITeamModule* otherEntity_team = otherEntity->getComponentAs<rpg3D::ITeamModule*>(false/*mustExist*/);

		if (selfEntity_team == nullptr || selfEntity_team->getTeamIndex() <= 0) {
			// Flags::Team_Same required the self entity to have a team module and a valid positive team.
			throw LogicException(LOC);
		}

		if (otherEntity_team == nullptr || selfEntity_team->getTeamIndex() != otherEntity_team->getTeamIndex()) {
			return false;
		}
	}
	if (BoolUtil::isBitAtIndex(flagsBitMap, Flags::Team_Different)) {
		// Get team modules and test team index.
		rpg3D::ITeamModule* selfEntity_team = selfEntity->getComponentAs<rpg3D::ITeamModule*>(false/*mustExist*/);
		rpg3D::ITeamModule* otherEntity_team = otherEntity->getComponentAs<rpg3D::ITeamModule*>(false/*mustExist*/);

		if (selfEntity_team == nullptr || selfEntity_team->getTeamIndex() <= 0) {
			// Flags::Team_Same required the self entity to have a team module and a valid positive team.
			throw LogicException(LOC);
		}

		if (otherEntity_team == nullptr || selfEntity_team->getTeamIndex() == otherEntity_team->getTeamIndex()) {
			return false;
		}
	}
	if (BoolUtil::isBitAtIndex(flagsBitMap, Flags::Team_Same_Or_NoTeamModule)) {
		// Get team modules and test team index.
		rpg3D::ITeamModule* selfEntity_team = selfEntity->getComponentAs<rpg3D::ITeamModule*>(false/*mustExist*/);
		rpg3D::ITeamModule* otherEntity_team = otherEntity->getComponentAs<rpg3D::ITeamModule*>(false/*mustExist*/);

		if (selfEntity_team == nullptr || selfEntity_team->getTeamIndex() <= 0) {
			// Flags::Team_Same required the self entity to have a team module and a valid positive team.
			throw LogicException(LOC);
		}

		if (otherEntity_team != nullptr && selfEntity_team->getTeamIndex() != otherEntity_team->getTeamIndex()) {
			return false;
		}
	}
	if (BoolUtil::isBitAtIndex(flagsBitMap, Flags::Alive)) {
		// Must have a living entity module and currentHp>0;

		rpg3D::ILivingEntityModule* otherEntity_living = otherEntity->getComponentAs<rpg3D::ILivingEntityModule*>(false/*mustExist*/);

		if (otherEntity_living == nullptr || otherEntity_living->getCurrentHP() <= 0.0f) {
			return false;
		}
	}

	return true;
}

bool EntityFilter::testEntityTags(IWorldEntity* selfEntity, IWorldEntity* otherEntity) {
	for (int i = 0; i < entityTagsList.count(); i++) {
		EntityTagConfig* entityTagConfig = entityTagsList.getPointer(i);

		bool hasTag = otherEntity->getTemplate()->hasTag(entityTagConfig->tag);

		if (hasTag != entityTagConfig->mustOrMustNotContain) {
			return false;
		}
	}

	return true;
}

bool EntityFilter::testEntityModules(IWorldEntity* selfEntity, IWorldEntity* otherEntity) {
	for (int i = 0; i < entityModuleRequirementsList.count(); i++) {
		int entityModuleId = entityModuleRequirementsList.getDirect(i);

		bool hasModule = otherEntity->hasComponent(entityModuleId);

		if (!hasModule) {
			return false;
		}
	}

	return true;
}

bool EntityFilter::testInstanceIds(IWorldEntity* selfEntity, IWorldEntity* otherEntity) {
	for (int i = 0; i < instanceIdsRequirementsList.count(); i++) {
		InstanceIdsRequirement& entityModuleId = instanceIdsRequirementsList.getReference(i);

		if (entityModuleId.instanceId != nullptr) {
			if (entityModuleId.instanceIdsList != nullptr) {
				throw LogicException(LOC);
			}

			bool isEqual = otherEntity->getInstanceId() == entityModuleId.instanceId;
			if (isEqual != entityModuleId.mustOrMustNotEqual) {
				return false;
			}
		} else if (entityModuleId.instanceIdsList != nullptr) {
			bool isCheckFailed = false;

			int count = entityModuleId.instanceIdsList->count();
			for (int i = 0; i < count; i++) {
				bool isEqual = otherEntity->getInstanceId() == entityModuleId.instanceIdsList->getReference(i);

				if (isEqual != entityModuleId.mustOrMustNotEqual) {
					isCheckFailed = true;

					break;
				}
			}
			
			if (isCheckFailed) {
				return false;
			}
		} else {
			throw LogicException(LOC);
		}
	}

	return true;
}

bool EntityFilter::test_entityCustomTest(IWorldEntity* selfEntity, IWorldEntity* otherEntity) {
	if (cb_entityCustomTest) {
		bool isCheckPassed = cb_entityCustomTest(otherEntity);
		if (!isCheckPassed) {
			return false;
		}
	}

	return true;
}

bool EntityFilter::testWorldPos(IWorldEntity* selfEntity, Vector3& worldPos) {
	if (cb_worldPosTest) {
		bool isCheckPassed = cb_worldPosTest(worldPos);
		if (!isCheckPassed) {
			return false;
		}
	}

	return true;
}

EntityFilter::~EntityFilter() {
	//void
}
