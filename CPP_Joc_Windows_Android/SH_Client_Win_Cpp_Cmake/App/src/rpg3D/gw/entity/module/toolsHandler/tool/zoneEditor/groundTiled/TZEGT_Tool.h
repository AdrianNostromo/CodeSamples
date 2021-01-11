#pragma once

#include <base/gh.h>
#include "TZEGT_Base.h"
#include <base/math/Vector2Int.h>
#include <base/sp.h>

class IWorldEntity;
namespace worldGame3D { class IVisual3DModule; }
class VoxelGridDrawable3D;
namespace rpg3D {
	class IPhysics3DModule;
};
namespace worldGame3D {
	class WrappedWorldFlag;
};

namespace rpg3D {
class TZEGT_Tool : public TZEGT_Base {priv typedef TZEGT_Base super;pub dCtor(TZEGT_Tool);
	priv static const int TouchMode_TOOL;

	priv Vector2Int lastProcessedGPos{};
	priv bool dontProcessSameGPos = false;

	prot sp<worldGame3D::WrappedWorldFlag> wWFlag_zoneEditorTileCellSizeCm = nullptr;

	IWorldEntity* entityToEdit = nullptr;
	worldGame3D::IVisual3DModule* entityToEdit_visual3D = nullptr;
	VoxelGridDrawable3D* entityToEdit_visual3D_editingVoxelsVisual = nullptr;
	IPhysics3DModule* entityToEdit_physics3D = nullptr;

	IWorldEntity* entityToEditB = nullptr;
	worldGame3D::IVisual3DModule* entityToEditB_visual3D = nullptr;
	VoxelGridDrawable3D* entityToEditB_visual3D_editingVoxelsVisual = nullptr;

	priv ExtraActivationListControl* listControl_ground_None = nullptr;
	priv ExtraActivationListControl* listControl_ground_AB = nullptr;
	priv ExtraActivationListControl* listControl_ground_A = nullptr;
	priv ExtraActivationListControl* listControl_ground_B = nullptr;

    pub explicit TZEGT_Tool(ToolConfigZEGroundTiled* config, std::shared_ptr<ExtraData> extraData);
	prot void create(IAssetsManager* assetsManager) override;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

	prot int computeTouchModeUnderPoint(ToolActivationInfo_Base* touchData) override;

	prot void onIsOperatingChanged(bool isOperating) override;
	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

	prot void tickActiveTouchMode(
		int touchMode, float deltaS,
		ToolActivationInfo_Base* touchData) override;

    pub ~TZEGT_Tool() override;
};
};
