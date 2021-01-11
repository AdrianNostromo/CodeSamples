#include "GPER_ActveRooms.h"
#include <worldGame3D/gw/entity/util/ContentGroup.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::PrefabRoomsDungeonA;

GPER_ActveRooms::GPER_ActveRooms(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void GPER_ActveRooms::preActivateRoomIfInnactive(int roomIndex) {
	Room& room = roomsList.getReference(roomIndex);

	activateRoom(room, true/*alsoCreateDoorNeighbourRooms*/);
}

void GPER_ActveRooms::activateRoom(Room& room, bool alsoCreateDoorNeighbourRooms) {
	if (!room.isContentCreated) {
		createRoomContent(room, true/*isContentEnabled*/);
	}else if (!room.isContentEnabled) {
		room.isContentEnabled = true;

		for (int i = 0; i < room.contentGroupsList.count(); i++) {
			std::shared_ptr<worldGame3D::ContentGroup>& cGroup = room.contentGroupsList.getReference(i);

			cGroup->setAllEntitiesEnabledState(true);
		}
	}

	if (alsoCreateDoorNeighbourRooms) {
		for (int i = 0; i < room.doorsList.count(); i++) {
			Room::Door& door = room.doorsList.getReference(i);

			activateRoom(*door.otherSideRoom, false/*alsoCreateDoorNeighbourRooms*/);
		}
	}
}

GPER_ActveRooms::~GPER_ActveRooms() {
	//void
}
