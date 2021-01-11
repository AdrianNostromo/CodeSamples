#pragma once

#include <base/gh.h>
#include "THM_Base.h"
#include <worldGame3D/gw/event/GWEvent.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>

namespace rpg3D {
class THM_ToolsList : public THM_Base {priv typedef THM_Base super;
	prot ArrayList<sp<Tool>> toolsList{};
	prot ArrayList<sp<Tool>> toolsList_userSelectable{};

	pub dCtor(THM_ToolsList);
    pub explicit THM_ToolsList(
		IWorldEntity* entity,
		ToolsHandlerTemplate* t);

	priv void onGWEvent_gameLoop_simulationC(IEventListener& eventListener, base::GWEvent& event);

	priv void onGWEvent_gameLoop_localInputPost(IEventListener& eventListener, base::GWEvent& event);

	pub ArrayList<sp<Tool>>* peekToolsList() override;
	pub ArrayList<sp<Tool>>* peekToolsList_userSelectable() override;

	pub bool hasTool(sp<Tool> tool) final;
	pub bool hasTool_userSelectable(sp<Tool> tool) final;

	pub void addTool(sp<Tool> tool, IAssetsManager* toolCreateAssetsManager = nullptr) final;
	pub void removeTool(sp<Tool> tool) final;
	prot virtual void onToolRemovePre(sp<Tool> tool);

	pub int getUserSelectableToolsCount() final;

	pub sp<Tool> getFirstToolWithTags(ArrayList<std::string>& tagsList) final;
	pub sp<Tool> getFirstToolWithTags(Array1D<std::string>& tagsList) final;
	pub sp<Tool> getFirstToolWithTags(std::string& singleTag) final;
	
	pub std::shared_ptr<ArrayList<sp<Tool>>> getAllToolWithTags(ArrayList<std::string>& tagsList) final;
	pub std::shared_ptr<ArrayList<sp<Tool>>> getAllToolWithTags(Array1D<std::string>& tagsList) final;
	pub std::shared_ptr<ArrayList<sp<Tool>>> getAllToolWithTags(std::string& singleTag) final;
	
	pub bool hasToolWithTags(ArrayList<std::string>& tagsList) final;
	pub bool hasToolWithTags(Array1D<std::string>& tagsList) final;
	pub bool hasToolWithTags(std::string singleTag) final;
	
	prot void disposePre() override;
	pub ~THM_ToolsList() override;
};

};
