#pragma once

#include <base/gh.h>
#include "IEntsGroup.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/list/ListDL.h>

class IEventListener;
namespace base {
    class EntityEvent;
};

namespace worldGame3D {
class EntsGroup : public virtual IEntsGroup {pub dCtor(EntsGroup);
    priv class ManagedEntityData {pub dCtor(ManagedEntityData);
		pub IWorldEntity* entity;

		pub IListEntry* listEntry = nullptr;

		pub ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

		pub explicit ManagedEntityData(IWorldEntity* entity)
			: entity(entity)
		{
			//void
		}
	};
	
    // This is also the index in the holder list.
    pub int uid;

    pub ListDL<ManagedEntityData> entitiesList{};

    prot base::TypedEventDispatcher1D<EntGroupsEvent> events{};

    pub explicit EntsGroup(int uid);

    pub base::TypedEventDispatcher1D<EntGroupsEvent>& getEvents() final;

    pub int getUid() final;

    pub int getEntitiesCount() final;
    pub bool hasEntity(IWorldEntity* entity) final;
    pub void addEntity(IWorldEntity* entity) final;
    pub void removeEntity(IWorldEntity* entity, bool disposeTheEntity) final;
    pub void disposeAllEntities() final;

    priv void entity_onRemoved_Pre(IEventListener& eventListener, base::EntityEvent& event);

    pub ~EntsGroup() override;
};
};
