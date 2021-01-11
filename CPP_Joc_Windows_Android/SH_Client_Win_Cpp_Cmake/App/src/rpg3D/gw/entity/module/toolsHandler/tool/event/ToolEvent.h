#pragma once

#include <base/gh.h>
#include <string>
#include <rpg3D/gw/entity/module/toolsHandler/tool/ITool.h>

class IWorldEntity;

namespace rpg3D {
class ToolEvent {pub dCtor(ToolEvent);
	priv static int typeCounter;

	pub int type;

	pub std::string name;

	pub ITool* tool = nullptr;

	pub int entryUid_deselected = -1;
	pub int entryUid_selected = -1;

	pub ITool::ExtraActivationSecondaryControl* extraActivationSecondaryControl = nullptr;

    pub explicit ToolEvent(std::string name);

	pub void clearData();

	pub ToolEvent* setExtraControlChangeDelta(
		ITool* tool,
		int entryUid_deselected, int entryUid_selected);
	
	pub ToolEvent* setSecondaryControlConfigChanged(
		ITool* tool,
		ITool::ExtraActivationSecondaryControl* extraActivationSecondaryControl);

	pub virtual ~ToolEvent();
};
};
