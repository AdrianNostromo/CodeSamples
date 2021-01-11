#include "MIGUIV_SelectedEntity.h"

using namespace rpg3D;

MIGUIV_SelectedEntity::MIGUIV_SelectedEntity(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw)
	: super(app, viewItemConfigs, viewEaseDurationsSList, gw)
{
	//void
}

IWorldEntity* MIGUIV_SelectedEntity::getSelectedFocusEntity() {
	return selectedFocusEntity;
}

void MIGUIV_SelectedEntity::setSelectedFocusEntity(IWorldEntity* selectedFocusEntity) {
	if (this->selectedFocusEntity == selectedFocusEntity) {
		return;
	}

	onSelectedFocusEntityChange_pre();

	this->selectedFocusEntity = selectedFocusEntity;

	onSelectedFocusEntityChanged(selectedFocusEntity, selectedFocusEntity_autoListenersList);

	updateShowStateIfChanged(true, nullptr);
}

void MIGUIV_SelectedEntity::onSelectedFocusEntityChange_pre() {
	selectedFocusEntity_autoListenersList.clear();
}

void MIGUIV_SelectedEntity::onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) {
	//void
}

std::string MIGUIV_SelectedEntity::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (getSelectedFocusEntity() != nullptr) {
		state += "_selectedFocusEntity:true_";
	}

	return state;
}

void MIGUIV_SelectedEntity::disposeMainPre() {
	setSelectedFocusEntity(nullptr);

	super::disposeMainPre();
}

MIGUIV_SelectedEntity::~MIGUIV_SelectedEntity() {
	//void
}
