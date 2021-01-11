#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <string>
#include <base/exceptions/LogicException.h>
#include "module/IEntityModule.h"
#include <base/map/Map1D.h>
#include <base/math/Matrix4.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <base/list/ListDL.h>
#include "event/PluggableEventsDispatcher.h"
#include <worldGame3D/gw/entity/util/IPositional3DHandler.h>

namespace worldGame3D {
class WE_Base : public ComponentsHandler, virtual public IWorldEntity {priv typedef ComponentsHandler super;pub dCtor(WE_Base);
    prot std::shared_ptr<std::string> instanceId = nullptr;

    // If < 0; Not synced.
    prot int syncedEntityId = -1;

    // This is edited only by GWIEntities.
    // Is setted only by the entitiesHandler.markEntityForRemoval();
    prot bool isMarkedForRemoval = false;
    // This is edited only by GWIEntities.
    prot EntityRemoveReason* markedForRemovalReason = nullptr;

    std::shared_ptr<base::WrappedWorldEntity> wrapper = nullptr;
    sp<base::WrappedWorldEntity> wrapperB = nullptr;

    prot worldGame3D::IPositional3DHandler* positional3D = nullptr;
    prot int positional3DHandlerPriority = -1;
	
    // Affects the root visual2D and physics boxes.
    prot float scale = 1.0f;

    // Value must be initialised when the entity is created.
    // == 0 : no owner;
    prot int ownerPlayerId = 0;

    // This is used for zone speciffic entities to handle cleanup on zone changes.
    prot bool isZoneOwned = false;

    // These are use for fast removal from the contentGroupsList entries.
    prot ArrayList<ConnectedContentGroup> connectedContentGroupsList{};
	
    // Use shared_ptr to avoid large map block memory usage on change.
    prot base::TypedEventDispatcher1D<base::EntityEvent> eventDispatcher{};

    prot Map1D<std::string, std::shared_ptr<base::IWrappedValue>> extraDataMap{};

    pub EntityTemplate* t;

    pub int localEntityId;

    // This is use to extract an entity fully from the gw. It can be re-added later.
    priv IListEntry* inHandler_markedForRemovalListEntry = nullptr;
    pub worldGame3D::IGameWorldGeneral* gw = nullptr;
    pub IEntityHandlerBase* handler = nullptr;

    // Innactive entities are kept in the gw but are not connected to game_parts and gw_listeners are disabled.
    priv int isActiveIncremental = 0;

    priv bool isConnectedToGameParts = false;

    priv worldGame3D::PluggableEventsDispatcher gwEvents{};
    
    //asd_1;// also need a isActive and markEntityForInactivation(...) and activate();
    //asd_1;/// these are use to connect, disconnect to gameParts but always remain in hte gw;

    pub std::shared_ptr<ExtraData> extraData;
    
    // Use void* because the actual class is inside the groups class;
    priv void* inEntitiesGroup_entry = nullptr;
    
    pub explicit WE_Base(EntityTemplate* t, int localEntityId, std::shared_ptr<ExtraData> extraData);

    pub EntityTemplate* getTemplate() override;

    pub std::shared_ptr<std::string> getInstanceId() override;
    pub void setInstanceId(std::shared_ptr<std::string> instanceId) override;

    pub bool getIsMarkedForRemoval() override;
    pub EntityRemoveReason* getMarkedForRemovalReason() override;
    pub void clearIsMarkedForRemoval() override;
    pub virtual void markEntityForRemoval(EntityRemoveReason* removeReason, bool isRequestFromServer, bool disposeAfterRemoval=true) final;
    pub void fromHandler_setIsMarkedForRemoval(EntityRemoveReason* markedForRemovalReason) override;
    pub IListEntry*& getInHandler_markedForRemovalListEntry() final;

    // This is used to get a pointer to this entity that "knows" if the entity exists or was deleted or stuff.
    pub std::shared_ptr<base::WrappedWorldEntity>& getWrapper() override;
    pub sp<base::WrappedWorldEntity>& getWrapperB() override;

