#include "THM_ToolsList.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

THM_ToolsList::THM_ToolsList(
	IWorldEntity* entity,
	ToolsHandlerTemplate* t)
	: super(
		entity,
		t
	)
{
	e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_simulationC->type,
		std::bind(&THM_ToolsList::onGWEvent_gameLoop_simulationC, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
	e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_localInputPost->type,
		std::bind(&THM_ToolsList::onGWEvent_gameLoop_localInputPost, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void THM_ToolsList::onGWEvent_gameLoop_simulationC(IEventListener& eventListener, base::GWEvent& event) {
	for (int i = 0; i < toolsList.size(); i++) {
		sp<Tool> tool = toolsList.getDirect(i);

		tool->onGameLoop_simulationC(event.deltaS, event.deltaMS, event.gameTimeS);
	}
}

void THM_ToolsList::onGWEvent_gameLoop_localInputPost(IEventListener& eventListener, base::GWEvent& event) {
	for (int i = 0; i < toolsList.size(); i++) {
		sp<Tool> tool = toolsList.getDirect(i);

		tool->onGameLoop_localInputPost(event.deltaS, event.deltaMS, event.gameTimeS);
	}
}

ArrayList<sp<Tool>>* THM_ToolsList::peekToolsList() {
	return &toolsList;
}

ArrayList<sp<Tool>>* THM_ToolsList::peekToolsList_userSelectable() {
	return &toolsList_userSelectable;
}

bool THM_ToolsList::hasTool(sp<Tool> tool) {
	if (toolsList.contains(tool)) {
		return true;
	}

	return false;
}

bool THM_ToolsList::hasTool_userSelectable(sp<Tool> tool) {
	if (toolsList_userSelectable.contains(tool)) {
		return true;
	}

	return false;
}

void THM_ToolsList::addTool(sp<Tool> tool, IAssetsManager* toolCreateAssetsManager) {
	if (!tool->getIsCreated()) {
		// The tool was not created. Some tools are constructed directly in the function call.
		// In this case, toolCreateAssetsManager must be provided to create the tool.
		if (toolCreateAssetsManager == nullptr) {
			// toolCreateAssetsManager must be provided to create the tool.

			throw LogicException(LOC);
		}

		tool->reservedCreate(toolCreateAssetsManager);
	}

	// Save the new tool.
	toolsList.appendReference(tool);

	if (tool->config->isUserSelectable) {
		toolsList_userSelectable.appendReference(tool);
	}

	tool->setEntity(entity);

	e->dispatchEvent(*EVENT_toolsHandler_toolAdded->setWrappedValue(base::IWrappedValue::new_Custom<sp<ITool>>(tool, -1)));
}

void THM_ToolsList::removeTool(sp<Tool> tool) {
	int i = toolsList.indexOf(tool);
	if (i < 0) {
		throw LogicException(LOC);
	}

	onToolRemovePre(tool);

	e->dispatchEvent(*EVENT_toolsHandler_toolRemovePre->setWrappedValue(base::IWrappedValue::new_Custom<sp<ITool>>(tool, -1)));

	if (tool->config->isUserSelectable) {
		toolsList_userSelectable.remove(tool);
	}

	// Tools are not disposed on removal, they are managed by sp.
	// Tools have their reservedDispose called from the sp automatically, not need to call it here also.
	//tool->reservedDisposeMain();

	if (gwGamePartsOptional != nullptr) {
		// This must be called here because the tool is not disposed here.
		tool->disconnectFromGamePartsPre();
	}
	
	toolsList.remove(tool);

	e->dispatchEvent(*EVENT_toolsHandler_toolRemoved);
}

void THM_ToolsList::onToolRemovePre(sp<Tool> tool) {
	//void
}

int THM_ToolsList::getUserSelectableToolsCount() {
	return toolsList_userSelectable.count();
}

sp<Tool> THM_ToolsList::getFirstToolWithTags(ArrayList<std::string>& tagsList) {
	for (int i = 0; i < toolsList.size(); i++) {
		sp<Tool> tool = toolsList.getDirect(i);
		if (tool->hasTags(tagsList)) {
			return tool;
		}
	}

	return nullptr;
}

sp<Tool> THM_ToolsList::getFirstToolWithTags(Array1D<std::string>& tagsList) {
	for (int i = 0; i < toolsList.size(); i++) {
		sp<Tool> tool = toolsList.getDirect(i);
		if (tool->hasTags(tagsList)) {
			return tool;
		}
	}

	return nullptr;
}

sp<Tool> THM_ToolsList::getFirstToolWithTags(std::string& singleTag) {
	for (int i = 0; i < toolsList.size(); i++) {
		sp<Tool> tool = toolsList.getDirect(i);
		if (tool->hasTags(singleTag)) {
			return tool;
		}
	}

	return nullptr;
}

std::shared_ptr<ArrayList<sp<Tool>>> THM_ToolsList::getAllToolWithTags(ArrayList<std::string>& tagsList) {
	std::shared_ptr<ArrayList<sp<Tool>>> ret = nullptr;

	for (int i = 0; i < toolsList.size(); i++) {
		sp<Tool> tool = toolsList.getDirect(i);
		if (tool->hasTags(tagsList)) {
			if (ret == nullptr) {
				ret = std::make_shared<ArrayList<sp<Tool>>>();
			}

			ret->appendDirect(tool);
		}
	}

	return ret;
}

std::shared_ptr<ArrayList<sp<Tool>>> THM_ToolsList::getAllToolWithTags(Array1D<std::string>& tagsList) {
	std::shared_ptr<ArrayList<sp<Tool>>> ret = nullptr;

	for (int i = 0; i < toolsList.size(); i++) {
		sp<Tool> tool = toolsList.getDirect(i);
		if (tool->hasTags(tagsList)) {
			if (ret == nullptr) {
				ret = std::make_shared<ArrayList<sp<Tool>>>();
			}

			ret->appendDirect(tool);
		}
	}

	return ret;
}

std::shared_ptr<ArrayList<sp<Tool>>> THM_ToolsList::getAllToolWithTags(std::string& singleTag) {
	std::shared_ptr<ArrayList<sp<Tool>>> ret = nullptr;

	for (int i = 0; i < toolsList.size(); i++) {
		sp<Tool> tool = toolsList.getDirect(i);
		if (tool->hasTags(singleTag)) {
			if (ret == nullptr) {
				ret = std::make_shared<ArrayList<sp<Tool>>>();
			}

			ret->appendDirect(tool);
		}
	}

	return ret;
}

bool THM_ToolsList::hasToolWithTags(ArrayList<std::string>& tagsList) {
	if (getFirstToolWithTags(tagsList) != nullptr) {
		return true;
	}

	return false;
}

bool THM_ToolsList::hasToolWithTags(Array1D<std::string>& tagsList) {
	if (getFirstToolWithTags(tagsList) != nullptr) {
		return true;
	}

	return false;
}

bool THM_ToolsList::hasToolWithTags(std::string singleTag) {
	if (getFirstToolWithTags(singleTag) != nullptr) {
		return true;
	}

	return false;
}

void THM_ToolsList::disposePre() {
	while (toolsList.size() > 0) {
		removeTool(toolsList.getDirect(0));
	}

	super::disposePre();
}

THM_ToolsList::~THM_ToolsList() {
	//void
}
