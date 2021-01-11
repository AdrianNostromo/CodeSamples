#include "GPER_ActiveRoomDetector.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <worldGame3D/gw/content/IGPContent.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

GPER_ActiveRoomDetector::GPER_ActiveRoomDetector(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_contentCreation->type,
		std::bind(&GPER_ActiveRoomDetector::onGWEvent_gameLoop_contentCreation, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPER_ActiveRoomDetector::createBMain() {
	super::createBMain();

	playerEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/);
	if (playerEntity != nullptr) {
		isDirty_playerPos = true;

		playerEntity->addEventListener(
			IWorldEntity::EVENT_syncPos->type,
			std::bind(&GPER_ActiveRoomDetector::onPlayerEntityEvent, this, std::placeholders::_1, std::placeholders::_2),
			playerEntityAutoListenersList
		);
	}

	gw->addEventListener(
		base::IGPContent::GWEvent_onPlayerEntitySelected->type, base::IGPContent::GWEvent_onPlayerEntityDeselectedPre->type,
		std::bind(&GPER_ActiveRoomDetector::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPER_ActiveRoomDetector::onPlayerEntityEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == IWorldEntity::EVENT_syncPos->type) {
		isDirty_playerPos = true;
	} else {
		throw LogicException(LOC);
	}
}

void GPER_ActiveRoomDetector::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == base::IGPContent::GWEvent_onPlayerEntitySelected->type) {
		playerEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/);
		isDirty_playerPos = true;

		playerEntity->addEventListener(
			IWorldEntity::EVENT_syncPos->type,
			std::bind(&GPER_ActiveRoomDetector::onPlayerEntityEvent, this, std::placeholders::_1, std::placeholders::_2),
			playerEntityAutoListenersList
		);
	} else if (event.type == base::IGPContent::GWEvent_onPlayerEntityDeselectedPre->type) {
		playerEntityAutoListenersList.clear();
		playerEntity = nullptr;
	} else {
		throw LogicException(LOC);
	}
}

void GPER_ActiveRoomDetector::onGWEvent_gameLoop_contentCreation(IEventListener& eventListener, base::GWEvent& event) {
	if (isDirty_playerPos) {
		isDirty_playerPos = false;

		if (playerEntity != nullptr) {
			Vector3* pos = playerEntity->getPos();
			Vector2Int levelZoneTilesGPos{
				(int)Math::floor(pos->x / levelZoneConfig->tileSizeM),
				(int)Math::floor(pos->y / levelZoneConfig->tileSizeM)
			};

			int roomIndex = levelZoneRoomIndexesGrid->getDirect(levelZoneTilesGPos.x, levelZoneTilesGPos.y, -1);

			if (roomIndex >= 0) {
				Room& room = roomsList.getReference(roomIndex);

				// Check if out of the current room and activate the new one if changed.
				// Also must handle already active rooms activation.
				if (roomIndex != getCurrentPlayerRoomIndex()) {
					activateRoom(room, true/*alsoCreateDoorNeighbourRooms*/);
				}
			}

			currentPlayerRoomIndex = roomIndex;
		}
	}
}

GPER_ActiveRoomDetector::~GPER_ActiveRoomDetector() {
	//void
}
