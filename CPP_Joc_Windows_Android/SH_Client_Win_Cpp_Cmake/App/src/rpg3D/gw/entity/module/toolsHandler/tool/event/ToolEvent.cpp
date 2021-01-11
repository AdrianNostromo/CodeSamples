#include "ToolEvent.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

int ToolEvent::typeCounter = 0;

ToolEvent::ToolEvent(std::string name)
	: name(std::move(name))
{
	type = ++typeCounter;
}

void ToolEvent::clearData() {
	tool = nullptr;

	entryUid_deselected = -1;
	entryUid_selected = -1;

	extraActivationSecondaryControl = nullptr;
}

ToolEvent* ToolEvent::setExtraControlChangeDelta(
	ITool* tool,
	int entryUid_deselected, int entryUid_selected)
{
	this->tool = tool;

	this->entryUid_deselected = entryUid_deselected;
	this->entryUid_selected = entryUid_selected;

	return this;
}

ToolEvent* ToolEvent::setSecondaryControlConfigChanged(
	ITool* tool,
	ITool::ExtraActivationSecondaryControl* extraActivationSecondaryControl)
{
	this->tool = tool;

	this->extraActivationSecondaryControl = extraActivationSecondaryControl;

	return this;
}

ToolEvent::~ToolEvent() {
	//void
}
