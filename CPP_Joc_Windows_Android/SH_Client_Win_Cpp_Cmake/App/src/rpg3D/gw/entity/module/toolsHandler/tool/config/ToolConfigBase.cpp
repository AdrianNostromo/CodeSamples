#include "ToolConfigBase.h"

using namespace rpg3D;

const int ToolConfigBase::FiltersPriority::automaticGeneral = 0;
const int ToolConfigBase::FiltersPriority::automaticMovement = 1;
const int ToolConfigBase::FiltersPriority::manual = 2;

Map1D<std::string, ToolConfigBase*> ToolConfigBase::EntriesMap{};
ArrayList<ToolConfigBase*> ToolConfigBase::EntriesList{};

ToolConfigBase::ToolConfigBase(
	std::string id, std::shared_ptr<Array1D<std::string>> tagsList,
	int filtersPriority, bool isUserSelectable, bool isDirectional, bool supportsContinuousReactivation, float channelingDurationS,
	std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
	std::string shortName_partA, std::string shortName_partB)
	: super(),
	id(id), tagsList(tagsList),
	filtersPriority(filtersPriority), isUserSelectable(isUserSelectable), isDirectional(isDirectional), supportsContinuousReactivation(supportsContinuousReactivation), channelingDurationS(channelingDurationS),
	icon_atlasId(icon_atlasId), icon_regionId(icon_regionId),
	shortName_partA(shortName_partA), shortName_partB(shortName_partB)
{
	EntriesMap.putDirect(id, this);
    EntriesList.appendDirect(this);
}

sp<Tool> ToolConfigBase::createNewTool(IAppAssets* appAssets) {
	// Must be extended completly to create a tool object with this util.
	// Not implemented correctly.

	throw LogicException(LOC);
}

ToolConfigBase* ToolConfigBase::FromId(std::string& id, bool mustExist) {
	ToolConfigBase* toolConfig = EntriesMap.getDirect(id, nullptr/*fallbackValue*/);
	if (toolConfig == nullptr && mustExist) {
		throw LogicException(LOC);
	}

	return toolConfig;
}

ToolConfigBase::~ToolConfigBase() {
	//void
}
