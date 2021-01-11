#include "GPManPlatforms.h"
#include <rpg3D/gw/entity/module/platformInteractor/PlatformInteractorModule.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <rpg3D/gw/config/entity/EntityIds.h>
#include <base/util/StringUtil.h>
#include <memory>
#include <worldGame3D/gw/handler/GWEventDataBase.h>
#include <rpg3D/gw/ui/GPUIMainUIView.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

GPManPlatforms::GPManPlatforms(base::IGameWorld* gw)
    : super(gw)
{
	gw->addEntityBubbledEventListener(
		rpg3D::PlatformModule::EVENT_onPlatformContactAdded->type,
		std::bind(&GPManPlatforms::onEntityEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_post->type,
		std::bind(&GPManPlatforms::onGWEvent_gameLoop_post, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPManPlatforms::onEntityEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if(event.type == rpg3D::PlatformModule::EVENT_onPlatformContactAdded->type) {
		IWorldEntity *contactedEntity = event.getEntityDataOther();

		if (gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/) == contactedEntity) {
			if (framesToIgnorePlatformActivations <= 0 ||
			    !playerSpawnIgnoredPlatformsActivation.containsDirect(event.entityData->getInstanceId()))
			{
				if (!isPlatformsActivationEnabled) {
					// This is disabled when the zone editor is active.
					return;
				}

				IPlatformModule* platform = event.entityData->getComponentAs<IPlatformModule*>(true/*mustExist*/);
				if(platform->getActivation_script() != nullptr) {
					std::shared_ptr<base::GWEventDataBase> data = std::make_shared<base::GWEventDataBase>();
					data->paramSPS = platform->getActivation_script();
					data->sourceTeleporterOffset.set(contactedEntity->getPos())
						->sub(event.entityData->getPos());

					gw->getHandler()->onGWEvent(
						base::IGameWorld::GWE_RequestRunScript,
						data
					);
				} else {
					gw->dispatchEvent(*GWEvent_onPlayerActivatedPlatformNonSpawn->setEntityData(event.entityData));
				}
			}
		}
	}else {
		throw LogicException(LOC);
	}
}

void GPManPlatforms::addPlayerSpawnIgnoredPlatformsActivation(std::shared_ptr<std::string> platformInstanceId) {
	playerSpawnIgnoredPlatformsActivation.appendReference(platformInstanceId);

	framesToIgnorePlatformActivations = 2;
}

void GPManPlatforms::setIsPlatformsActivationEnabled(bool isPlatformsActivationEnabled) {
	if (this->isPlatformsActivationEnabled == isPlatformsActivationEnabled) {
		// This must never occur, a bug may have occured.
		throw LogicException(LOC);
	}

	this->isPlatformsActivationEnabled = isPlatformsActivationEnabled;
}

void GPManPlatforms::onGWEvent_gameLoop_post(IEventListener& eventListener, base::GWEvent& event) {
	if(framesToIgnorePlatformActivations > 0) {
		framesToIgnorePlatformActivations--;

		if(framesToIgnorePlatformActivations == 0) {
			playerSpawnIgnoredPlatformsActivation.clear();
		}
	}
}

void GPManPlatforms::disposeMain() {
	playerSpawnIgnoredPlatformsActivation.clear();

	super::disposeMain();
}

GPManPlatforms::~GPManPlatforms() {
    //void
}
