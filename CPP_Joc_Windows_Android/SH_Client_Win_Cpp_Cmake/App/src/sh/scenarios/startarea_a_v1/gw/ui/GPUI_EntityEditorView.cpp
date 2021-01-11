#include "GPUI_EntityEditorView.h"
#include "../zoneMain/localCacheContent/IGamePartLocalCacheContent.h"

using namespace startarea_a_v1;

GPUI_EntityEditorView::GPUI_EntityEditorView(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void GPUI_EntityEditorView::entityEditor_onEntityCreated(IWorldEntity* editEntity) {
	gw->getComponentAs<startarea_a_v1::IGamePartLocalCacheContent*>(true/*mustExist*/)->pushPersistentEntity(editEntity, false);
}

std::shared_ptr<std::string> GPUI_EntityEditorView::entityEditor_getNewEntityInstanceId() {
	std::shared_ptr<std::string> instanceId = std::make_shared<std::string>("persistentEntity_" + std::to_string(gw->getComponentAs<startarea_a_v1::IGamePartLocalCacheContent*>(true/*mustExist*/)->getNewPersistentEntityId()));

	return instanceId;
}

void GPUI_EntityEditorView::entityEditor_onEditOccured(IWorldEntity* editEntity) {
	gw->getComponentAs<startarea_a_v1::IGamePartLocalCacheContent*>(true/*mustExist*/)->persistentEntityTemplateChanged(editEntity);
}

void GPUI_EntityEditorView::entityEditor_onEditorClosed() {
	gw->getComponentAs<startarea_a_v1::IGamePartLocalCacheContent*>(true/*mustExist*/)->savePendingPersistentEntitiesChanges();
}

GPUI_EntityEditorView::~GPUI_EntityEditorView() {
	//void
}
