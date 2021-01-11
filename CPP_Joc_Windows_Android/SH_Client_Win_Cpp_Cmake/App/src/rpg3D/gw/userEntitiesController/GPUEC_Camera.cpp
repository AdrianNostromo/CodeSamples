#include "GPUEC_Camera.h"
#include <rpg3D/gw/interaction/deviceinput/IManagedDeviceInput.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <rpg3D/gw/entity/module/userControlled/util/IWorldOrEntityInteractionFilter.h>
#include <graphics/camera/Camera.h>
#include <worldGame3D/gw/IGameWorld.h>

using namespace rpg3D;

GPUEC_Camera::GPUEC_Camera(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void GPUEC_Camera::onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_localInput(deltaS, deltaMS, gameTimeS);

	Vector3 const* newCamDir = gw->getMain()->getEnvironment3D()->peekCamera()->getDirection();
	if (isDirtyControlType || lastDispatched_cameraDir == nullptr || !lastDispatched_cameraDir->equals(newCamDir)) {
		dispatch_CameraDirection(*newCamDir);
	}
}

void GPUEC_Camera::dispatch_CameraDirection(const Vector3& camDir) {
	if (lastDispatched_cameraDir == nullptr) {
		lastDispatched_cameraDir = std::make_shared<Vector3>(camDir);
	}else {
		lastDispatched_cameraDir->set(camDir);
	}

	if (controledEntityTarget != nullptr && controledEntityTarget->userControlledModule != nullptr) {
		base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::CameraDirection, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_CameraDirection(-1, *lastDispatched_cameraDir),
				dispatcher, IWorldOrEntityInteractionFilter::Type::CameraDirection
			);
		}
	}
}

void GPUEC_Camera::clearInput() {
	super::clearInput();

	lastDispatched_cameraDir = nullptr;
}

GPUEC_Camera::~GPUEC_Camera() {
	//void
}
