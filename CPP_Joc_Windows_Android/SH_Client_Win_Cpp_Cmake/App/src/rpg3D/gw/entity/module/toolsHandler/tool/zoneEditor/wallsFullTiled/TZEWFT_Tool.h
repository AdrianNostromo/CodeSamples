#pragma once

#include <base/gh.h>
#include "TZEWFT_Base.h"
#include <base/math/Vector3Int.h>

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
class TZEWFT_Tool : public TZEWFT_Base {priv typedef TZEWFT_Base super;pub dCtor(TZEWFT_Tool);
	priv static const int TouchMode_TOOL;

	priv Vector2Int lastProcessedGPos{};
	priv bool dontProcessSameGPos = false;

	prot sp<worldGame3D::WrappedWorldFlag> wWFlag_zoneEditorTileCellSizeCm = nullptr;

	IWorldEntity* entityToEdit = nullptr;
	worldGame3D::IVisual3DModule* entityToEdit_visual3D = nullptr;
	VoxelGridDrawable3D* entityToEdit_visual3D_editingVoxelsVisual = nullptr;
	IPhysics3DModule* entityToEdit_physics3D = nullptr;

	priv ExtraActivationListControl* listControl_wall_None = nullptr;
	priv ExtraActivationListControl* listControl_wall_AB = nullptr;
	priv ExtraActivationListControl* listControl_wall_A = nullptr;
	priv ExtraActivationListControl* listControl_wall_B = nullptr;

    pub explicit TZEWFT_Tool(ToolConfigZEWallsFullTiled* config, std::shared_ptr<ExtraData> extraData);
	prot void create(IAssetsManager* assetsManager) override;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

	prot int computeTouchModeUnderPoint(ToolActivationInfo_Base* touchData) override;

	prot void onIsOperatingChanged(bool isOperating) override;
	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

	prot void tickActiveTouchMode(
		int touchMode, float deltaS,
		ToolActivationInfo_Base* touchData) override;

    pub ~TZEWFT_Tool() override;
};
};
