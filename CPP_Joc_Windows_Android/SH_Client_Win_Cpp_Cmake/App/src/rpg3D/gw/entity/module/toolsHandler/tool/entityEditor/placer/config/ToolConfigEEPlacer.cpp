#include "ToolConfigEEPlacer.h"
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>
#include "../ToolEEPlacer.h"

using namespace rpg3D;

std::string ToolConfigEEPlacer::TAG_eePlacer = "TAG_eePlacer";

ToolConfigEEPlacer::ToolConfigEEPlacer(
	std::string id,
	int filtersPriority,
	std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
	std::string shortName_partA, std::string shortName_partB,
	float cooldownS, float channelingDurationS,
	std::string* activationSoundConfig_id, float activationSoundConfig_volume)
	: super(
		id, std::make_shared<Array1D<std::string>>(2, new std::string[2]{ TAG_entityEditor, TAG_eePlacer }),
		filtersPriority,
		icon_atlasId, icon_regionId,
		shortName_partA, shortName_partB,
		cooldownS, channelingDurationS,
		activationSoundConfig_id, activationSoundConfig_volume)
{
	//void
}

sp<Tool> ToolConfigEEPlacer::createNewTool(IAppAssets* appAssets) {
	//return super.createNewTool();

	sp<ToolEEPlacer> tool = msp<ToolEEPlacer>(this, nullptr);
	tool->reservedCreate(appAssets->getEmbeddedAssetsManager());

	return tool;
}

ToolConfigEEPlacer::~ToolConfigEEPlacer() {
	//void
}
