#pragma once

#include <base/gh.h>
#include "GPUI_Base.h"

namespace startarea_a_v1 {
class GPUI_EntityEditorView : public GPUI_Base {priv typedef GPUI_Base super;pub dCtor(GPUI_EntityEditorView);
    pub explicit GPUI_EntityEditorView(base::IGameWorld* gw);

	prot void entityEditor_onEntityCreated(IWorldEntity* editEntity) final;
	prot std::shared_ptr<std::string> entityEditor_getNewEntityInstanceId() final;
	prot void entityEditor_onEditOccured(IWorldEntity* editEntity) final;
	prot void entityEditor_onEditorClosed() final;

    pub ~GPUI_EntityEditorView() override;
};
};
