#include <base/math/util/BoolUtil.h>
#include "CustomLogicModule_PaintPad.h"
#include <worldGame3D/gw/entity/module/visual3D/IVisual3DModule.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <base/exceptions/LogicException.h>
#include <base/math/Math.h>
#include <base/ease/Ease.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <graphics/visual3d/drawable/IDrawableNode3D.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/event/IPluggableEventsDispatcher.h>

using namespace rpg3D;

CustomLogicModule_PaintPad::CustomLogicModule_PaintPad(
    IWorldEntity* entity,
	base::CustomLogicTemplate* t,
    std::string brushId, std::string brushHolderId
)
    : super(entity, t),
      brushId(brushId), brushHolderId(brushHolderId)
{
    e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_simulation->type,
		std::bind(&CustomLogicModule_PaintPad::onGWEvent_gameLoop_simulation, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

    e->addEventListener(
		PlatformModule::EVENT_Platform_activeStateChanged->type,
        std::bind(&CustomLogicModule_PaintPad::onEvent, this, std::placeholders::_1, std::placeholders::_2),
        autoListenersList
    );
}

void CustomLogicModule_PaintPad::createBMain() {
    super::createBMain();

    platform = entity->getComponentAs<IPlatformModule*>(true/*mustExist*/);

    currentActiveStatus = platform->getIsActive();
    isStateDirty = true;

    worldGame3D::IVisual3DModule* visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);

    brush = visual3D->getDrawableOptionalAs<IDrawableNode3D*>(brushId);
    brushHolder = visual3D->getDrawableOptionalAs<IDrawableNode3D*>(brushHolderId);
    if(brush == nullptr || brushHolder == nullptr) {
        throw LogicException(LOC);
    }

    brushAngZ = brushHolder->getRotation()->getAngleAround(Vector3::Z);
}

void CustomLogicModule_PaintPad::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
    if (event.type == PlatformModule::EVENT_Platform_activeStateChanged->type) {
        currentActiveStatus = platform->getIsActive();
    }else {
        throw LogicException(LOC);
    }
}

void CustomLogicModule_PaintPad::onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event) {
    float newBrushAngZ = Math::normalizeAngleDeg(brushAngZ + 90.0f * event.deltaS);
    float newBrushAngX = brushAngX;

    if(!currentActiveStatus) {
        if(brushAngX < 60.0f) {
            newBrushAngX = Math::min(60.0f, brushAngX + 30.0f * event.deltaS);
        }
    }else {
        if(brushAngX > 10.0f) {
            newBrushAngX = Math::max(10.0f, brushAngX - 30.0f * event.deltaS);
        }

        if(brushAngZ <= 0.0f && newBrushAngZ > 0.0f) {
            // Lock the angle to 0 degrees.
            newBrushAngZ = 0.0f;
        }
    }

    if(brushAngZ != newBrushAngZ || isStateDirty) {
        brushAngZ = newBrushAngZ;

        brushHolder->setRotation(Vector3::Z, brushAngZ);
    }

    if(brushAngX != newBrushAngX || isStateDirty) {
        brushAngX = newBrushAngX;

        brush->setRotation(Vector3::X, brushAngX);
    }

    isStateDirty = false;
}

CustomLogicModule_PaintPad::~CustomLogicModule_PaintPad() {
    //void
}
