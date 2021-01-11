#include "GPC_PlayerEntities.h"
#include <base/view/debug/entryUser/IDebugEntryUser.h>
#include <base/app/IApp.h>
#include <base/view/debug/IDebugView.h>
#include <base/math/Math.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/viewsHandler/IViewsHandler.h>
#include <worldGame3D/gw/entityGroups/IGCEntityGroups.h>
#include <worldGame3D/gw/entityGroups/util/IEntsGroup.h>
#include <worldGame3D/gw/entityGroups/event/EntGroupsEvent.h>

using namespace base;

bool GPC_PlayerEntities::IS_DEBUG_VIEW_LOG = true;

GPC_PlayerEntities::GPC_PlayerEntities(IGameWorld* gw)
    : super(gw)
{
    //void
}

void GPC_PlayerEntities::createBMain() {
	super::createBMain();
    
    gpEntsGroups = gw->getComponentAs<worldGame3D::IGCEntityGroups*>(true/*gpGroups*/);
}

IWorldEntity* GPC_PlayerEntities::getSelectedPlayerEntity(bool mustExist) {
    if (mustExist && selectedPlayerEntity == nullptr) {
        throw LogicException(LOC);
    }

    return selectedPlayerEntity;
}

void GPC_PlayerEntities::setSelectManagedPlayerEntity(IWorldEntity* entity) {
    if (this->selectedPlayerEntity == entity) {
        return;
    }

    if (this->selectedPlayerEntity != nullptr) {
        localAutoListenersList_selectedPlayerEntity.clear();

        gw->dispatchEvent(*GWEvent_onPlayerEntityDeselectedPre);

        this->selectedPlayerEntity = nullptr;
    }

    if (entity != nullptr) {
        this->selectedPlayerEntity = entity;

        this->selectedPlayerEntity->addEventListener(
            IWorldEntity::EVENT_syncPos->type,
            std::bind(&GPC_PlayerEntities::selectedPlayerEntity_onSyncPos, this, std::placeholders::_1, std::placeholders::_2),
            localAutoListenersList_selectedPlayerEntity
        );

        gw->dispatchEvent(*GWEvent_onPlayerEntitySelected);
    }

    if (this->selectedPlayerEntity == nullptr) {
        if (dynamicDebugEntry_pos != nullptr && selectedPlayerEntity == nullptr) {
            dynamicDebugEntry_pos->remove();
            dynamicDebugEntry_pos = nullptr;
        }
        if (dynamicDebugEntry_gPos != nullptr && selectedPlayerEntity == nullptr) {
            dynamicDebugEntry_gPos->remove();
            dynamicDebugEntry_gPos = nullptr;
        }
    } else {
        if (IS_DEBUG_VIEW_LOG && gw->getApp()->getComponentAs<base::IViewsHandler*>(true/*mustExist*/)->getDebugView() != nullptr) {
            if (dynamicDebugEntry_pos == nullptr) {
                dynamicDebugEntry_pos = gw->getApp()->getComponentAs<base::IViewsHandler*>(true/*mustExist*/)->getDebugView()->addDebugEntry();

                dynamicDebugEntry_pos->setPlaceholderedText("Player pos x:#0#, y:#1#, z:#2#");
                Vector3* playerEntityPos = selectedPlayerEntity->getPos();
                dynamicDebugEntry_pos->appendTrackedDataValue(dynamicDebugEntry_pos_playerPosX = std::make_shared<base::WrappedValue<float>>(playerEntityPos->x, base::IWrappedValue::Type::T_float));
                dynamicDebugEntry_pos->appendTrackedDataValue(dynamicDebugEntry_pos_playerPosY = std::make_shared<base::WrappedValue<float>>(playerEntityPos->y, base::IWrappedValue::Type::T_float));
                dynamicDebugEntry_pos->appendTrackedDataValue(dynamicDebugEntry_pos_playerPosZ = std::make_shared<base::WrappedValue<float>>(playerEntityPos->z, base::IWrappedValue::Type::T_float));
            }
            if (dynamicDebugEntry_gPos == nullptr) {
                dynamicDebugEntry_gPos = gw->getApp()->getComponentAs<base::IViewsHandler*>(true/*mustExist*/)->getDebugView()->addDebugEntry();

                dynamicDebugEntry_gPos->setPlaceholderedText("Player pos g1X:#0#, g1Y:#1#, g06X:#2#, g06Y:#3#");
                Vector3* playerEntityPos = selectedPlayerEntity->getPos();
                dynamicDebugEntry_gPos->appendTrackedDataValue(dynamicDebugEntry_g1Pos_playerPosX = std::make_shared<base::WrappedValue<float>>(playerEntityPos->x, base::IWrappedValue::Type::T_float));
                dynamicDebugEntry_gPos->appendTrackedDataValue(dynamicDebugEntry_g1Pos_playerPosY = std::make_shared<base::WrappedValue<float>>(playerEntityPos->y, base::IWrappedValue::Type::T_float));
                dynamicDebugEntry_gPos->appendTrackedDataValue(dynamicDebugEntry_g06Pos_playerPosX = std::make_shared<base::WrappedValue<float>>(playerEntityPos->x, base::IWrappedValue::Type::T_float));
                dynamicDebugEntry_gPos->appendTrackedDataValue(dynamicDebugEntry_g06Pos_playerPosY = std::make_shared<base::WrappedValue<float>>(playerEntityPos->y, base::IWrappedValue::Type::T_float));
            }
        }
    }

    gw->dispatchEvent(*GWEvent_onSelectedPlayerEntityChanged);
}

