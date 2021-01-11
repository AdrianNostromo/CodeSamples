#include "WIF_Touch_FeetPlaneOrHalfSphereDome.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/util/GameCameraModeBitGroups.h>

using namespace rpg3D;

WIF_Touch_FeetPlaneOrHalfSphereDome::WIF_Touch_FeetPlaneOrHalfSphereDome(
	int cursorModeBitGroups, int buttonIdBitGroups, int touchTypeBitGroups,
	int inputPriority, int cameraModesBitMaskAny,
	std::function<bool(Vector3& localFeetPlanePos, int cursorIndex, int cameraModeBitGroup)> cb_onFilterActivation_LocalFeetPlane,
	std::function<bool(Vector3& halfSphereDomeWorldDir, int cursorIndex, int cameraModeBitGroup)> cb_onFilterActivation_HalfSphereDome)
	: super(
		cursorModeBitGroups, buttonIdBitGroups, touchTypeBitGroups,
		inputPriority, cameraModesBitMaskAny
	),
	cb_onFilterActivation_LocalFeetPlane(cb_onFilterActivation_LocalFeetPlane),
	cb_onFilterActivation_HalfSphereDome(cb_onFilterActivation_HalfSphereDome)
{
	//void
}

bool WIF_Touch_FeetPlaneOrHalfSphereDome::onCursorInput(
	base::Touch::CursorMode* cursorMode, base::Touch::ButtonCode* buttonId, TouchEventType* touchType,
	int cursorIndex, Vector2& screenPos,
	IWorldEntity* controlledEntity, int cameraModeBitGroup, base::IGameWorld* gw)
{
	Vector3 rayDir{};
	Vector3 localFeetPlanePos{};
	bool isIntersection = gw->getMain()->camera2DCoordsProjectionTo3DGroundPos(
		screenPos/*screenPos*/,
		controlledEntity->getZ()/*groundPlanePosZ*/,
		&localFeetPlanePos/*oIntersectionPos*/,
		&rayDir/*oRayDirection*/
	);
	if (isIntersection) {
		if (cursorIndex < 0) {
			// This can occur if the input entity is changed from a input filter.
			throw LogicException(LOC);
		}
		return cb_onFilterActivation_LocalFeetPlane(localFeetPlanePos, cursorIndex, cameraModeBitGroup);
	}

	return cb_onFilterActivation_HalfSphereDome(rayDir, cursorIndex, cameraModeBitGroup);
}

WIF_Touch_FeetPlaneOrHalfSphereDome::~WIF_Touch_FeetPlaneOrHalfSphereDome() {
	//void
}
