#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/main/util/IScreenTouch2DListener.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <worldGame3D/gw/main/util/IEntitiesFilter.h>
#include <worldGame3D/gw/event/GWEvent.h>
#include <base/math/collision/Ray.h>
#include <base/list/ArrayList.h>
#include <base/math/Vector2.h>

class IContainer2D;
class IWorldEntity;
class IContainer3D;
class Stage3D;
class EntityTemplate;
class ExtraData;
class EntityRemoveReason;
namespace worldGame3D {
	class ContentGroup;
};
namespace graphics {
	class Environment3D;
	class ICinematicController;
	class IRenderer;
};

namespace base {
class IGamePartMain {
	pub class DT_Entity_Distance {pub dCtor(DT_Entity_Distance);
		pub IWorldEntity* entity;
		pub float dist;

		pub explicit DT_Entity_Distance(IWorldEntity* entity, float dist) : entity(entity), dist(dist) {};

		pub virtual ~DT_Entity_Distance() {};
	};

	pub static GWEvent* GWEvent_onEntityAdded;
	pub static GWEvent* GWEvent_onEntityRemoved_Pre;

	pub virtual Stage3D* getStage3D() = 0;
	pub virtual graphics::IRenderer* getRenderer3D() = 0;
	pub virtual graphics::Environment3D* getEnvironment3D() = 0;

	pub virtual IContainer3D* getLayerGameWorld3D() = 0;

	pub virtual std::shared_ptr<graphics::ICinematicController> getCC() = 0;

	pub virtual std::shared_ptr<ArrayList<DT_Entity_Distance>> getEntitiesUnderTouch(Ray& ray, IEntitiesFilter* entityFilter) = 0;
	pub virtual std::shared_ptr<ArrayList<DT_Entity_Distance>> getEntitiesUnderTouch(Ray& ray) = 0;

	pub virtual bool camera2DCoordsProjectionTo3DGroundPos(Vector2& screenPos, float groundPlanePosZ, Vector3* oIntersectionPos, Vector3* oRayDirection) = 0;
	pub virtual void camera2DCoordsProjectionGetRayDir(Vector2& screenPos, Vector3* oRayDirection) = 0;

	pub virtual IContainer2D* getLayerSubUserInterface() = 0;
	pub virtual IContainer2D* getLayerMainUserInterface() = 0;
	
	pub virtual void setScreenTouch2DListener(IScreenTouch2DListener* screenTouch2DListener) = 0;

	// isZoneOwnedEntity; Used for zones entities cleanup.
	pub virtual IWorldEntity* createEntity(
        EntityTemplate* entityTemplate,
        Vector3& wPos, Quaternion* rot, float* cardinalRotation,
        std::shared_ptr<std::string> instanceId, int ownerPlayerId,
        std::shared_ptr<ExtraData> extraData,
		// Only one of contentGroupsList or creatorEntity may be provided. They serve the same purpose.
		bool isZoneOwnedEntity, ArrayList<std::shared_ptr<worldGame3D::ContentGroup>>* manualContentGroupsList, IWorldEntity* creatorEntity,
        std::function<void(IWorldEntity* entity)> entityPostInitCb,
		bool isActive = true) = 0;

	pub virtual void markEntityForRemoval(
		IWorldEntity* entity,
		EntityRemoveReason* removeReason, bool isRequestFromServer, bool disposeAfterRemoval=true) = 0;

	pub virtual IWorldEntity* getEntityWithLocalEntityId(int localEntityId) = 0;

	pub virtual void removeAllZoneOwnedEntities() = 0;

	pub virtual ArrayList<IWorldEntity*>* peekEntitiesList() = 0;

	pub virtual IWorldEntity* getEntityWithInstanceId(std::string& instanceId, bool mustExist=false) = 0;
	pub virtual bool isAnyEntityWithInstanceId(std::string& instanceId) = 0;
	pub virtual std::shared_ptr<ArrayList<IWorldEntity*>> getEntitiesListWithInstanceId(std::string& instanceId) = 0;

	pub virtual void connectAllEntitiesToGameParts() = 0;
	pub virtual void disconnectAllEntitiesFromGameParts() = 0;

	pub virtual ~IGamePartMain() = default;
};
};
