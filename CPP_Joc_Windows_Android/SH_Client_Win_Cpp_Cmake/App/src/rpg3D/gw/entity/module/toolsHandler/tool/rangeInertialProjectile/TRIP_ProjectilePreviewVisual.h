#pragma once

#include <base/gh.h>
#include "TRIP_Visual.h"

namespace rpg3D {
class TRIP_ProjectilePreviewVisual : public TRIP_Visual {priv typedef TRIP_Visual super;pub dCtor(TRIP_ProjectilePreviewVisual);
	priv ArrayList<IDrawableNode3D*>* drawablesList = nullptr;
	priv Map1D<std::string, IDrawableNode3D*>* drawablesMap = nullptr;
	priv IDrawableNode3D* rootDrawable = nullptr;

	priv IContainer3D* projectilePreviewParent = nullptr;

	priv bool isVisualsDirty = false;

	pub explicit TRIP_ProjectilePreviewVisual(ToolConfigRangeInertialProjectile* config, std::shared_ptr<ExtraData> extraData);

	pub void requireManagedVisualUpdate(IManagedDynamicVisual* targetManagedVisual) override;

	pub void updateVisual() override;

	priv void disposeDrawables();

	prot void onProjectileConfigChanged() override;

	prot void onEntityChangePre() override;
	prot void onEntityChanged() override;

	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

	prot void onCooldownStateChanged() override;

	priv void requireVisualsUpdate();

	priv void checkProjectileVisualUpdate();

	prot void disposeMain() override;
	pub ~TRIP_ProjectilePreviewVisual() override;

};
};
