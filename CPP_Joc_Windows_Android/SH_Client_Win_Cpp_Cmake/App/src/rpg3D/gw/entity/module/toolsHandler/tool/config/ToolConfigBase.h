#pragma once

#include <base/gh.h>
#include <string>
#include <base/list/ArrayList.h>
#include <base/list/Array1D.h>
#include <memory>
#include <base/sp.h>
#include <base/map/Map1D.h>
#include <base/exceptions/LogicException.h>
#include <base/object/Object.h>

class IAppAssets;

namespace rpg3D {

class Tool;

class ToolConfigBase : public base::Object {priv typedef base::Object super; pub dCtor(ToolConfigBase);
	pub class FiltersPriority {
		// Lower is processed earlier.
		pub static const int automaticGeneral;
		pub static const int automaticMovement;
		pub static const int manual;
	};

    pub static Map1D<std::string, ToolConfigBase*> EntriesMap;
	pub static ArrayList<ToolConfigBase*> EntriesList;

	pub std::string id;

	// Can be nullptr (eg. movement tools).
	pub std::shared_ptr<Array1D<std::string>> tagsList;

	//asd_tool;// This is the old filters priority system, remove it and use the new one.
	///asd_tool;// This enumerates all inputs for all active tools each frame and discards lower priority filters.
	///asd_tool;// This doesn't work with the new system that passes the filters dirrectly to the tools without the WrappedTool.filtersInputsCacheList.
	pub int filtersPriority;

	pub bool isUserSelectable;

	// If this is true, idle_auto_turn will not occur while this is active_in_continuous_mode (That is the default mode, it just auto activates the tool if a btn is pressed.).
	// This is used only with certain turn types that work with idle_auto_turn logic.
	pub bool isDirectional;

	// This is used for tools to reactivate if the player keeps the cursor down.
	pub bool supportsContinuousReactivation;
	
	// <0;// Infinite channeling or until manually disabled.
	// 0;// No channeling.
	// > 0;// Max channeling duration.
	pub float channelingDurationS;

	pub std::shared_ptr<std::string> icon_atlasId;
	pub std::shared_ptr<std::string> icon_regionId;

	pub std::string shortName_partA;
	pub std::string shortName_partB;

	pub explicit ToolConfigBase(
		std::string id, std::shared_ptr<Array1D<std::string>> tagsList,
		int filtersPriority, bool isUserSelectable, bool isDirectional, bool supportsContinuousReactivation, float channelingDurationS,
		std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
		std::string shortName_partA, std::string shortName_partB);

	pub virtual sp<Tool> createNewTool(IAppAssets* appAssets);

	pub static ToolConfigBase* FromId(std::string& id, bool mustExist=false);

	pub template <typename T>
	static T FromIdAs(std::string& id);

	pub virtual ~ToolConfigBase();
};

template <typename T>
T ToolConfigBase::FromIdAs(std::string& id) {
	ToolConfigBase* toolConfig = EntriesMap.getDirect(id, nullptr/*fallbackValue*/);
	if(toolConfig == nullptr) {
		return nullptr;
	}

	T cToolConfig = toolConfig->castAs<T>();
	if(cToolConfig == nullptr) {
		throw LogicException(LOC);
	}

	return cToolConfig;
}

};
