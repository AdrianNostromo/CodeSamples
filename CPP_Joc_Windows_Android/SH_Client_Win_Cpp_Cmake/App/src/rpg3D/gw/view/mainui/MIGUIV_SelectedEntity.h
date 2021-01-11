#pragma once

#include <base/gh.h>
#include "MIGUIV_Base.h"

class IWorldEntity;

namespace rpg3D {
class MIGUIV_SelectedEntity : public MIGUIV_Base {priv typedef MIGUIV_Base super;pub dCtor(MIGUIV_SelectedEntity);
	// Use selected because thi may be a list in the future. Use focus because if a list of entities is used, one will be in focus.
	priv IWorldEntity* selectedFocusEntity = nullptr;
	priv ArrayList<std::shared_ptr<IEventListener>> selectedFocusEntity_autoListenersList{};

	pub explicit MIGUIV_SelectedEntity(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw);

	pub IWorldEntity* getSelectedFocusEntity();
	pub void setSelectedFocusEntity(IWorldEntity* selectedFocusEntity);
	prot virtual void onSelectedFocusEntityChange_pre();
	prot virtual void onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList);

	pub std::string computeStateFlags(std::string baseState) override;

	prot void disposeMainPre() override;
	pub ~MIGUIV_SelectedEntity() override;
};
};
