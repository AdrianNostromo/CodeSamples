#include "ToolConfigZEAssets.h"
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>
#include "../ToolZEAssets.h"

using namespace rpg3D;

std::string ToolConfigZEAssets::TAG_zeAssets = "TAG_zeAssets";

ToolConfigZEAssets::ToolConfigZEAssets(
	std::string id,
	int filtersPriority,
	std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
	std::string shortName_partA, std::string shortName_partB,
	float cooldownS, float channelingDurationS,
	std::string* activationSoundConfig_id, float activationSoundConfig_volume)
	: super(
		id, std::make_shared<Array1D<std::string>>(2, new std::string[2]{ TAG_zoneEditor, TAG_zeAssets }),
		filtersPriority,
		icon_atlasId, icon_regionId,
		shortName_partA, shortName_partB,
		cooldownS, channelingDurationS,
		activationSoundConfig_id, activationSoundConfig_volume)
{
	//void
}

sp<Tool> ToolConfigZEAssets::createNewTool(IAppAssets* appAssets) {
	//return super.createNewTool();

	sp<ToolZEAssets> tool = msp<ToolZEAssets>(this, nullptr);
	tool->reservedCreate(appAssets->getEmbeddedAssetsManager());

	return tool;
}

ToolConfigZEAssets::~ToolConfigZEAssets() {
	//void
}
