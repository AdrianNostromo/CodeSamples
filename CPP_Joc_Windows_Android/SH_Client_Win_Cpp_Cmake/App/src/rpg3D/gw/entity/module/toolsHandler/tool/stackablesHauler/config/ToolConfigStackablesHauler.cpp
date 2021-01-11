#include "ToolConfigStackablesHauler.h"
#include <memory>
#include <base/list/Array1D.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/stackablesHauler/ToolStackablesHauler.h>
#include <base/assets/IAssetsManager.h>
#include <base/app/IAppAssets.h>

using namespace rpg3D;

std::string ToolConfigStackablesHauler::TAG_stackablesHauler = "TAG_stackablesHauler";

ToolConfigStackablesHauler::ToolConfigStackablesHauler(
	std::string id,
	int filtersPriority,
	std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
	std::string shortName_partA, std::string shortName_partB,
	float cooldownS, float channelingDurationS,
	float pickupRangeM, float throwRangeM,
	std::string* activationSoundConfig_id, float activationSoundConfig_volume,
	float throwPosXYGridSize, float throwAngDegZGridSize)
	: super(
		id, std::make_shared<Array1D<std::string>>(1, new std::string[1]{ TAG_stackablesHauler }), 
		filtersPriority, false/*isUserSelectable*/, true/*isDirectional*/, true/*supportsContinuousReactivation*/, channelingDurationS,
		icon_atlasId, icon_regionId,
		shortName_partA, shortName_partB),
	cooldownS(cooldownS),
	pickupRangeM(pickupRangeM), throwRangeM(throwRangeM),
	activationSoundConfig_id(activationSoundConfig_id), activationSoundConfig_volume(activationSoundConfig_volume),
	throwPosXYGridSize(throwPosXYGridSize), throwAngDegZGridSize(throwAngDegZGridSize)
{
	//void
}

bool ToolConfigStackablesHauler::getCanPickup() {
	if (pickupRangeM >= 0) {
		return true;
	}

	return false;
}

bool ToolConfigStackablesHauler::getCanThrow() {
	if (throwRangeM >= 0) {
		return true;
	}

	return false;
}

sp<Tool> ToolConfigStackablesHauler::createNewTool(IAppAssets* appAssets) {
	//return super.createNewTool();

	sp<ToolStackablesHauler> tool = msp<ToolStackablesHauler>(this);
	tool->reservedCreate(appAssets->getEmbeddedAssetsManager());

	return tool;
}

ToolConfigStackablesHauler::~ToolConfigStackablesHauler() {
	//void
}
