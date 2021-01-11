#include "WIF_Touch.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/main/IGamePartMain.h>

using namespace rpg3D;

WIF_Touch::WIF_Touch(
	int cursorModeBitGroups, int buttonIdBitGroups, int touchTypeBitGroups,
	int inputPriority,
	std::function<bool(int cursorIndex, Vector2& screenPos)> cb_onFilterActivation)
	: super(
		cursorModeBitGroups, buttonIdBitGroups, touchTypeBitGroups,
		inputPriority, -1/*cameraModesBitMaskAny*/
	),
	cb_onFilterActivation(cb_onFilterActivation)
{
	//void
}

bool WIF_Touch::onCursorInput(
	base::Touch::CursorMode* cursorMode, base::Touch::ButtonCode* buttonId, TouchEventType* touchType,
	int cursorIndex, Vector2& screenPos,
	IWorldEntity* controlledEntity, int cameraMode, base::IGameWorld* gw)
{
	return cb_onFilterActivation(cursorIndex, screenPos);
}

WIF_Touch::~WIF_Touch() {
	//void
}
