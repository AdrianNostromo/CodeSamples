#pragma once

#include <base/gh.h>
#include "GPUI_ZoneEditorView.h"
#include <rpg3D/gw/view/entityEditor/IEntityEditorView.h>
#include <rpg3D/gw/entity/module/voxelizedEditor/util/IHandlerVoxelizedEditorModule.h>

namespace rpg3D {
	class EntityEditorView;
};

namespace rpg3D {
class GPUI_EntityEditorView : public GPUI_ZoneEditorView, virtual public IHandlerVoxelizedEditorModule {priv typedef GPUI_ZoneEditorView super;pub dCtor(GPUI_EntityEditorView);
	priv bool isEditorActive = false;
	priv EntityEditorView* entityEditorView = nullptr;
	priv IWorldEntity* editorEntity = nullptr;
	priv IWorldEntity* entityToEdit = nullptr;

	priv IEntityEditorView::CbAnimationListener viewHideAnimationListener{};

	priv int prevValue_cameraMode = -1;
	priv int prevValue_entityControlType = -1;

	pub explicit GPUI_EntityEditorView(base::IGameWorld* gw);
	pub void onEvent(IEventListener& eventListener, base::GWEvent& event);

	pub bool getIsEntityEditorActive() override;
	pub void startEntityEditor(int editorMode, bool isEditorStructureEnabled, Vector3* pos, IWorldEntity* existingEntityToEditOptional, IWorldEntity* existingEntityToEditOptionalB) override;
	priv EntityEditorView* createNewView();

	pub void addEditorTools(IWorldEntity* editorEntity, int editorMode);
	pub void removeEditorTools(IWorldEntity* editorEntity);

	pub void entityEditor_requestStopEditor() override;
	pub void entityEditor_processOnEditOccured(IWorldEntity* entityToEdit) override;

	// These must be fully extended to use the editor.
	prot virtual void entityEditor_onEntityCreated(IWorldEntity* editEntity);
	prot virtual std::shared_ptr<std::string> entityEditor_getNewEntityInstanceId();
	prot virtual void entityEditor_onEditOccured(IWorldEntity* editEntity);
	prot virtual void entityEditor_onEditorClosed();

	priv void stopCreatureCreator(bool doAnimation);

	priv void onViewHideAnimationFinished(IGenericDrawable* target);

	prot void disposePrePre() override;
	pub ~GPUI_EntityEditorView() override;
};
};
