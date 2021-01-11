#include <base/app/config/AppConfig.h>
#include <base/log/ILogHandler.h>
#include <base/log/GlobalAccessLogHandler.h>
#include "GPSFrustumCulling3D.h"
#include <graphics/environment/environment3D/Environment3D.h>
#include <graphics/camera/Camera.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace base;

const bool GPSFrustumCulling3D::IS_DEBUG_FRUSTUM_CULLING_3D = false;

GPSFrustumCulling3D::GPSFrustumCulling3D(IGameWorld* gw)
    : super(gw)
{
    gw->addEventListener(
        IGamePartMain::GWEvent_onEntityAdded->type, IGamePartMain::GWEvent_onEntityRemoved_Pre->type,
        std::bind(&GPSFrustumCulling3D::onEvent, this, std::placeholders::_1, std::placeholders::_2),
        autoListenersList
    );

	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_frustumCulling3DUpdate->type,
		std::bind(&GPSFrustumCulling3D::onGWEvent_gameLoop_frustumCulling3DUpdate, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPSFrustumCulling3D::createBMain() {
    super::createBMain();

    //void
}

void GPSFrustumCulling3D::onFrustumCullable3DInvalid(IFrustumCullable3DModule* target) {
    invalidFrustumCullables3DList.appendReference(target);
}

void GPSFrustumCulling3D::onEvent(IEventListener& eventListener, GWEvent& event) {
    if(event.type == IGamePartMain::GWEvent_onEntityAdded->type) {
        onEntityAdded(event.getEntityData());
    }else if(event.type == IGamePartMain::GWEvent_onEntityRemoved_Pre->type) {
        onEntityRemoved_Pre(event.getEntityData());
    }else {
        throw LogicException(LOC);
    }
}

void GPSFrustumCulling3D::onEntityAdded(IWorldEntity* entity) {
    IFrustumCullable3DModule* frustumCullable3D = entity->getComponentAs<IFrustumCullable3DModule*>(false/*mustExist*/);
    if(frustumCullable3D != nullptr) {
        addCullable3D(frustumCullable3D);
    }
}

void GPSFrustumCulling3D::onEntityRemoved_Pre(IWorldEntity* entity) {
    IFrustumCullable3DModule* frustumCullable3D = entity->getComponentAs<IFrustumCullable3DModule*>(false/*mustExist*/);
    if(frustumCullable3D != nullptr) {
        removeCullable3D(frustumCullable3D);
    }
}

void GPSFrustumCulling3D::addCullable3D(IFrustumCullable3DModule* frustumCullable3D) {
    frustumCullables3DList.appendReference(frustumCullable3D);

    if(!frustumCullable3D->getIsVisible()) {
        hiddenCullables++;
    }else {
        visibleCullables++;
    }

    frustumCullable3D->setFrustumCulling3DHandler(this);

    if(AppConfig::IS_DEBUG && IS_DEBUG_FRUSTUM_CULLING_3D) {
        logCullables();
    }
}

void GPSFrustumCulling3D::removeCullable3D(IFrustumCullable3DModule* frustumCullable3D) {
    int i = frustumCullables3DList.indexOf(frustumCullable3D);
    if(i < 0) {
        throw LogicException(LOC);
    }
    frustumCullables3DList.remove(i);

    if(!frustumCullable3D->getIsVisible()) {
        hiddenCullables--;
    }else {
        visibleCullables--;
    }
    if(!frustumCullable3D->getIsVisible()) {
        frustumCullable3D->setIsVisible(true);
    }
    frustumCullable3D->setFrustumCulling3DHandler(nullptr);

    if(AppConfig::IS_DEBUG && IS_DEBUG_FRUSTUM_CULLING_3D) {
        logCullables();
    }
}

void GPSFrustumCulling3D::onGWEvent_gameLoop_frustumCulling3DUpdate(IEventListener& eventListener, base::GWEvent& event) {
    //asdA9_t;
    return ;
	/*
    int prev_visibleCullables = visibleCullables;
    int prev_hiddenCullables = hiddenCullables;
    bool updateOccurred = false;
    if(gw->getGPMain()->getEnvironment3D()->getCameraUpdateCounter() > lastCheckedCameraUpdateCounter) {
        lastCheckedCameraUpdateCounter = gw->getGPMain()->getEnvironment3D()->getCameraUpdateCounter();

        updateCullables(&frustumCullables3DList);

        updateOccurred = true;
    }else if(invalidFrustumCullables3DList.size() > 0) {
        updateCullables(&invalidFrustumCullables3DList);

        updateOccurred = true;
    }

    if(updateOccurred && (prev_visibleCullables != visibleCullables || prev_hiddenCullables != hiddenCullables)) {
        if(AppConfig::IS_DEBUG && IS_DEBUG_FRUSTUM_CULLING_3D) {
            logCullables();
        }
    }

    if(invalidFrustumCullables3DList.size() > 0) {
        invalidFrustumCullables3DList.clear();
    }*/
}

void GPSFrustumCulling3D::updateCullables(ArrayList<IFrustumCullable3DModule*>* cullablesList) {
    for(int i=0;i<cullablesList->size();i++) {
        IFrustumCullable3DModule* frustumCullable3D = cullablesList->getDirect(i);

        bool isVisible;
        if(frustumCullable3D->getT()->shape == FrustumCullable3DTemplate::SHAPE_SPHERE) {
            isVisible = gw->getGPMain()->getEnvironment3D()->peekCamera()->getFrustum()->sphereInFrustum(
                frustumCullable3D->getEntity()->getX(),
                frustumCullable3D->getEntity()->getY(),
                frustumCullable3D->getEntity()->getZ() + frustumCullable3D->getT()->sphere_offsetZ,
                frustumCullable3D->getT()->sphere_radiusM
            );
        }else {
            // Implement extra shapes when needed.
            // "Err. Shape not implemented yet.";
            throw LogicException(LOC);
        }

        if(isVisible != frustumCullable3D->getIsVisible()) {
            if (!isVisible) {
                hiddenCullables++;
                visibleCullables--;
            } else {
                hiddenCullables--;
                visibleCullables++;
            }
            frustumCullable3D->setIsVisible(isVisible);
        }
    }
}

void GPSFrustumCulling3D::logCullables() {
    GlobalAccessLogHandler::devLogHandler->postDebug(std::string("Visual 3d frustum culling, visible: ") + std::to_string(visibleCullables) + ", hidden: " + std::to_string(hiddenCullables) + ".");
}

GPSFrustumCulling3D::~GPSFrustumCulling3D() {
    //void
}
