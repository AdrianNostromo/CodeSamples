#include "ToolConfigZEGroundTiled.h"
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>
#include "../ToolZEGroundTiled.h"

using namespace rpg3D;

std::string ToolConfigZEGroundTiled::TAG_zeGroundTiles = "TAG_zeGroundTiles";

ToolConfigZEGroundTiled::ToolConfigZEGroundTiled(
	std::string id,
	int filtersPriority,
	std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
	std::string shortName_partA, std::string shortName_partB,
	float cooldownS, float channelingDurationS,
	std::string* activationSoundConfig_id, float activationSoundConfig_volume)
	: super(
		id, std::make_shared<Array1D<std::string>>(2, new std::string[2]{ TAG_zoneEditor, TAG_zeGroundTiles }),
		filtersPriority,
		icon_atlasId, icon_regionId,
		shortName_partA, shortName_partB,
		cooldownS, channelingDurationS,
		activationSoundConfig_id, activationSoundConfig_volume)
{
	//void
}

sp<Tool> ToolConfigZEGroundTiled::createNewTool(IAppAssets* appAssets) {
	//return super.createNewTool();

	sp<ToolZEGroundTiled> tool = msp<ToolZEGroundTiled>(this, nullptr);
	tool->reservedCreate(appAssets->getEmbeddedAssetsManager());

	return tool;
}

ToolConfigZEGroundTiled::~ToolConfigZEGroundTiled() {
	//void
}
