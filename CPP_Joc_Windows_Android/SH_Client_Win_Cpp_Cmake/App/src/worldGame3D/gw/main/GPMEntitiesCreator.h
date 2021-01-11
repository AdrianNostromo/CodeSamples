#pragma once

#include "GPMEntities.h"
#include <worldGame3D/gw/IEntityModuleCreator.h>

class WorldEntity;

namespace base {
class GPMEntitiesCreator : public GPMEntities, virtual public IEntityModuleCreator {
private: typedef GPMEntities super;
private:
    int localEntityIdIncrementor = 0;

public:
    GPMEntitiesCreator(GPMEntitiesCreator const&) = delete;
    GPMEntitiesCreator(GPMEntitiesCreator &&) = default;
    GPMEntitiesCreator& operator=(GPMEntitiesCreator const&) = delete;
    GPMEntitiesCreator& operator=(GPMEntitiesCreator &&) = default;

    explicit GPMEntitiesCreator(base::IGameWorld* gw);

    base::IGameWorld* getGW() override;

    base::TypedEventDispatcher1D<base::GWEvent>* getGWEventDispatcher() final;

    IHandler_GameWorld* getHandler() override {
        return gw->getHandler();
    }

    IContainer3D* getLayerGameWorld3D() override { return super::getLayerGameWorld3D(); }

    IWorldEntity* createEntity(
        EntityTemplate* entityTemplate,
        Vector3& wPos, Quaternion* rot, float* cardinalRotation,
        std::shared_ptr<std::string> instanceId, int ownerPlayerId,
        std::shared_ptr<ExtraData> extraData,
        // Only one of contentGroupsList or creatorEntity may be provided. They serve the same purpose.
        bool isZoneOwnedEntity, ArrayList<std::shared_ptr<worldGame3D::ContentGroup>>* manualContentGroupsList, IWorldEntity* creatorEntity,
        std::function<void(IWorldEntity* entity)> entityPostInitCb,
        bool isActive = true) final;

    ~GPMEntitiesCreator() override;
protected:
    WorldEntity* createEntityObject(EntityTemplate* t, int localEntityId, std::shared_ptr<ExtraData> extraData);
};
};
