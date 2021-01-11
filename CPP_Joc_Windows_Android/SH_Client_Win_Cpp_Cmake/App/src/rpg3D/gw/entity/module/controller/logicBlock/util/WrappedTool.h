#pragma once

#include <base/gh.h>
#include <memory>
#include <rpg3D/gw/entity/module/toolsHandler/tool/ITool.h>
#include <rpg3D/gw/entity/module/userControlled/util/InteractionFiltersGroup.h>

namespace rpg3D {
	class WorldOrEntityInteractionEvent;
};

namespace rpg3D {
namespace util {
class WrappedTool {
public:
	class InteractionCacheEntry {
		pub int filterSource;
		pub std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent> interaction;

		pub InteractionCacheEntry(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction)
			: filterSource(filterSource), interaction(interaction)
		{
			//void
		}
	};

	pub sp<rpg3D::ITool> tool;

	//asd_tool_1;// maybe can remove the intermediate groups from the new system.
	pub rpg3D::InteractionFiltersGroup iFilter_innactiveForActivation;
	pub rpg3D::InteractionFiltersGroup iFilter_activeContinuousForUpdate;
	pub rpg3D::InteractionFiltersGroup iFilter_activeContinuousForDeactivation;
	pub rpg3D::InteractionFiltersGroup iFilter_alwaysUpdate;

	ArrayList<std::shared_ptr< InteractionCacheEntry>> cachedInteractionsList{};

	// On tool activation input, the update and deactivate filters are pre-activated (to avoid input dropping) for a few frames.
	// On tool confirmed continuous activation, this cooldown is disabled. It is used only for failed activations (should never occur but may).
	// This is usually 2. That skips the current frame and deactivates on the next one.
	pub int preactivatedUpdateDeactivateFiltersDisableCooldownFrames = 0;

	// This is used to disabled auto activation tools when needed (eg disabled while the entity_editor is active).
	pub bool isFiltersActivationEnabled = false;

public:
	pub dCtor(WrappedTool);
    pub explicit WrappedTool(sp<rpg3D::ITool> tool);

	priv void onEntityInteraction_innactiveForActivation(IEventListener& eventListener, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction);
	priv void onEntityInteraction_activeContinuousForUpdate(IEventListener& eventListener, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction);
	priv void onEntityInteraction_activeContinuousForDeactivation(IEventListener& eventListener, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction);
	priv void onEntityInteraction_alwaysUpdate(IEventListener& eventListener, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction);

    pub virtual ~WrappedTool();
};
};
};
