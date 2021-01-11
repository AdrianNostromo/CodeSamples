#pragma once

#include <base/gh.h>
#include "THM_ToolsList.h"
#include <base/MM.h>

namespace rpg3D {
class THM_SelectedTools : public THM_ToolsList {priv typedef THM_ToolsList super; pub dCtor(THM_SelectedTools);
	// This contains all selected tools.
	// These tools may not be in the u.i. tools list (eg. stackable tools).
	priv ListDL<sp<Tool>> selectedToolsList{LOC};
	// These are used for tool selector u.i. code.
	priv ListDL<sp<Tool>> selectedToolsList_userSelectable{LOC};
	// These are the occupied slots.
	// A slot may have only 1 tool.
	priv Array1D<sp<Tool>> slotSelectedToolsList;

    pub explicit THM_SelectedTools(
		IWorldEntity* entity,
		ToolsHandlerTemplate* t);

	prot void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	prot void disconnectFromGamePartsPre() override;

	pub int getSelectedToolsCount() final;
	pub ListDL<sp<Tool>>& getSelectedToolsList() final;
	
	pub int getSelectedToolsCount_userSelectable() final;
	pub ListDL<sp<Tool>>& getSelectedToolsList_userSelectable() final;

	pub sp<Tool> getToolAtSlot(int toolSlotIndex) final;

	pub void selectTool(sp<Tool> tool, int toolSlotIndex) final;
	
	pub void deselectTool(sp<Tool> tool, bool dispatchSlotEvents =true) final;
	pub void deselectAllToolsAtSlot(int toolSlotIndex) final;

	priv void onToolIsSelectedAndGWCapabilitiesExistChanged(ITool* target, bool isSelectedAndGWCapabilitiesExist);

	pub void selectSingleToolInGroup(int groupId, int toolId, int toolSlotIndex) final;

	prot void onToolRemovePre(sp<Tool> tool) override;

	prot void disposePre() override;
    pub ~THM_SelectedTools() override;
};

};
