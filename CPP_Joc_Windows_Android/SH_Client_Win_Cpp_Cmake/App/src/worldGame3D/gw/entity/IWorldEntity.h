#pragma once

#include <base/gh.h>
#include <base/component/IComponentsHandler.h>
#include <string>
#include <memory>
#include <functional>
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include "util/EntityRemoveReason.h"
#include <base/list/ArrayList.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>
#include <base/event/listener/IEventListener.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include <base/exceptions/LogicException.h>
#include <base/exceptions/NotFoundException.h>
#include <worldGame3D/gw/entity/event/IPluggableEventsDispatcher.h>
#include <worldGame3D/gw/entity/util/WrappedWorldEntity.h>
#include <base/sp.h>

class EntityRemoveReason;
class IEntityHandlerBase;
class IListEntry;
namespace base {
	class WrappedWorldEntity;
};
namespace worldGame3D {
	class IGameWorldGeneral;
	class IPluggableEventsDispatcher;
	class IPositional3DHandler;
	class ContentGroup;
	class IGameWorldGameParts;
};

class IWorldEntity : virtual public IComponentsHandler {
	pub class ConnectedContentGroup {
		pub std::shared_ptr<worldGame3D::ContentGroup> contentGroup;
		pub IListEntry* entryInContentGroup;

		pub explicit ConnectedContentGroup(std::shared_ptr<worldGame3D::ContentGroup> contentGroup, IListEntry* entryInContentGroup)
			: contentGroup(contentGroup), entryInContentGroup(entryInContentGroup)
		{}
	};

	pub static EntityRemoveReason* const ENT_REM_REASON_PURPOSE_FINISHED;

	pub static base::EntityEvent* EVENT_syncPos;
	pub static base::EntityEvent* EVENT_posManuallyChanged;
	pub static base::EntityEvent* EVENT_syncRot;
	pub static base::EntityEvent* EVENT_rotManuallyChanged;
	pub static base::EntityEvent* EVENT_syncCardinalRotation;
	pub static base::EntityEvent* EVENT_syncScale;
	
	pub static base::EntityEvent* EVENT_onRemoved_pre;

	// Note. This must be manually disposed.
	pub virtual void*& getInEntitiesGroup_entryRef() = 0;

	pub virtual IEntityHandlerBase* getHandler() = 0;
	pub virtual worldGame3D::IGameWorldGeneral* getGW() = 0;
	pub virtual worldGame3D::IPluggableEventsDispatcher* getGWEvents() = 0;

	pub virtual bool getIsInGameWorld() = 0;
	pub virtual void setGameWorld(worldGame3D::IGameWorldGeneral* gw, IEntityHandlerBase* handler) = 0;

	pub virtual bool getIsActive() = 0;
	// This must be called outside of the logic loops (eg. call from zone change and similar locations).
	pub virtual void setIsActiveIncremental(bool isActive) = 0;

	pub virtual bool getIsInGameWorldAndActive() = 0;

	pub virtual EntityTemplate* getTemplate() = 0;

	pub virtual int getLocalEntityId() = 0;

	pub virtual worldGame3D::IPositional3DHandler* getPositional3D() = 0;
	// priority is used if multiple modules can handle the positional (eg. visuals3D_priority_1 vs physics3D_priority_2).
	pub virtual void registerPositional3DHandler(worldGame3D::IPositional3DHandler* positional3DHandler, int priority) = 0;

	pub virtual Vector3* getPos() = 0;
	pub virtual float getX() = 0;
	pub virtual float getY() = 0;
	pub virtual float getZ() = 0;
	pub virtual void setPos(float x, float y, float z) = 0;
	pub virtual void setPos(Vector3& pos) = 0;
	pub virtual void setX(float x) = 0;
	pub virtual void setY(float y) = 0;
	pub virtual void setZ(float z) = 0;

	pub virtual Quaternion* getRot() = 0;
	pub virtual void setRot(Quaternion& rot) = 0;
	pub virtual void setRot(Vector3& axis, float degrees) = 0;
	pub virtual void setRotEulerZYX(Vector3& axis, float degrees) = 0;

	pub virtual Vector3* getScale() = 0;
	pub virtual void setScale(float scale) = 0;

	// This has a similar purpose to contentGroups but this makes sure that player_entity projectiles are removed on zone changed.
	pub virtual bool getIsZoneOwned() = 0;

	pub virtual void addToContentGroup(std::shared_ptr<worldGame3D::ContentGroup>& contentGroup) = 0;
	pub virtual ArrayList<ConnectedContentGroup>& peekConnectedContentGroupsList() = 0;
	
	pub virtual int getSyncedEntityId() = 0;
	
	pub virtual bool hasModule(int moduleId) = 0;

	pub virtual std::shared_ptr<std::string> getInstanceId() = 0;
	pub virtual void setInstanceId(std::shared_ptr<std::string> instanceId) = 0;

	pub virtual bool getIsMarkedForRemoval() = 0;
	pub virtual EntityRemoveReason* getMarkedForRemovalReason() = 0;
	pub virtual void clearIsMarkedForRemoval() = 0;
	pub virtual void markEntityForRemoval(EntityRemoveReason* removeReason, bool isRequestFromServer, bool disposeAfterRemoval=true) = 0;
	pub virtual void fromHandler_setIsMarkedForRemoval(EntityRemoveReason* markedForRemovalReason) = 0;
	pub virtual IListEntry*& getInHandler_markedForRemovalListEntry() = 0;

	// This is used to get a pointer to this entity that "knows" if the entity exists or was deleted or stuff.
	pub virtual std::shared_ptr<base::WrappedWorldEntity>& getWrapper() = 0;
	pub virtual sp<base::WrappedWorldEntity>& getWrapperB() = 0;

	pub virtual base::TypedEventDispatcher1D<base::EntityEvent>* getEventDispatcher() = 0;
	pub virtual std::shared_ptr<IEventListener> addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb) = 0;
	pub virtual void addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		ArrayList<int>& eventTypesList,
		std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual int dispatchEvent(base::EntityEvent& event) = 0;

	pub virtual std::shared_ptr<base::IWrappedValue> getExtraDataMustExist(std::string& property) = 0;
	pub virtual std::shared_ptr<base::IWrappedValue> getExtraDataMustExistB(std::string property) = 0;
	pub virtual std::shared_ptr<base::IWrappedValue> getExtraDataOptional(std::string& property) = 0;
	pub virtual std::shared_ptr<base::IWrappedValue> getExtraDataOptionalB(std::string property) = 0;

	pub virtual void setExtraData(std::string& property, std::shared_ptr<base::IWrappedValue> value) = 0;
	pub virtual void setExtraDataB(std::string property, std::shared_ptr<base::IWrappedValue> value) = 0;

	pub virtual void syncGamePartConnections() = 0;

	pub virtual ~IWorldEntity() = default;
};
