#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IToolsHandlerModule.h"

namespace rpg3D {
	class ToolsHandlerTemplate;
};

namespace rpg3D {
class THM_Base : public EntityModule, virtual public IToolsHandlerModule {priv typedef EntityModule super;pub dCtor(THM_Base);
	prot ToolsHandlerTemplate* t;

	priv int isNonUserSelectableToolsEnabledIncremental = 1;

	// This is used to disable continuous activation for tool when normal tools are used in turn based logic.
	// This is only set by the logic blocks without any error checking.
	// This is true for hooked_tools that are in turn_based_active_tool_logic_blocks.
	// This can be enabled,disabled multiple times and the tools will work fine.
	priv bool isTurnBasedToolsModeActive = false;
	// This is used for turn_based_mode tools to only trigger once.
	// The tools saves this counter when it triggers_pointOfNoReturn and when the action tries to trigger again, it checks this var against the saved one and will stop operating the tool.
	// This is incremented on each start_state of the turn based turn logic.
	priv int turnBasedActionStateCounter = 0;

	pub bool getIsTurnBasedToolsModeActive() final;
	pub void setIsTurnBasedToolsModeActive(bool isTurnBasedToolsModeActive) final;
	
	pub int getTurnBasedActionStateCounter() final;
	pub void incrementTurnBasedActionStateCounter() final;

    pub explicit THM_Base(
		IWorldEntity* entity,
		ToolsHandlerTemplate* t);

	pub bool getIsAutomaticToolsEnabled() override;
	// These are the move to target, activate switches, buttons, levers, doors tools. Active selectable tools are not affected b this).
	pub void setIsNonUserSelectableToolsEnabledIncremental(bool isEnabled) override;

    pub ~THM_Base() override;
};
};
