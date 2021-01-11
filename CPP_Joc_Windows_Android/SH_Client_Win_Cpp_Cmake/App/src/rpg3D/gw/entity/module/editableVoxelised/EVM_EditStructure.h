#pragma once

#include <base/gh.h>
#include "EVM_EditInterface.h"
#include <base/math/Vector3Int.h>

class VoxelGridDrawable3D;
namespace worldGame3D {
	class IVisual3DModule;
}

namespace rpg3D {
class EVM_EditStructure : public EVM_EditInterface {priv typedef EVM_EditInterface super;pub dCtor(EVM_EditStructure);
	priv bool isEditStructureVisualsCreated = false;
	priv IContainer3D* vHolderA = nullptr;
	priv IContainer3D* vHolderB = nullptr;
	priv IContainer3D* entityRootDrawable = nullptr;
	
    pub explicit EVM_EditStructure(
		IWorldEntity* entity,
		EditableVoxelisedTemplate* t);
	prot void createBMain() override;
	
	pub void onIsEditingEnabledChanged() override;

	pub IContainer3D* getEditorStructure_rotator() final;
	pub IContainer3D* getEditorStructure_childrensHolder() final;

	prot void onTranslateVoxelsGrid(float x, float y, float z, bool updatesHistory) override;
	prot void onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) override;
	prot void onVoxelsGridSizeChange(Vector3Int& gSizeBefore, Vector3Int& gSizeAfter) override;

	priv void createEditorStructure();
	priv void removeExistingEditorStructure();

	priv void syncEditorStructureVisualsIfNeeded();

	prot void disposeMain() override;
    pub ~EVM_EditStructure() override;
};
};
