#include "T_Filters.h"
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionFilter.h>

using namespace rpg3D;

T_Filters::T_Filters(ToolConfigBase* config)
	: super(config)
{
	//void
}

int T_Filters::getFiltersPriority() {
	return config->filtersPriority;
}

std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> T_Filters::getActivationFiltersList_innactiveForActivation() {
    return activationFiltersList_innactiveForActivation;
}

std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> T_Filters::getActivationFiltersList_activeContinuousForUpdate() {
    return activationFiltersList_activeContinuousForUpdate;
}

std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> T_Filters::getActivationFiltersList_activeContinuousForDeactivation() {
    return activationFiltersList_activeContinuousForDeactivation;
}

std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> T_Filters::getActivationFiltersList_alwaysUpdate() {
    return activationFiltersList_alwaysUpdate;
}

rpg3D::ManagedFilersList* T_Filters::getCustomFiltersList_innactiveForActivation() {
    return &customFiltersList_innactiveForActivation;
}

rpg3D::ManagedFilersList* T_Filters::getCustomFiltersList_activeContinuousForUpdate() {
    return &customFiltersList_activeContinuousForUpdate;
}

rpg3D::ManagedFilersList* T_Filters::getCustomFiltersList_activeContinuousForDeactivation() {
    return &customFiltersList_activeContinuousForDeactivation;
}

rpg3D::ManagedFilersList* T_Filters::getCustomFiltersList_alwaysUpdate() {
    return &customFiltersList_alwaysUpdate;
}

bool T_Filters::onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
    //void

    return false;
}

T_Filters::~T_Filters() {
	//void
}