void GPC_PlayerEntities::setSelectManagedPlayerEntity(std::shared_ptr<base::WrappedWorldEntity> wEntity, bool handleBadEntity) {
    if (wEntity == nullptr || wEntity->entity == nullptr || playerEntsGroup == nullptr || !playerEntsGroup->hasEntity(wEntity->entity)) {
        if (!handleBadEntity) {
            throw LogicException(LOC);
        }

        setSelectManagedPlayerEntity(nullptr);
    } else {
        setSelectManagedPlayerEntity(wEntity->entity);
    }
}

worldGame3D::IEntsGroup* GPC_PlayerEntities::getPlayerEntitiesGroup() {
    if (playerEntsGroup == nullptr) {
        if (gpEntsGroups == nullptr) {
            throw LogicException(LOC);
        }

        playerEntsGroup = gpEntsGroups->createEntitiesGroup();
        playerEntsGroup->getEvents().addEventListener(
            worldGame3D::IEntsGroup::EGV_EntityAdded->type, worldGame3D::IEntsGroup::EGV_EntityRemovePre->type, worldGame3D::IEntsGroup::EGV_EntityRemoved->type,
            std::bind(&GPC_PlayerEntities::entsGroup_OnEntitiesCountChanged, this, std::placeholders::_1, std::placeholders::_2),
            localAutoListenersList_entsGroup
        );
    }

    return playerEntsGroup;
}

void GPC_PlayerEntities::entsGroup_OnEntitiesCountChanged(IEventListener& eventListener, worldGame3D::EntGroupsEvent& event) {
    if (event.type == worldGame3D::IEntsGroup::EGV_EntityAdded->type) {
        onPlayerEntityAdded();
    }else if (event.type == worldGame3D::IEntsGroup::EGV_EntityRemovePre->type) {
        onPlayerEntityRemovePre();

        if (selectedPlayerEntity == event.entity) {
            setSelectManagedPlayerEntity(nullptr);
        }
    }else if (event.type == worldGame3D::IEntsGroup::EGV_EntityRemoved->type) {
        onPlayerEntityRemoved();
    } else {
        throw LogicException(LOC);
    }
}

void GPC_PlayerEntities::selectedPlayerEntity_onSyncPos(IEventListener& eventListener, base::EntityEvent& event) {
    if (event.type == IWorldEntity::EVENT_syncPos->type) {
        if (event.entityData != selectedPlayerEntity) {
            return;
        }

        if (dynamicDebugEntry_pos_playerPosX != nullptr) {
            dynamicDebugEntry_pos_playerPosX->getReference() = selectedPlayerEntity->getX();
            dynamicDebugEntry_pos_playerPosY->getReference() = selectedPlayerEntity->getY();
            dynamicDebugEntry_pos_playerPosZ->getReference() = selectedPlayerEntity->getZ();
            dynamicDebugEntry_pos->invalidateData();

            dynamicDebugEntry_g1Pos_playerPosX->getReference() = Math::floor(selectedPlayerEntity->getX());
            dynamicDebugEntry_g1Pos_playerPosY->getReference() = Math::floor(selectedPlayerEntity->getY());
            dynamicDebugEntry_g06Pos_playerPosX->getReference() = Math::floor(selectedPlayerEntity->getX() / 0.6f);
            dynamicDebugEntry_g06Pos_playerPosY->getReference() = Math::floor(selectedPlayerEntity->getY() / 0.6f);
            dynamicDebugEntry_gPos->invalidateData();
        }
    } else {
        throw LogicException(LOC);
    }
}

void GPC_PlayerEntities::onPlayerEntityAdded() {
    //void
}

void GPC_PlayerEntities::onPlayerEntityRemovePre() {
    //void
}

void GPC_PlayerEntities::onPlayerEntityRemoved() {
    //void
}

void GPC_PlayerEntities::disposePre() {
    //void

    super::disposePre();
}

GPC_PlayerEntities::~GPC_PlayerEntities() {
    //void
}
