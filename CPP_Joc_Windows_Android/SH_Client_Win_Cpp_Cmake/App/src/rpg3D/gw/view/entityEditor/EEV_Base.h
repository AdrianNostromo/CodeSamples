#pragma once

#include <base/gh.h>
#include <base/view/UIView.h>
#include <base/app/IAppUtils.h>
#include "IEntityEditorView.h"
#include "IEntityEditorView_Edit.h"
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>
#include <base/map/ExtraData.h>
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>

class WorldEntity;
class IWorldEntity;
class EntityTemplate;
namespace base {
	class IBlockingInputLayer;
};
namespace rpg3D {
	class IHandlerEntityEditorView;
	class IListenerEntityEditorView;
	class IStackableModule;
};

namespace rpg3D {
class EEV_Base : public UIView, virtual public IEntityEditorView, virtual public IEntityEditorView_Edit {priv typedef UIView super;pub dCtor(EEV_Base);
	prot int editorType = EditorType::EDIT_OTHER;

	prot IAppUtils* appUtils;

	prot IVoxelizedEditorM_Edit* entityEditor = nullptr;
	prot IListenerEntityEditorView* listener = nullptr;

	prot IEditableVoxelisedM_Edit* entityToEdit = nullptr;
	prot rpg3D::IStackableModule* entityToEdit_stackable = nullptr;

	priv base::IBlockingInputLayer* blockingStreamLayer = nullptr;

	pub explicit EEV_Base(
		IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
		IAppUtils* appUtils);

	pub void setHandlerAndListener(IVoxelizedEditorM_Edit* entityEditor, IListenerEntityEditorView* listener) override;
	pub void setEntityToEdit(IEditableVoxelisedM_Edit* entityToEdit) final;
	prot virtual void onEntityToEditChangePre();
	prot virtual void onEntityToEditChanged();

	prot base::IBlockingInputLayer* getBlockingStreamLayer();
	prot void setBlockingStreamLayer(base::IBlockingInputLayer* blockingStreamLayer);

	pub void onTranslateVoxelsGrid(float x, float y, float z, bool updatesHistory) override;
	pub void onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) override;
	pub void onEditEntity_stackable_haulOffsetZM_changed(float haulOffsetZM, float old_haulOffsetZM, bool updatesHistory) override;
	pub void onVoxelsGridSizeChange(Vector3Int& gSizeBefore, Vector3Int& gSizeAfter) override;

	pub std::string computeStateFlags(std::string baseState) override;

	pub ~EEV_Base() override;

};
};
