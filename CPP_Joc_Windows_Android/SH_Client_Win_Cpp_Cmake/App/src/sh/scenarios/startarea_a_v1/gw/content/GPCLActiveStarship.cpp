#include "GPCLActiveStarship.h"
#include <worldGame3D/gw/main/IGamePartMain.h>

using namespace startarea_a_v1;

GPCLActiveStarship::GPCLActiveStarship(base::IGameWorld* gw)
    : super(gw)
{
    gw->addEventListener(
        base::IGamePartMain::GWEvent_onEntityRemoved_Pre->type,
        std::bind(&GPCLActiveStarship::onEvent, this, std::placeholders::_1, std::placeholders::_2),
        autoListenersList
    );
}

void GPCLActiveStarship::createBMain() {
    super::createBMain();

    //void
}

IWorldEntity* GPCLActiveStarship::getActiveStarship() {
    return activeStarship;
}

void GPCLActiveStarship::onEvent(IEventListener& eventListener, base::GWEvent& event) {
    if(event.type == base::IGamePartMain::GWEvent_onEntityRemoved_Pre->type) {
        onEntityRemoved_Pre(event.getEntityData());
    }else {
        throw LogicException(LOC);
    }
}

void GPCLActiveStarship::onEntityRemoved_Pre(IWorldEntity* entity) {
    if(entity == activeStarship) {
        setActiveStarship(nullptr);
    }
}

void GPCLActiveStarship::setActiveStarship(IWorldEntity* activeStarship) {
    if(this->activeStarship == activeStarship) {
        return ;
    }

    if(this->activeStarship != nullptr) {
        //void

        this->activeStarship = nullptr;
    }

    this->activeStarship = activeStarship;
    if(this->activeStarship != nullptr) {
        //void
    }

    gw->dispatchEvent(*GWEvent_onActiveStarshipChanged);
}

GPCLActiveStarship::~GPCLActiveStarship() {
    //void
}
