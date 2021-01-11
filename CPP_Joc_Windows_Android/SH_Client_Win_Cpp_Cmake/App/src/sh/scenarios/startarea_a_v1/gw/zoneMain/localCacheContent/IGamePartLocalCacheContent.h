#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>
#include <memory>
#include <string>

class IWorldEntity;

namespace startarea_a_v1 {
class IGamePartLocalCacheContent : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;

    DEC_componentBasics();
	
	pub virtual int getNewPersistentEntityId() = 0;
	pub virtual void pushPersistentEntity(IWorldEntity* entity, bool isEntityAlreadyCached) = 0;
	pub virtual void persistentEntityTemplateChanged(IWorldEntity* entity) = 0;
	pub virtual void savePendingPersistentEntitiesChanges() = 0;

	pub virtual void loadAllPersistentEntities() = 0;

    pub ~IGamePartLocalCacheContent() override = default;
};
};
