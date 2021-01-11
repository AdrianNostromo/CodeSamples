#include "GPPlayerZeroHpHandling.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <sh/scenarios/gridAdventure_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/controller/ControllerModule.h>
#include <sh/scenarios/gridAdventure_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>
#include <sh/scenarios/gridAdventure_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <base/util/StringUtil.h>
#include <worldGame3D/gw/contentZones/IGamePartContentZones.h>
#include <rpg3D/gw/manager/IGPManager.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/content/IGPContent.h>
#include <sh/scenarios/gridAdventure_a_v1/gw/zoneCommon/config/util/ZoneStartExtraData.h>
#include <sh/scenarios/gridAdventure_a_v1/gw/zoneMain/config/ContentZoneMain.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>
#include <worldGame3D/scripts/ScriptCommands.h>
#include <worldGame3D/gw/handler/GWEventDataBase.h>

using namespace gridAdventure_a_v1;

GPPlayerZeroHpHandling::GPPlayerZeroHpHandling(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		rpg3D::IGPContent::GWEvent_onSelectedPlayerEntityChanged->type,
		std::bind(&GPPlayerZeroHpHandling::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPPlayerZeroHpHandling::setData(
	std::shared_ptr<std::string> playerDestroyedTeleportZoneName, std::shared_ptr<std::string> playerDestroyedTeleportPlayerPosEntityId,
	bool playerDestroyed_restartScenario, bool playerDestroyed_restartZoneIfActive)
{
	this->playerDestroyedTeleportZoneName = playerDestroyedTeleportZoneName;
	this->playerDestroyedTeleportPlayerPosEntityId = playerDestroyedTeleportPlayerPosEntityId;

	this->playerDestroyed_restartScenario = playerDestroyed_restartScenario;
	this->playerDestroyed_restartZoneIfActive = playerDestroyed_restartZoneIfActive;
}

void GPPlayerZeroHpHandling::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == rpg3D::IGPContent::GWEvent_onSelectedPlayerEntityChanged->type) {
		playerEventsListenersList.clear();

		IWorldEntity* playerEntity = gw->getContent()->getSelectedPlayerEntity(true/*mustExist*/);
		if (playerEntity != nullptr) {
			playerEntity->addEventListener(
				rpg3D::LivingEntityModule::EVENT_livingEntity_ZeroHp->type,
				std::bind(&GPPlayerZeroHpHandling::onPlayerEntityEvent, this, std::placeholders::_1, std::placeholders::_2),
				autoListenersList
			);
		}
	} else {
		throw LogicException(LOC);
	}
}

void GPPlayerZeroHpHandling::onPlayerEntityEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::LivingEntityModule::EVENT_livingEntity_ZeroHp->type) {
		// Exit the dungeon.
		// playerDestroyedTeleportPlayerPosEntityId may be nullptr and use default then.
		if (playerDestroyedTeleportZoneName == nullptr) {
			throw LogicException(LOC);
		}
		
		worldGame3D::IGamePartContentZones* gpContentZones = gw->getComponentAs<worldGame3D::IGamePartContentZones*>(true/*mustExist*/);
		
		std::shared_ptr<common::ZoneStartExtraData> zoneStartExtraData(new common::ZoneStartExtraData(
			playerDestroyedTeleportPlayerPosEntityId, nullptr/*teleportTargetOffset*/,
			nullptr/*zeroHpZoneName*/, nullptr/*zeroHpZonePlayerPosEntityId*/
		));

		if (playerDestroyed_restartScenario) {
			std::shared_ptr<std::string> command = std::make_shared<std::string>(
				worldGame3D::ScriptCommands::cmd_setScenarioAndZone + worldGame3D::ScriptCommands::commandPartsDelimiter
				+ worldGame3D::ScriptCommands::sameScenario + worldGame3D::ScriptCommands::commandPartsDelimiter
				+ worldGame3D::ScriptCommands::restart
			);

			std::shared_ptr<base::GWEventDataBase> data = std::make_shared<base::GWEventDataBase>();
			data->paramSPS = command;

			gw->getHandler()->onGWEvent(
				base::IGameWorld::GWE_RequestRunScript,
				data
			);
		} else {
			gpContentZones->setActiveZone(
				*playerDestroyedTeleportZoneName, zoneStartExtraData, playerDestroyed_restartZoneIfActive/*restartZoneIfActive*/,
				true/*useDelayedOnLoopActivation*/
			);
		}
	} else {
		throw LogicException(LOC);
	}
}

GPPlayerZeroHpHandling::~GPPlayerZeroHpHandling() {
	//void
}
