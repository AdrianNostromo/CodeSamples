#include "ContentZoneCommon.h"
#include <worldGame3D/gw/IGameWorld.h>
#include "../gw/view/mainui/MainInGameUIView.h"
#include "util/ZoneStartExtraData.h"
#include <sh/scenarios/randDungeons_a_v1/gw/playerZeroHpHandling/IGPPlayerZeroHpHandling.h>
#include "../../zoneMain/config/ContentZoneMain.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::common;

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

	std::shared_ptr<std::string> zeroHpZoneName;
	ZoneStartExtraData* cZoneStartExtraData;
	std::shared_ptr< std::string> zeroHpZonePlayerPosEntityId;
	if (zoneStartExtraData != nullptr && (cZoneStartExtraData = zoneStartExtraData->getAs<ZoneStartExtraData*>())->zeroHpZoneName != nullptr) {
		//cZoneStartExtraData->zeroHpZonePlayerPosEntityId can be nullptr and use the default pos then.
		if (cZoneStartExtraData->zeroHpZoneName == nullptr) {
			throw LogicException(LOC);
		}

		zeroHpZoneName = cZoneStartExtraData->zeroHpZoneName;
		zeroHpZonePlayerPosEntityId = cZoneStartExtraData->zeroHpZonePlayerPosEntityId;
	} else {
		zeroHpZoneName = std::make_shared<std::string>(Main::ContentZoneMain::NAME);
		zeroHpZonePlayerPosEntityId = nullptr;
	}

	gw->getComponentAs<IGPPlayerZeroHpHandling*>(true/*mustExist*/)->setData(
		zeroHpZoneName, zeroHpZonePlayerPosEntityId,
		false/*playerDestroyed_restartScenario*/, false/*playerDestroyed_restartZoneIfActive*/
	);
}

void ContentZoneCommon::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	//void
}

ContentZoneCommon::~ContentZoneCommon() {
	//void
}
