#pragma once

#include <base/gh.h>
#include "LBGAT_ExtraControls.h"
#include <worldGame3D/gw/entity/event/EntityEvent.h>
#include <base/memory/SharedPointer.h>

namespace base {
	class GWEvent;
};
namespace rpg3D {
	class IUserControlledModule;
};

namespace rpg3D {
namespace playerUnit {
class LBGAT_SelectedTools : public LBGAT_ExtraControls {priv typedef LBGAT_ExtraControls super; pub dCtor(LBGAT_SelectedTools);
	priv class BlockingFilterTriggerIndex {
		// Filter interractions with this exact value will be blocked if the tested_tool filtersPriority != this->blockerTool_filtersPriority;
		pub int triggerEventIndex;

		pub int blockerTool_filtersPriority;

		pub explicit BlockingFilterTriggerIndex(int triggerEventIndex, int blockerTool_filtersPriority)
			: triggerEventIndex(triggerEventIndex), blockerTool_filtersPriority(blockerTool_filtersPriority)
		{}
	};

	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit LBGAT_SelectedTools();

	prot void onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) override;

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	priv void onGWEvent_gameLoop_localInputPost(IEventListener& eventListener, base::GWEvent& event);

	priv sp<BlockingFilterTriggerIndex> getFilterWithTriggerIndex(ArrayList<sp<BlockingFilterTriggerIndex>>* blockingFilterTriggerIndexesList, int triggerEventIndex);

    pub ~LBGAT_SelectedTools() override;
};
};
};
