#pragma once

#include <base/gh.h>
#include "T_Events.h"

namespace rpg3D {
	class WorldOrEntityInteractionFilter;
};

namespace rpg3D {
class T_Filters : public T_Events {priv typedef T_Events super;pub dCtor(T_Filters);
    //asd_3;// remove these filters after new system replaces all filter ussages.
	// This is enabled always and can allow for multiple fingers tools (eg: editor_tool zoom change with 2 fingers).
	prot std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> activationFiltersList_innactiveForActivation = std::make_shared<ArrayList<rpg3D::WorldOrEntityInteractionFilter>>();
	// This is enabled only when tool is active_continuous.
	prot std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> activationFiltersList_activeContinuousForUpdate = std::make_shared<ArrayList<rpg3D::WorldOrEntityInteractionFilter>>();
	prot std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> activationFiltersList_activeContinuousForDeactivation = std::make_shared<ArrayList<rpg3D::WorldOrEntityInteractionFilter>>();
	prot std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> activationFiltersList_alwaysUpdate = std::make_shared<ArrayList<rpg3D::WorldOrEntityInteractionFilter>>();
	
	// This is active only when the tool is innactive.
	prot rpg3D::ManagedFilersList customFiltersList_innactiveForActivation{};
	// This is enabled only when tool is active_continuous.
	prot rpg3D::ManagedFilersList customFiltersList_activeContinuousForUpdate{};
	prot rpg3D::ManagedFilersList customFiltersList_activeContinuousForDeactivation{};
	prot rpg3D::ManagedFilersList customFiltersList_alwaysUpdate{};

	pub explicit T_Filters(ToolConfigBase* config);

	pub int getFiltersPriority() final;

	pub std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> getActivationFiltersList_innactiveForActivation() final;
	pub std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> getActivationFiltersList_activeContinuousForUpdate() final;
	pub std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> getActivationFiltersList_activeContinuousForDeactivation() final;
	pub std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> getActivationFiltersList_alwaysUpdate() final;

	pub rpg3D::ManagedFilersList* getCustomFiltersList_innactiveForActivation() final;
	pub rpg3D::ManagedFilersList* getCustomFiltersList_activeContinuousForUpdate() final;
	pub rpg3D::ManagedFilersList* getCustomFiltersList_activeContinuousForDeactivation() final;
	pub rpg3D::ManagedFilersList* getCustomFiltersList_alwaysUpdate() final;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

    pub ~T_Filters() override;
};
};
