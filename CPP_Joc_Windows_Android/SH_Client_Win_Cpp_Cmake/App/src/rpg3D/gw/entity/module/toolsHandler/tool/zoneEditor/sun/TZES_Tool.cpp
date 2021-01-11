#include "TZES_Tool.h"
#include <rpg3D/gw/entity/module/zoneEditor/IZoneEditorM_Edit.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <base/math/collision/Ray.h>
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionEvent.h>
#include <base/log/GlobalAccessLogHandler.h>
#include "../util/ToolActivationInfo_ScreenCursor.h"
#include "../util/ToolActivationInfo_None.h"
#include <base/math/Math.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/app/IAppMetrics.h>

using namespace rpg3D;

const int TZES_Tool::TouchMode_TOOL = getNew_touchModeIncrementor();

TZES_Tool::TZES_Tool(ToolConfigZESun* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData)
{
	activationFiltersList_innactiveForActivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::LockedCursor_Down_Main);
	activationFiltersList_innactiveForActivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::ScreenCursor_Down_Main);

	activationFiltersList_activeContinuousForUpdate->append_emplace((int)IWorldOrEntityInteractionFilter::Type::LockedCursor_Move);
	activationFiltersList_activeContinuousForUpdate->append_emplace((int)IWorldOrEntityInteractionFilter::Type::ScreenCursor_Drag_Main);

	activationFiltersList_activeContinuousForDeactivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::LockedCursor_Up_Main);
	activationFiltersList_activeContinuousForDeactivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Main);
}

bool TZES_Tool::onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	bool isProcessed = super::onFilterInteraction(filterSource, interaction);

	// Also check locally to detect error where multiple layers use the same filters (The filters lists are not set up for that and it should never occur).
	bool localIsProcessed = true;
	if (filterSource == FilterSource::innactiveForActivation) {
		if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::LockedCursor_Down_Main) {
			sp<ToolActivationInfo_Base> touchData = new ToolActivationInfo_ScreenCursor(interaction->cursorIndex, interaction->screenCursorPos);
			onFilterInteraction_innactiveForActivation(touchData);
		} else if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::ScreenCursor_Down_Main) {
			sp<ToolActivationInfo_Base> touchData = new ToolActivationInfo_ScreenCursor(interaction->cursorIndex, interaction->screenCursorPos);
			onFilterInteraction_innactiveForActivation(touchData);
		} else {
			localIsProcessed = false;
		}
	} else if (filterSource == FilterSource::activeContinuousForUpdate) {
		if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::LockedCursor_Move) {
			sp<ToolActivationInfo_Base> touchData = new ToolActivationInfo_ScreenCursor(interaction->cursorIndex, interaction->screenCursorPos, interaction->lockedCursorSensitisedDelta);
			onFilterInteraction_activeContinuousForUpdate(touchData);
		} else if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::ScreenCursor_Drag_Main) {
			sp<ToolActivationInfo_Base> touchData = new ToolActivationInfo_ScreenCursor(interaction->cursorIndex, interaction->screenCursorPos, interaction->screenCursorMoveDelta_sensitised);
			onFilterInteraction_activeContinuousForUpdate(touchData);
		} else {
			localIsProcessed = false;
		}
	} else if (filterSource == FilterSource::activeContinuousForDeactivation) {
		if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::LockedCursor_Up_Main) {
			sp<ToolActivationInfo_Base> touchData = new ToolActivationInfo_None(interaction->cursorIndex);
			onFilterInteraction_activeContinuousForDeactivation(touchData);
		} else if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Main) {
			sp<ToolActivationInfo_Base> touchData = new ToolActivationInfo_None(interaction->cursorIndex);
			onFilterInteraction_activeContinuousForDeactivation(touchData);
		} else {
			localIsProcessed = false;
		}
	} else {
		localIsProcessed = false;
	}

	if (isProcessed && localIsProcessed) {
		// Filter used in multiple layers. That should never occur. Filters code is not made for that.
		throw LogicException(LOC);
	}

	return isProcessed || localIsProcessed;
}

int TZES_Tool::computeTouchModeUnderPoint(ToolActivationInfo_Base* touchData) {
	// Use this extra var to avoid commenting the other return. This is to avoid bugs when the touch mode is not guaranteed to bhe tool if it changes in the future.
	bool alwaysUseToolTouchMode = true | true;
	if (alwaysUseToolTouchMode) {
		// Always use tool touc mode. The zone editor doesn't have extra tool touch modes (eg. entity editor has editing entity rotation as a extra touch mode).
		return TouchMode_TOOL;
	}

	return super::computeTouchModeUnderPoint(touchData);
}

void TZES_Tool::onIsOperatingChanged(bool isOperating) {
	super::onIsOperatingChanged(isOperating);

	if (!isOperating) {
		// Do clearing on the deactivation because doing it on activation may cause problems with certain activation logics.
		//void
	}
}

void TZES_Tool::tickActiveTouchMode(
	int touchMode, float deltaS,
	ToolActivationInfo_Base* touchData)
{
	super::tickActiveTouchMode(
		touchMode, deltaS,
		touchData
	);

	if (touchMode == TouchMode_TOOL) {
		ToolActivationInfo_ScreenCursor* cTouchData = touchData->castAs<ToolActivationInfo_ScreenCursor*>(ToolActivationInfo_ScreenCursor::TYPE);
		
		if (!cTouchData->sensitisedScreenCursorMoveDelta.equals(0.0f, 0.0f)) {
			//asd_01;// rotate the sun_light and sun_shadow_light
			GlobalAccessLogHandler::devLogHandler->post_debug("assets_sun_tick; Screen cursor delta(sensitised): %f %f", cTouchData->sensitisedScreenCursorMoveDelta.x, cTouchData->sensitisedScreenCursorMoveDelta.y);
		}
	}
}

TZES_Tool::~TZES_Tool() {
	//void
}
