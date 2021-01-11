#pragma once

#include <base/gh.h>
#include "TZEA_Base.h"
#include <base/math/Vector3Int.h>
#include <base/sp.h>
#include <string>
#include <base/math/Vector3.h>

namespace base {
	class WrappedWorldEntity;
	namespace snackbar {
		class WrappedSnackbarView;
	};
};
namespace worldGame3D {
	class WrappedWorldFlag;
};

namespace rpg3D {
class TZEA_Tool : public TZEA_Base {priv typedef TZEA_Base super;pub dCtor(TZEA_Tool);
	priv static const int TouchMode_TOOL;

	priv enum ExistingEntitiesEditMode { Move = 0, Rotate = 1, Delete = 2 };
	priv static std::string existingEntitiesEditModeIconRegionIds[3];

	priv bool isToolActivationFilterReceived = false;

	// This is required to handle delete and place_none_selected.
	priv bool isPlacementOccured = false;
	priv bool isInitialGroundPosProcessed = false;
	priv bool forceMoveNewlyPlacedAsset = false;
	priv Vector3 lastTickGroundPos{};
	// This is also used for the drag, rotate that occurs after touch.
	priv std::shared_ptr<base::WrappedWorldEntity> placedWEntity = nullptr;

	prot sp<worldGame3D::WrappedWorldFlag> wWFlag_zoneEditorAssetInstanceIdCounter = nullptr;

	priv ExistingEntitiesEditMode existingEntitiesEditMode = Move;
	priv ExtraActivationSecondaryControl* secondaryControl_entityEditMode = nullptr;

	priv ExtraActivationListControl* listControl_asset_None = nullptr;
	priv ExtraActivationListControl* listControl_asset_Tree = nullptr;
	priv ExtraActivationListControl* listControl_asset_Flower = nullptr;
	priv ExtraActivationListControl* listControl_asset_pillar = nullptr;
	priv ExtraActivationListControl* listControl_asset_wallSegment = nullptr;
	priv ExtraActivationListControl* listControl_asset_doorframe = nullptr;

	// This is used to avoid messages spam while the same message already is visible.
	priv sp<base::snackbar::WrappedSnackbarView> snackbarWView_assetNotDeletable = nullptr;

    pub explicit TZEA_Tool(ToolConfigZEAssets* config, std::shared_ptr<ExtraData> extraData);
	prot void create(IAssetsManager* assetsManager) override;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

	prot int computeTouchModeUnderPoint(ToolActivationInfo_Base* touchData) override;

	prot void onIsOperatingChanged(bool isOperating) override;
	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

	prot void tickActiveTouchMode(
		int touchMode, float deltaS,
		ToolActivationInfo_Base* touchData) override;

	priv void toggleNextExistingEntitiesEditMode();

    pub ~TZEA_Tool() override;
};
};
