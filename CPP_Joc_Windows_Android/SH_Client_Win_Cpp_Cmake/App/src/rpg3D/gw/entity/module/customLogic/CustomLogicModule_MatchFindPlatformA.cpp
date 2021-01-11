#include <base/math/util/BoolUtil.h>
#include "CustomLogicModule_MatchFindPlatformA.h"
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <base/exceptions/LogicException.h>
#include <base/math/Math.h>
#include <base/ease/Ease.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/event/IPluggableEventsDispatcher.h>

using namespace rpg3D;


const float CustomLogicModule_MatchFindPlatformA::z_IDDLE = 0.05f;
const float CustomLogicModule_MatchFindPlatformA::z_ACTIVE = 1.0f;

const float CustomLogicModule_MatchFindPlatformA::rotDeg_IDDLE = 0.0f;
const float CustomLogicModule_MatchFindPlatformA::rotDeg_ACTIVE = 90.0f;
const Vector3 CustomLogicModule_MatchFindPlatformA::rotAxis{1.0f, -1.0f, 0.0f, true};

CustomLogicModule_MatchFindPlatformA::CustomLogicModule_MatchFindPlatformA(
    IWorldEntity* entity,
	base::CustomLogicTemplate* t,
    std::string spinnerId, std::string spinnerSymbolId
)
    : super(entity, t),
    spinnerId(spinnerId), spinnerSymbolId(spinnerSymbolId)
{
    e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_simulation->type,
		std::bind(&CustomLogicModule_MatchFindPlatformA::onGWEvent_gameLoop_simulation, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

    e->addEventListener(
        PlatformModule::EVENT_Platform_activeStateChanged->type,
        std::bind(&CustomLogicModule_MatchFindPlatformA::onEvent, this, std::placeholders::_1, std::placeholders::_2),
        autoListenersList
    );
}

void CustomLogicModule_MatchFindPlatformA::createBMain() {
    super::createBMain();

    platform = entity->getComponentAs<IPlatformModule*>(true/*mustExist*/);

    nextActiveState = platform->getIsActive();
    isStateDirty = true;

    worldGame3D::IVisual3DModule* visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);

    spinner = visual3D->getDrawableOptionalAs<IDrawableNode3D*>(spinnerId);
    spinnerSymbol = visual3D->getDrawableOptionalAs<IDrawableNode3D*>(spinnerSymbolId);
    if(spinner == nullptr || spinnerSymbol == nullptr) {
        throw LogicException(LOC);
    }
    spinner->setPosition(
        0.0f,
        0.0f,
        0.05f
    );
    spinner->setRotation(Vector3::Z, -0.0f);

    spinnerSymbol->setRotation(Vector3::X, 0.0f);
}

void CustomLogicModule_MatchFindPlatformA::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
    if (event.type == PlatformModule::EVENT_Platform_activeStateChanged->type) {
        nextActiveState = platform->getIsActive();
    }else {
        throw LogicException(LOC);
    }
}

void CustomLogicModule_MatchFindPlatformA::onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event) {
    if(currentActiveStatus != nextActiveState) {
        currentActiveStatus = nextActiveState;
        animationCompletionStatus = 1.0f - animationCompletionStatus;
    }

    if(animationCompletionStatus < 1.0f || isStateDirty) {
        isStateDirty = false;

        float animSpeedMultiplier;
        const std::string* easeFunctionName;
        float z_start;
        float z_end;
        float rotXDeg_start;
        float rotXDeg_end;

        if(!currentActiveStatus) {
            animSpeedMultiplier = 1.0f;
            easeFunctionName = &Ease::easeInSine;

            z_start = z_ACTIVE;
            z_end = z_IDDLE;

            rotXDeg_start = rotDeg_ACTIVE;
            rotXDeg_end = rotDeg_IDDLE;
        }else {
            animSpeedMultiplier = 3.0f;
            easeFunctionName = &Ease::easeOutSine;

            z_start = z_IDDLE;
            z_end = z_ACTIVE;

            rotXDeg_start = rotDeg_IDDLE;
            rotXDeg_end = rotDeg_ACTIVE;
        }

        animationCompletionStatus = Math::min(1.0f, animationCompletionStatus + animSpeedMultiplier * event.deltaS);

        float newPosZ = Ease::computeEaseValue(
            *easeFunctionName,
            nullptr,
            animationCompletionStatus,
            z_start,
            z_end
        );
        spinner->setZ(newPosZ);

        float newRotX = Ease::computeEaseValue(
            *easeFunctionName,
            nullptr,
            animationCompletionStatus,
            rotXDeg_start,
            rotXDeg_end
        );
        spinnerSymbol->setRotation(rotAxis, newRotX);
    }

    if(currentActiveStatus || animationCompletionStatus < 1.0f) {
        spinner->rotate(
            Vector3::Z,
			event.deltaS * (-180.0f * (1.0f / 0.8f))
        );
    }
}

CustomLogicModule_MatchFindPlatformA::~CustomLogicModule_MatchFindPlatformA() {
    //void
}
