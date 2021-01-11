#pragma once

#include <base/gh.h>
#include "MIGUIV_SelectedEntity.h"

namespace rpg3D {
	class IToolsHandlerModule;
};

namespace rpg3D {
class MIGUIVAT_Base : public MIGUIV_SelectedEntity {priv typedef MIGUIV_SelectedEntity super;pub dCtor(MIGUIVAT_Base);
	prot IToolsHandlerModule* selectedFocusEntity_toolsHandler = nullptr;
	
	prot IListenerToolExtraActivation* listenerToolExtraActivation = nullptr;

	pub explicit MIGUIVAT_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw);

	pub void setListenerToolExtraActivation(IListenerToolExtraActivation* listenerToolExtraActivation) final;

	prot void onSelectedFocusEntityChange_pre() override;
	prot void onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) override;

    pub ~MIGUIVAT_Base() override;
};
};