    pub base::TypedEventDispatcher1D<base::EntityEvent>* getEventDispatcher() override;
    pub std::shared_ptr<IEventListener> addEventListener(
        int type,
        std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb) override;
    pub void addEventListener(
        int type,
        std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
        ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
    pub void addEventListener(
        int type1, int type2,
        std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
        ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
    pub void addEventListener(
        int type1, int type2, int type3,
        std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
        ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
    pub void addEventListener(
        ArrayList<int>& eventTypesList,
        std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
        ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
    pub int dispatchEvent(base::EntityEvent& event) override;

    pub worldGame3D::IPluggableEventsDispatcher* getGWEvents() final;

    pub void*& getInEntitiesGroup_entryRef() final;

    pub IEntityHandlerBase* getHandler() override;
    pub worldGame3D::IGameWorldGeneral* getGW() override;

    pub bool getIsInGameWorld() final;
    pub void setGameWorld(worldGame3D::IGameWorldGeneral* gw, IEntityHandlerBase* handler) final;

    pub bool getIsActive() final;
    // This must be called outside of the logic loops (eg. call from zone change and similar locations).
    pub void setIsActiveIncremental(bool isActive) final;

    pub bool getIsInGameWorldAndActive() final;

    pub bool hasModule(int moduleId) override;

    pub int getLocalEntityId() override;

    pub worldGame3D::IPositional3DHandler* getPositional3D() final;
    pub void registerPositional3DHandler(worldGame3D::IPositional3DHandler* positional3DHandler, int priority) final;

    // Making this virtual function final may allow it to be inlined (Not sure if true. If false, the function will not be inlined because it is virtual).
    pub inline Vector3* getPos() final;
    pub inline float getX() final;
    pub inline float getY() final;
    pub inline float getZ() final;
    pub void setPos(float x, float y, float z) override;
    pub void setPos(Vector3& pos) override;
    pub void setX(float x) override;
    pub void setY(float y) override;
    pub void setZ(float z) override;

    pub inline Quaternion* getRot() final;
    pub void setRot(Quaternion& rot) override;
    pub void setRot(Vector3& axis, float degrees) override;
    pub void setRotEulerZYX(Vector3& axis, float degrees) override;

    pub Vector3* getScale() override;
    pub void setScale(float scale) override;

    pub int getOwnerPlayerId();
    pub void setOwnerPlayerId(int ownerPlayerId);

    pub bool getIsZoneOwned() final;
    pub void setIsZoneOwned(bool isZoneOwned);

    pub void addToContentGroup(std::shared_ptr<worldGame3D::ContentGroup>& contentGroup) final;
    pub ArrayList<ConnectedContentGroup>& peekConnectedContentGroupsList() final;

    pub int getSyncedEntityId() override;
    pub void setSyncedEntityId(int syncedEntityId);
    pub void registerUninitialisedModule(IEntityModule* mod);
    pub void createB();

    pub std::shared_ptr<base::IWrappedValue> getExtraDataMustExist(std::string& property) override;
    pub std::shared_ptr<base::IWrappedValue> getExtraDataMustExistB(std::string property) override;
    pub std::shared_ptr<base::IWrappedValue> getExtraDataOptional(std::string& property) override;
    pub std::shared_ptr<base::IWrappedValue> getExtraDataOptionalB(std::string property) override;
    
    pub void setExtraData(std::string& property, std::shared_ptr<base::IWrappedValue> value) override;
    pub void setExtraDataB(std::string property, std::shared_ptr<base::IWrappedValue> value) override;

    pub void syncGamePartConnections() final;

    pub void addComponent(IComponent* component, bool doCreate, int disposeOrderDefault5 = 5) override;
    pub void removeComponent(IComponent* component, bool doDispose, bool useDelayedDispose = false) override;

    prot void disposeMainPre() override;
    prot void disposeMain() override;
    pub ~WE_Base() override;
};

// These are placed here in the header because they are inline;
inline Vector3* WE_Base::getPos() {
    if (positional3D == nullptr) {
        throw LogicException(LOC);
    }

    return positional3D->getPos();
}

inline float WE_Base::getX() {
    if (positional3D == nullptr) {
        throw LogicException(LOC);
    }

    return positional3D->getX();
}

inline float WE_Base::getY() {
    if (positional3D == nullptr) {
        throw LogicException(LOC);
    }

    return positional3D->getY();
}

inline float WE_Base::getZ() {
    if (positional3D == nullptr) {
        throw LogicException(LOC);
    }

    return positional3D->getZ();
}

inline Quaternion* WE_Base::getRot() {
    if (positional3D == nullptr) {
        throw LogicException(LOC);
    }

    return positional3D->getRot();
}

};
