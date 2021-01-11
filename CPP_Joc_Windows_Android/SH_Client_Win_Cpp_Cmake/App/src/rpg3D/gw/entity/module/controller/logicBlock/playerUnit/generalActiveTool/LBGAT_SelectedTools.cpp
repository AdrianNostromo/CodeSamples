#include "LBGAT_SelectedTools.h"
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/template/userControlled/UserControlledTemplate.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <base/math/Math.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/util/WrappedTool.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionEvent.h>
#include <worldGame3D/gw/event/GWEvent.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
#include <worldGame3D/gw/entity/event/IPluggableEventsDispatcher.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

LBGAT_SelectedTools::LBGAT_SelectedTools()
	: super()
{
	//void
}

void LBGAT_SelectedTools::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::IToolsHandlerModule::EVENT_toolsHandler_tool_selectedAndRequiredGWCapabilitiesExist->type) {
		sp<ITool> tool = event.wrappedValue->getDirectAs_Custom<sp<ITool>>(-1);
		if (tool == nullptr) {
			throw LogicException(LOC);
		}

		hookTool(tool);
	}else if (event.type == rpg3D::IToolsHandlerModule::EVENT_toolsHandler_tool_deselectedOrRequiredGWCapabilitiesMissing->type) {
		sp<ITool> tool = event.wrappedValue->getDirectAs_Custom<sp<ITool>>(-1);
		if (tool == nullptr) {
			throw LogicException(LOC);
		}
		
		releaseHookedTool(tool);
	} else if (event.type == rpg3D::ITool::EVENT_tool_onIsOperatingChanged->type) {
		rpg3D::ITool* rawTool = static_cast<rpg3D::ITool*>(event.voidData);
		std::shared_ptr<util::WrappedTool> wTool = getWToolFromRawTool(rawTool);
		if (wTool == nullptr) {
			throw LogicException(LOC);
		}

		//asd_tool_1;
		if (!wTool->tool->getIsOperatingAny()) {
			// The activation filter is always enabled to allow for auto_activation tools to always block.
			//wTool->iFilter_innactiveForActivation.connect(userControlledModule->getInteractionsDispatcher());
			//userControlledModule->addManagedFiltersList(wTool->tool->getCustomFiltersList_innactiveForActivation());

			userControlledModule->removeManagedFiltersListIfExists(wTool->tool->getCustomFiltersList_activeContinuousForUpdate());
			wTool->iFilter_activeContinuousForUpdate.disconnect();
			
			userControlledModule->removeManagedFiltersListIfExists(wTool->tool->getCustomFiltersList_activeContinuousForDeactivation());
			wTool->iFilter_activeContinuousForDeactivation.disconnect();
		} else {
			wTool->preactivatedUpdateDeactivateFiltersDisableCooldownFrames = 0;

			// The activation filter is always enabled to allow for auto_activation tools to always block.
			//wTool->iFilter_innactiveForActivation.disconnect();
			//userControlledModule->removeManagedFiltersListIfExists(wTool->tool->getCustomFiltersList_innactiveForActivation());

			userControlledModule->addManagedFiltersList(wTool->tool->getCustomFiltersList_activeContinuousForUpdate());
			wTool->iFilter_activeContinuousForUpdate.connect(userControlledModule->getInteractionsDispatcher());

			userControlledModule->addManagedFiltersList(wTool->tool->getCustomFiltersList_activeContinuousForDeactivation());
			wTool->iFilter_activeContinuousForDeactivation.connect(userControlledModule->getInteractionsDispatcher());
		}
	}

    else {
        throw LogicException(LOC);
    }
}

