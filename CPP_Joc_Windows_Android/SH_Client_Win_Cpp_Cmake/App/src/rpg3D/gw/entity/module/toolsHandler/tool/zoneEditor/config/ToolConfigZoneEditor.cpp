#include "ToolConfigZoneEditor.h"

using namespace rpg3D;

std::string ToolConfigZoneEditor::TAG_zoneEditor = "TAG_zoneEditor";

ToolConfigZoneEditor::ToolConfigZoneEditor(
	std::string id,
	std::shared_ptr<Array1D<std::string>> tagsList,
	int filtersPriority,
	std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
	std::string& shortName_partA, std::string& shortName_partB,
	float cooldownS, float channelingDurationS,
	std::string* activationSoundConfig_id, float activationSoundConfig_volume)
	: super(
		id, tagsList, 
		filtersPriority, true/*isUserSelectable*/, true/*isDirectional*/, true/*supportsContinuousReactivation*/, channelingDurationS,
		icon_atlasId, icon_regionId,
		shortName_partA, shortName_partB),
	cooldownS(cooldownS),
	activationSoundConfig_id(activationSoundConfig_id), activationSoundConfig_volume(activationSoundConfig_volume)
{
	//void
}

ToolConfigZoneEditor::~ToolConfigZoneEditor() {
	//void
}
