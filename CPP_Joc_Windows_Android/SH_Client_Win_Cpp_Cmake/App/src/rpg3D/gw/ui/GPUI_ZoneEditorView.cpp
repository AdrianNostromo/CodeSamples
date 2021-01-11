#include "GPUI_ZoneEditorView.h"
#include <base/app/IApp.h>
#include <base/view/debug/entryUser/IDebugEntryUser.h>
#include <base/view/debug/IDebugView.h>
#include <functional>
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/view/zoneEditor/ZoneEditorView.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <rpg3D/gw/interaction/deviceinput/IManagedDeviceInput.h>
#include <worldGame3D/gw/interaction/IGPInteraction.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <rpg3D/gw/entity/module/zoneEditor/IZoneEditorModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/toolsHandler/ToolsHandlerModule.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <rpg3D/gw/entity/template/zoneEditor/ZoneEditorTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/zoneEditor/config/ToolConfigZoneEditor.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/zoneEditor/ToolZoneEditor.h>
#include <rpg3D/gw/contentZones/zone/contentData/ZoneContentData.h>
#include <base/assets/cachesHandler/ICachesGroupsHandler.h>
#include <base/assets/cachesHandler/cacheGroup/ICacheGroup.h>
#include <worldGame3D/gw/contentZones/IGamePartContentZones.h>
#include <worldGame3D/gw/contentZones/zone/ContentZone.h>
#include <base/viewsHandler/IViewsHandler.h>

using namespace rpg3D;

GPUI_ZoneEditorView::GPUI_ZoneEditorView(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		base::IGameWorld::GWEvent_dispose_views->type, base::IGameWorld::GWEvent_saveData->type,
		std::bind(&GPUI_ZoneEditorView::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	viewHideAnimationListener.cb_onAnimationFinished = std::bind(&GPUI_ZoneEditorView::onViewHideAnimationFinished, this, std::placeholders::_1);
}

void GPUI_ZoneEditorView::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == base::IGameWorld::GWEvent_dispose_views->type) {
		if (zoneEditorView != nullptr) {
			gw->getViewComponentsHandler()->removeComponent(zoneEditorView, true/*doDispose*/);
			
			zoneEditorView = nullptr;
			gw->dispatchEvent(*IGamePartUserInterface::GWEvent_onZoneEditor_deactivated);
		}
	} else if (event.type == base::IGameWorld::GWEvent_saveData->type) {
		if (isEditorActive) {
			stopZoneEditor(false/*doAnimation*/);
		}
	}

	else {
		throw LogicException(LOC);
	}
}

void GPUI_ZoneEditorView::createBMain() {
	super::createBMain();

	if (debugViewEntry_StartZoneEditor.dde == nullptr && gw->getApp()->getComponentAs<base::IViewsHandler*>(true/*mustExist*/)->getDebugView() != nullptr) {
		debugViewEntry_StartZoneEditor.dde = gw->getApp()->getComponentAs<base::IViewsHandler*>(true/*mustExist*/)->getDebugView()->addDebugEntry(1/*linesCount*/, false/*hasToggleSwitch*/, -1/*pageIndex*/, true/*hasBtn*/);
		debugViewEntry_StartZoneEditor.dde->setPlaceholderedText(">> Start zone editor");
		debugViewEntry_StartZoneEditor.dde->setCb_onBtnCursorDown(std::bind(&GPUI_ZoneEditorView::onStartZoneEditorBtnCursorDown, this, std::placeholders::_1));
	}
}

void GPUI_ZoneEditorView::onStartZoneEditorBtnCursorDown(base::IDebugEntryUser* debugEntryUser) {
	startZoneEditor();
}

