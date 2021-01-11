#include "GPPLH_Base.h"
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>
#include <rpg3D/gw/ui/IGamePartUserInterface.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/ui/GPUIMainUIView.h>
#include <sh/scenarios/startarea_a_v1/gw/config/ScenarioFactory.h>
#include <memory>
#include <worldGame3D/gw/handler/GWEventDataBase.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

GPPLH_Base::GPPLH_Base(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

std::shared_ptr< RDungeonA::DungeonContent> GPPLH_Base::getDungeonContent() {
	return dungeonContent;
}

std::shared_ptr<std::string> GPPLH_Base::getExitZoneName() {
	return exitZoneName;
}

std::shared_ptr<std::string> GPPLH_Base::getExitZonePlayerPosEntityId() {
	return exitZonePlayerPosEntityId;
}

std::shared_ptr<std::string> GPPLH_Base::getZeroHpZoneName() {
	return zeroHpZoneName;
}

std::shared_ptr<std::string> GPPLH_Base::getZeroHpZonePlayerPosEntityId() {
	return zeroHpZonePlayerPosEntityId;
}

void GPPLH_Base::setData(
	std::shared_ptr< RDungeonA::DungeonContent> dungeonContent,
	std::shared_ptr<std::string> exitZoneName, std::shared_ptr<std::string> exitZonePlayerPosEntityId,
	std::shared_ptr<std::string> zeroHpZoneName, std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId)
{
	this->dungeonContent = dungeonContent;

	this->exitZoneName = exitZoneName;
	this->exitZonePlayerPosEntityId = exitZonePlayerPosEntityId;

	this->zeroHpZoneName = zeroHpZoneName;
	this->zeroHpZonePlayerPosEntityId = zeroHpZonePlayerPosEntityId;
}

GPPLH_Base::~GPPLH_Base() {
	//void
}
