#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionFilter.h>
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>
#include <base/event/listener/IEventListener.h>
#include <rpg3D/gw/entity/module/userControlled/util/InteractionFiltersGroup.h>
#include "TMA_Physics.h"

namespace rpg3D {
class ToolMelleArea : public TMA_Physics {priv typedef TMA_Physics super; pub dCtor(ToolMelleArea);
	priv IContainer3D* slotVisualHolder = nullptr;

	pub explicit ToolMelleArea(ToolConfigMelleArea* config, std::shared_ptr<ExtraData> extraData);

	priv bool onFilterInteraction_innactiveForActivation_Feet_Plane_Down_BtnMain(Vector3& localFeetPlanePos, int cursorIndex, int cameraModeBitGroup);
	priv bool onFilterInteraction_innactiveForActivation_Feet_HalfSphereDome_Down_BtnMain(Vector3& halfSphereDomeWorldDir, int cursorIndex, int cameraModeBitGroup);
	priv bool onFilterInteraction_activeContinuousForUpdate_Feet_Plane_Down_BtnMain(Vector3& localFeetPlanePos, int cursorIndex, int cameraModeBitGroup);
	priv bool onFilterInteraction_activeContinuousForUpdate_Feet_HalfSphereDome_Down_BtnMain(Vector3& halfSphereDomeWorldDir, int cursorIndex, int cameraModeBitGroup);
	priv bool onFilterInteraction_activeContinuousForDeactivation_Touch_Up(int cursorIndex, Vector2& screenPos);
	
	prot bool tickTriggerInProgress(float triggerDurationS, int tickIndex) final;

	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

	prot void onActiveTriggeringCancelPre() override;

	pub ~ToolMelleArea() override;
};
};