void GPUI_ZoneEditorView::startZoneEditor() {
	if (isEditorActive || zoneEditorView != nullptr) {
		// Editor already active or hiding.
		return;
	}
	isEditorActive = true;

	GlobalAccessLogHandler::devLogHandler->post_debug("Enabling zone editor.");

	// Disable the in_game U.I.
	activateMainUIMinimalModeIncremental();

	// Do a player entity stop action.
	editorEntity = gw->getContent()->getSelectedPlayerEntity(true/*mustExist*/);
	
	createNewView();

	// Initialise the edit modules on the editable and editor entities.
	rpg3D::zoneEditorModule::IZoneEditorModule* editorEntity_zoneEditor = editorEntity->getComponentAs<rpg3D::zoneEditorModule::IZoneEditorModule*>(true/*mustExist*/);

	editorEntity_zoneEditor->startEdit(
		this,
		zoneEditorView,
		gw->getGPMain()->getCC()
	);

	//// Create and activate the editor tool.
	addEditorTools(editorEntity);

	zoneEditorView->show(
		true, nullptr, nullptr,
		gw->getInteractionAs<base::IGPInteraction*>()->getInputAs<IManagedDeviceInput*>()->getBlockingInputLayer()
	);

	gw->dispatchEvent(*IGamePartUserInterface::GWEvent_onZoneEditor_activated);
}

void GPUI_ZoneEditorView::addEditorTools(IWorldEntity* editorEntity) {
	rpg3D::IToolsHandlerModule* entity_toolsHandler = editorEntity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);
	// These are the move to target, activate switches, buttons, levers, doors tools. Active selectable tools are not affected b this).
	entity_toolsHandler->setIsNonUserSelectableToolsEnabledIncremental(false);

	preEditorToolAtSlot = entity_toolsHandler->getToolAtSlot(0);

	ArrayList< rpg3D::ToolConfigBase*> editorToolConfigsList{};
	editorToolConfigsList.appendDirect_chain(EntityTemplatesList::Tool_ToolZEGroundTiled)
		->appendDirect_chain(EntityTemplatesList::Tool_ToolZEWallsFullTiled)
		->appendDirect_chain(EntityTemplatesList::Tool_ToolZEAssets)
		->appendDirect_chain(EntityTemplatesList::Tool_ToolZESun);

	rpg3D::ToolConfigBase* defaultEditorToolConfig = EntityTemplatesList::Tool_ToolZEGroundTiled;

	for (int i = 0; i < editorToolConfigsList.count(); i++) {
		rpg3D::ToolConfigBase* toolCfg = editorToolConfigsList.getDirect(i);

		sp<rpg3D::Tool> tool = toolCfg->createNewTool(
			gw->getAppAssets()
		);
		sp<rpg3D::ToolZoneEditor> cTool = tool.dCast<rpg3D::ToolZoneEditor>(true/*mustExtend*/);

		entity_toolsHandler->addTool(
			cTool
		);

		if (toolCfg == defaultEditorToolConfig) {
			entity_toolsHandler->selectTool(
				cTool,
				0
			);
		}
	}
}

void GPUI_ZoneEditorView::removeEditorTools(IWorldEntity* editorEntity) {
	rpg3D::IToolsHandlerModule* entity_toolsHandler = editorEntity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);

	std::shared_ptr<ArrayList<sp<Tool>>> toolsList = entity_toolsHandler->getAllToolWithTags(rpg3D::ToolConfigZoneEditor::TAG_zoneEditor);
	if (toolsList != nullptr) {
		for (int i = 0; i < toolsList->count(); i++) {
			sp<Tool> tool = toolsList->getDirect(i);

			entity_toolsHandler->removeTool(tool);
		}
	}

	if (entity_toolsHandler->getToolAtSlot(0) == nullptr && preEditorToolAtSlot != nullptr && entity_toolsHandler->hasTool_userSelectable(preEditorToolAtSlot)) {
		entity_toolsHandler->selectTool(
			preEditorToolAtSlot,
			0
		);
	}
	preEditorToolAtSlot = nullptr;

	// Enable previously disabled auto_tools activation.
	entity_toolsHandler->setIsNonUserSelectableToolsEnabledIncremental(true);
}