void LBGAT_SelectedTools::onGWEvent_gameLoop_localInputPost(IEventListener& eventListener, base::GWEvent& event) {
	// All tools are sorted according to their filresPriority. This is used to make some tools block other tools input.
	// Enumerate all tools and keep track of the current tool filterPriority;

	int currentFilterPriority = -1;
	for (int i = 0; i < hookedToolsListOrdered.count(); i++) {
		std::shared_ptr<util::WrappedTool> wTool = hookedToolsListOrdered.getDirect(i);
		if (wTool->tool->getFiltersPriority() > currentFilterPriority) {
			// Current filter priority changed.
			currentFilterPriority = wTool->tool->getFiltersPriority();
		} else if (wTool->tool->getFiltersPriority() < currentFilterPriority) {
			// Error. The tools are not sorted by their filterPriority correctly.
			throw LogicException(LOC);
		}

		// Enumerate all filter events from the tool filter and discard the ones that have been already processed by a lower filterPriority.
		ArrayList<sp<BlockingFilterTriggerIndex>> blockingFilterTriggerIndexesList{};

		if (wTool->cachedInteractionsList.count() > 0) {
			for (int i = 0; i < wTool->cachedInteractionsList.count(); i++) {
				std::shared_ptr< util::WrappedTool::InteractionCacheEntry> filterInteraction = wTool->cachedInteractionsList.getDirect(i);

				if (filterInteraction->interaction->triggerEventIndex >= 0) {
					// Check if blocking is required and ignore the filter interaction.
					// Find the BlockingFilterTriggerIndex for filterInteraction->interaction->triggerEventIndex;
					sp<BlockingFilterTriggerIndex> blockingFilterTriggerIndex = getFilterWithTriggerIndex(&blockingFilterTriggerIndexesList, filterInteraction->interaction->triggerEventIndex);
					if (blockingFilterTriggerIndex != nullptr && blockingFilterTriggerIndex->blockerTool_filtersPriority < wTool->tool->getFiltersPriority()) {
						// Interaction blocked and ignored.
						continue;
					}

					if (blockingFilterTriggerIndex == nullptr) {
						// Insert a new blocking index.

						blockingFilterTriggerIndex = newt BlockingFilterTriggerIndex(
							filterInteraction->interaction->triggerEventIndex,
							wTool->tool->getFiltersPriority()
						);
						blockingFilterTriggerIndexesList.appendReference(blockingFilterTriggerIndex);
					}
				}

				bool isProcessed = wTool->tool->onFilterInteraction(filterInteraction->filterSource, filterInteraction->interaction);
				if (!isProcessed) {
					// Filter interraction must be processed.
					throw LogicException(LOC);
				}
			}

			wTool->cachedInteractionsList.clear();

			if (wTool->preactivatedUpdateDeactivateFiltersDisableCooldownFrames > 0) {
				wTool->preactivatedUpdateDeactivateFiltersDisableCooldownFrames--;

				if (wTool->preactivatedUpdateDeactivateFiltersDisableCooldownFrames <= 0) {
					//asd_tool_1;
					userControlledModule->removeManagedFiltersListIfExists(wTool->tool->getCustomFiltersList_activeContinuousForUpdate());
					wTool->iFilter_activeContinuousForUpdate.disconnect();

					userControlledModule->removeManagedFiltersListIfExists(wTool->tool->getCustomFiltersList_activeContinuousForDeactivation());
					wTool->iFilter_activeContinuousForDeactivation.disconnect();
				}
			}
		}
	}
}

sp<LBGAT_SelectedTools::BlockingFilterTriggerIndex> LBGAT_SelectedTools::getFilterWithTriggerIndex(ArrayList<sp<BlockingFilterTriggerIndex>>* blockingFilterTriggerIndexesList, int triggerEventIndex) {
	for (int i = 0; i < blockingFilterTriggerIndexesList->count(); i++) {
		sp<BlockingFilterTriggerIndex> blockingFilterTriggerIndex = blockingFilterTriggerIndexesList->getDirect(i);

		if (blockingFilterTriggerIndex->triggerEventIndex == triggerEventIndex) {
			return blockingFilterTriggerIndex;
		}
	}

	return nullptr;
}

void LBGAT_SelectedTools::onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::onIsActiveChanged(isActive_new, gwGameParts);

	if (!isActive_new) {
		localAutoListenersList.clear();

		// Release all tools.
		releaseAllTools();
	} else {
		// This is the new control type. Activate.
		e->addEventListener(
			rpg3D::IToolsHandlerModule::EVENT_toolsHandler_tool_selectedAndRequiredGWCapabilitiesExist->type, rpg3D::IToolsHandlerModule::EVENT_toolsHandler_tool_deselectedOrRequiredGWCapabilitiesMissing->type, rpg3D::ITool::EVENT_tool_onIsOperatingChanged->type,
			std::bind(&LBGAT_SelectedTools::onEvent, this, std::placeholders::_1, std::placeholders::_2),
			localAutoListenersList
		);

		e->getGWEvents()->addEventListener(
			base::IGameWorld::GWEvent_gameLoop_localInputPost->type,
			std::bind(&LBGAT_SelectedTools::onGWEvent_gameLoop_localInputPost, this, std::placeholders::_1, std::placeholders::_2),
			localAutoListenersList
		);

		if (toolsHandler->getSelectedToolsCount() > 0) {
			toolsHandler->getSelectedToolsList().enumerateDirect([this](ListDL<sp<Tool>>::Entry* listEntry, sp<Tool>& tool, bool& stopEnumeration) ->void {
				hookTool(tool);
			});
		}
	}
}

LBGAT_SelectedTools::~LBGAT_SelectedTools() {
	//void
}
