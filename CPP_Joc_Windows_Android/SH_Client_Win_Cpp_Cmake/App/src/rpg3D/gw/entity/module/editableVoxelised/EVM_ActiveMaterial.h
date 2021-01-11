#pragma once

#include <base/gh.h>
#include "EVM_TouchProjection.h"

namespace rpg3D {
class EVM_ActiveMaterial : public EVM_TouchProjection {priv typedef EVM_TouchProjection super;pub dCtor(EVM_ActiveMaterial);
	int activeDynamicMaterialIndex = -1;
	
	pub explicit EVM_ActiveMaterial(
		IWorldEntity* entity,
		EditableVoxelisedTemplate* t);

	pub int peekActiveDynamicMaterialIndex() final;
	pub void setActiveDynamicMaterialIndex(int activeDynamicMaterialIndex) final;
	prot virtual void onActiveDrawDynamicMaterialIndexChanged();

	prot void onEditorColorRemoved(int editorColorIndex, Color& col, bool updatesHistory) override;

	prot void onIsEditingEnabledChanged() override;

    pub ~EVM_ActiveMaterial() override;
};
};
