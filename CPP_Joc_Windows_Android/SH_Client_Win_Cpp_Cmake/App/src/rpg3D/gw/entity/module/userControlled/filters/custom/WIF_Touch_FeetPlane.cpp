#include "WIF_Touch_FeetPlane.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/main/IGamePartMain.h>

using namespace rpg3D;

WIF_Touch_FeetPlane::WIF_Touch_FeetPlane(
	int cursorModeBitGroups, int buttonIdBitGroups, int touchTypeBitGroups,
	int inputPriority,
	std::function<bool(Vector3& groundPos, int cursorIndex)> cb_onFilterActivation)
	: super(
		cursorModeBitGroups, buttonIdBitGroups, touchTypeBitGroups,
		inputPriority, -1/*cameraModesBitMaskAny*/
	),
	cb_onFilterActivation(cb_onFilterActivation)
{
	//void
}

bool WIF_Touch_FeetPlane::onCursorInput(
	base::Touch::CursorMode* cursorMode, base::Touch::ButtonCode* buttonId, TouchEventType* touchType,
	int cursorIndex, Vector2& screenPos,
	IWorldEntity* controlledEntity, int cameraMode, base::IGameWorld* gw)
{
	Vector3 localFeetPlanePos{};
	bool isIntersection = gw->getMain()->camera2DCoordsProjectionTo3DGroundPos(
		screenPos/*screenPos*/,
		controlledEntity->getZ()/*groundPlanePosZ*/,
		&localFeetPlanePos/*oIntersectionPos*/,
		nullptr/*oRayDirection*/
	);

	if (!isIntersection) {
		return false;
	}

	return cb_onFilterActivation(localFeetPlanePos, cursorIndex);
}

WIF_Touch_FeetPlane::~WIF_Touch_FeetPlane() {
	//void
}
