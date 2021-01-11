#include "WorldInteractionFilter.h"
#include <base/exceptions/NotImplementedException.h>

using namespace rpg3D;

WorldInteractionFilter::WorldInteractionFilter(
	int cursorModeBitGroups, int buttonIdBitGroups, int touchTypeBitGroups,
	int inputPriority, int cameraModesBitMaskAny)
	: cursorModeBitGroups(cursorModeBitGroups), buttonIdBitGroups(buttonIdBitGroups), touchTypeBitGroups(touchTypeBitGroups),
	inputPriority(inputPriority), cameraModesBitMaskAny(cameraModesBitMaskAny)
{
	//void
}

int WorldInteractionFilter::getCursorModeBitGroups() {
	return cursorModeBitGroups;
}

int WorldInteractionFilter::getButtonIdBitGroups() {
	return buttonIdBitGroups;
}

int WorldInteractionFilter::getTouchTypeBitGroups() {
	return touchTypeBitGroups;
}

int WorldInteractionFilter::getInputPriority() {
	return inputPriority;
}

int WorldInteractionFilter::getCameraModesBitMaskAny() {
	return cameraModesBitMaskAny;
}

bool& WorldInteractionFilter::getIsHookedRef() {
	return isHooked;
}

ArrayList<IListEntry*>& WorldInteractionFilter::getHookedListEntries() {
	return hookedListEntries;
}

bool WorldInteractionFilter::onCursorInput(
	base::Touch::CursorMode* cursorMode, base::Touch::ButtonCode* buttonId, TouchEventType* touchType,
	int cursorIndex, Vector2& screenPos,
	IWorldEntity* controlledEntity, int cameraModeBitGroup, base::IGameWorld* gw)
{
	// Fully extend this when input type used.
	throw NotImplementedException();
}

WorldInteractionFilter::~WorldInteractionFilter() {
	//void
}
