#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionFilter.h>
#include <rpg3D/gw/entity/module/userControlled/util/InteractionFiltersGroup.h>
#include "TRIP_ProjectilePreviewVisual.h"

namespace rpg3D {
class ToolRangeInertialProjectile : public TRIP_ProjectilePreviewVisual {priv typedef TRIP_ProjectilePreviewVisual super; pub dCtor(ToolRangeInertialProjectile);
	// These are used to detect when the tool extended activation finishes.
	priv ArrayList<std::shared_ptr<IEventListener>> projectilesAutoListenersList{};

	prot ArrayList<IContainer3D*> projectileSpawnParentsList{};

	// This is used to detect tool extended activation finish.
	priv int activeProjectilesCount = 0;

	pub explicit ToolRangeInertialProjectile(ToolConfigRangeInertialProjectile* config, std::shared_ptr<ExtraData> extraData);

	priv bool onFilterInteraction_innactiveForActivation_Feet_Plane_Down_BtnMain(Vector3& localFeetPlanePos, int cursorIndex, int cameraModeBitGroup);
	priv bool onFilterInteraction_innactiveForActivation_Feet_HalfSphereDome_Down_BtnMain(Vector3& halfSphereDomeWorldDir, int cursorIndex, int cameraModeBitGroup);
	
	priv bool onFilterInteraction_activeContinuousForUpdate_Feet_Plane_Down_BtnMain(Vector3& localFeetPlanePos, int cursorIndex, int cameraModeBitGroup);
	priv bool onFilterInteraction_activeContinuousForUpdate_Feet_HalfSphereDome_Down_BtnMain(Vector3& halfSphereDomeWorldDir, int cursorIndex, int cameraModeBitGroup);
	
	priv bool onFilterInteraction_activeContinuousForDeactivation_Touch_Up(int cursorIndex, Vector2& screenPos);

	prot void onEntityChanged() override;

	prot bool tickTriggerInProgress(float triggerDurationS, int tickIndex) final;
	priv void spawnProjectile(IContainer3D* projectileSpawnParent, float finalAngZDegLocal);
	priv void onProjectileEvent(IEventListener& eventListener, base::EntityEvent& event);

	prot void onActiveTriggeringCancelPre() override;

	pub ~ToolRangeInertialProjectile() override;
};
};
