#include "ContentZoneCommon.h"
#include <worldGame3D/gw/IGameWorld.h>
#include "../gw/view/mainui/MainInGameUIView.h"
#include "util/ZoneStartExtraData.h"
#include <sh/scenarios/roomgeons_a_v1/gw/playerZeroHpHandling/IGPPlayerZeroHpHandling.h>
#include "../../zoneArenaRoom/config/ContentZoneArenaRoom.h"

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::common;

ContentZoneCommon::ContentZoneCommon(std::string& id)
	: super(id)
{
	//void
}

int ContentZoneCommon::getMainUIViewID() {
	return IMainInGameUIView::ID;
}

rpg3D::MainInGameUIView* ContentZoneCommon::getNewMainUIViewUninitialised(base::IGameWorld* gw) {
	MainInGameUIView* ret = new MainInGameUIView(gw->getHandler()->getApp(), gw);

	return ret;
}

void ContentZoneCommon::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);

	// This is the default.
	std::shared_ptr<std::string> zeroHpZoneName = std::make_shared<std::string>(ArenaRoom::ContentZoneArenaRoom::NAME);
	// Use nullptr an the zone will select a default.
	std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId = nullptr;
	bool playerDestroyed_restartScenario = true;

	gw->getComponentAs<IGPPlayerZeroHpHandling*>(true/*mustExist*/)->setData(
		zeroHpZoneName, zeroHpZonePlayerPosEntityId,
		playerDestroyed_restartScenario/*playerDestroyed_restartScenario*/, false/*playerDestroyed_restartZoneIfActive*/
	);
}

void ContentZoneCommon::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	//void
}

ContentZoneCommon::~ContentZoneCommon() {
	//void
}