void GPUI_ZoneEditorView::zoneEditor_requestStopEditor() {
	stopZoneEditor(true/*doAnimation*/);
}

ZoneEditorView* GPUI_ZoneEditorView::createNewView() {
	GlobalAccessLogHandler::devLogHandler->post_debug("Creating a new zone editor.");

	if (zoneEditorView != nullptr) {
		throw LogicException(LOC);
	}

	gw->getViewComponentsHandler()->addComponent(zoneEditorView = new ZoneEditorView(
		gw->getHandler()->getApp(), gw->getHandler()->getAppUtils()
	), true/*doCreate*/);
	gw->getGPMain()->getLayerSubUserInterface()->addChild(zoneEditorView->getVisualsHolder());

	return zoneEditorView;
}

void GPUI_ZoneEditorView::stopZoneEditor(bool doAnimation) {
	if (!isEditorActive) {
		return;
	}
	isEditorActive = false;

	GlobalAccessLogHandler::devLogHandler->post_debug("Disabling zone editor.");

	if (zoneEditorView == nullptr) {
		throw LogicException(LOC);
	}

	rpg3D::zoneEditorModule::IZoneEditorModule* editorEntity_zoneEditor = editorEntity->getComponentAs<rpg3D::zoneEditorModule::IZoneEditorModule*>(true/*mustExist*/);
	editorEntity_zoneEditor->endEditIfAny();

	// The painter view is busy, add it to the destroying list.
	zoneEditorView->hide(doAnimation, &viewHideAnimationListener);

	// Deactivate and remove the editor tool.
	removeEditorTools(editorEntity);

	// Do a template instant save.
	zoneEditor_onEditorClosed();

	// Enable the in_game U.I.
	deactivateMainUIMinimalModeIncremental();
}

void GPUI_ZoneEditorView::zoneEditor_onEditorClosed() {
	std::shared_ptr<std::stringstream> levelData = rpg3D::ZoneContentData::Serialise(gw);

	worldGame3D::IGamePartContentZones* gpContentZones = gw->getComponentAs<worldGame3D::IGamePartContentZones*>(true/*mustExist*/);
	if (gpContentZones == nullptr || gpContentZones->getActiveContentZone() == nullptr) {
		throw LogicException(LOC);
	};
	std::string& zoneId = gpContentZones->getActiveContentZone()->getId();
	std::string cacheEntryId = zoneId + "/zoneData.dat";

	gw->getHandler()->getApp()->getEmbeddedAssetsManager()->getCacheGroupsHandler()
		->getCacheGroup(gw->getScenarioTemplate()->id)->setCache(cacheEntryId, levelData);
}

void GPUI_ZoneEditorView::onViewHideAnimationFinished(IGenericDrawable* target) {
	// This listener is used only for the panel hide.

	GlobalAccessLogHandler::devLogHandler->post_debug("Zone editor hide finished, disposing it.");

	if (zoneEditorView != target) {
		throw LogicException(LOC);
	}

	gw->getViewComponentsHandler()->removeComponent(zoneEditorView, true/*doDispose*/, true/*useDelayedRemoval*/);
	zoneEditorView = nullptr;

	gw->dispatchEvent(*IGamePartUserInterface::GWEvent_onZoneEditor_deactivated);
}

void GPUI_ZoneEditorView::disposePrePre() {
	if (debugViewEntry_StartZoneEditor.dde != nullptr) {
		debugViewEntry_StartZoneEditor.dde->remove();
		debugViewEntry_StartZoneEditor.dde = nullptr;
	}
	if (zoneEditorView != nullptr) {
		isEditorActive = false;

		gw->getViewComponentsHandler()->removeComponent(zoneEditorView, true/*doDispose*/);

		zoneEditorView = nullptr;
	}

	super::disposePrePre();
}

GPUI_ZoneEditorView::~GPUI_ZoneEditorView() {
	//void
}
