#include "FrustumCullable3DModule.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

FrustumCullable3DModule::FrustumCullable3DModule(
    IWorldEntity* entity,
    FrustumCullable3DTemplate* t
)
    :super(t->moduleName, entity),
     t(t)
{
    e->addEventListener(
        IWorldEntity::EVENT_syncPos->type,
        std::bind(&FrustumCullable3DModule::onEvent, this, std::placeholders::_1, std::placeholders::_2),
        autoListenersList
    );
}

void FrustumCullable3DModule::createBMain() {
    super::createBMain();

    //void
}

FrustumCullable3DTemplate* FrustumCullable3DModule::getT() {
    return t;
}

FrustumCullable3DTemplate* FrustumCullable3DModule::getTemplate() {
    return t;
}

void FrustumCullable3DModule::setFrustumCulling3DHandler(IFrustumCullable3DHandler* frustumCulling3DHandler) {
    if((this->frustumCulling3DHandler == nullptr && frustumCulling3DHandler == nullptr) ||
        (this->frustumCulling3DHandler != nullptr && frustumCulling3DHandler != nullptr))
    {
        throw LogicException(LOC);
    }

    // No need to invalidate the frustum here when setting it from a non-null value to a null value.

    this->frustumCulling3DHandler = frustumCulling3DHandler;
    if(this->frustumCulling3DHandler != nullptr) {
        this->frustumCulling3DHandler->onFrustumCullable3DInvalid(this);
    }
}

bool FrustumCullable3DModule::getIsVisible() {
    return isVisible;
}

void FrustumCullable3DModule::setIsVisible(bool isVisible) {
    if(this->isVisible == isVisible) {
        return ;
    }

    this->isVisible = isVisible;
    visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
    visual3D->setIsVisibleIncremental(this->isVisible);
}

void FrustumCullable3DModule::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
    if(event.type == IWorldEntity::EVENT_syncPos->type) {
        if(frustumCulling3DHandler != nullptr) {
            frustumCulling3DHandler->onFrustumCullable3DInvalid(this);
        }
    }else {
        throw LogicException(LOC);
    }
}

FrustumCullable3DModule::~FrustumCullable3DModule() {
    //void
}
