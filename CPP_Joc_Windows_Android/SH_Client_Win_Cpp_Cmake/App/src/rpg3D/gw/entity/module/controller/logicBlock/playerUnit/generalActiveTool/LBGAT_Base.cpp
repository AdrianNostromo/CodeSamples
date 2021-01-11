#include "LBGAT_Base.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/template/userControlled/UserControlledTemplate.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

LBGAT_Base::LBGAT_Base()
	: super(0.0f/*tickCooldownS*/)
{
	//void
}

void LBGAT_Base::onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::onIsActiveChanged(isActive_new, gwGameParts);

	if (!isActive_new) {
		//void
	} else {
		// This is the new control type. Activate.
		if (userControlledModule == nullptr) {
			userControlledModule = entity->getComponentAs<rpg3D::IUserControlledModule*>(true/*mustExist*/);
		}
	}
}

void LBGAT_Base::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::connectToGameParts(gwGameParts);

	syncToolsFilters();
}

std::shared_ptr<util::WrappedTool> LBGAT_Base::getWToolFromTool(sp<rpg3D::ITool> tool) {
	for (int i = 0; i < hookedToolsListOrdered.count(); i++) {
		std::shared_ptr<util::WrappedTool> wTool = hookedToolsListOrdered.getDirect(i);

		if (wTool->tool == tool) {
			return wTool;
		}
	}

	return nullptr;
}

std::shared_ptr<util::WrappedTool> LBGAT_Base::getWToolFromRawTool(rpg3D::ITool* rawTool) {
	for (int i = 0; i < hookedToolsListOrdered.count(); i++) {
		std::shared_ptr<util::WrappedTool> wTool = hookedToolsListOrdered.getDirect(i);

		if (wTool->tool.get() == rawTool) {
			return wTool;
		}
	}

	return nullptr;
}

void LBGAT_Base::hookTool(sp<rpg3D::ITool> tool) {
	if (getWToolFromTool(tool) != nullptr) {
		throw LogicException(LOC);
	}

	std::shared_ptr<util::WrappedTool> wTool = std::make_shared<util::WrappedTool>(tool);

	// The tool is inserted according to it's filtersPriority.
	int insertIndex = 0;
	for (int i = hookedToolsListOrdered.count() - 1; i >= 0; i--) {
		std::shared_ptr<util::WrappedTool> existingWTool = hookedToolsListOrdered.getDirect(i);

		if (wTool->tool->getFiltersPriority() >= existingWTool->tool->getFiltersPriority()) {
			insertIndex = i + 1;

			break;
		}
	}
	hookedToolsListOrdered.insertReference(insertIndex, wTool);

	if (wTool->isFiltersActivationEnabled) {
		throw LogicException(LOC);
	}

	onToolHooked(wTool);
}

void LBGAT_Base::onToolHooked(std::shared_ptr<util::WrappedTool> wTool) {
	if (shouldManageConnectTool(wTool)) {
		managedConnectToolFilters(wTool);
	}
}

void LBGAT_Base::releaseAllTools() {
	for (int i = hookedToolsListOrdered.count() - 1; i >= 0; i--) {
		std::shared_ptr<util::WrappedTool> wTool = hookedToolsListOrdered.getDirect(i);

		releaseWTool(wTool);
	}
}

void LBGAT_Base::releaseWTool(std::shared_ptr<util::WrappedTool> wTool) {
	if (wTool->isFiltersActivationEnabled) {
		managedDisconnectToolFilters(wTool);
	}

	wTool->iFilter_innactiveForActivation.releaseFiltersListBorrowed();
	wTool->iFilter_activeContinuousForUpdate.releaseFiltersListBorrowed();
	wTool->iFilter_activeContinuousForDeactivation.releaseFiltersListBorrowed();
	wTool->iFilter_alwaysUpdate.releaseFiltersListBorrowed();

	if (!hookedToolsListOrdered.removeReference(wTool)) {
		throw LogicException(LOC);
	}
}

void LBGAT_Base::releaseHookedTool(sp<rpg3D::ITool> tool) {
	std::shared_ptr<util::WrappedTool> wTool = getWToolFromTool(tool);
	if (wTool == nullptr) {
		throw LogicException(LOC);
	}

	releaseWTool(wTool);
}

void LBGAT_Base::managedConnectToolFilters(std::shared_ptr<util::WrappedTool> wTool) {
	if (!wTool->isFiltersActivationEnabled) {
		//GlobalAccessLogHandler::devLogHandler->post_debug("managedConnectToolFilters id: %s", wTool->tool->getConfig()->id.c_str());
		
		wTool->isFiltersActivationEnabled = true;

		userControlledModule->addManagedFiltersList(wTool->tool->getCustomFiltersList_innactiveForActivation());
		userControlledModule->addManagedFiltersList(wTool->tool->getCustomFiltersList_alwaysUpdate());
		
		wTool->iFilter_innactiveForActivation.connect(userControlledModule->getInteractionsDispatcher());
		wTool->iFilter_alwaysUpdate.connect(userControlledModule->getInteractionsDispatcher());
	}
}

void LBGAT_Base::managedDisconnectToolFilters(std::shared_ptr<util::WrappedTool> wTool) {
	if (wTool->isFiltersActivationEnabled) {
		wTool->isFiltersActivationEnabled = false;

		GlobalAccessLogHandler::devLogHandler->post_debug("managedDisconnectToolFilters id: %s", wTool->tool->getConfig()->id.c_str());

		userControlledModule->removeManagedFiltersListIfExists(wTool->tool->getCustomFiltersList_innactiveForActivation());
		userControlledModule->removeManagedFiltersListIfExists(wTool->tool->getCustomFiltersList_activeContinuousForUpdate());
		userControlledModule->removeManagedFiltersListIfExists(wTool->tool->getCustomFiltersList_activeContinuousForDeactivation());
		userControlledModule->removeManagedFiltersListIfExists(wTool->tool->getCustomFiltersList_alwaysUpdate());

		wTool->iFilter_innactiveForActivation.disconnect();
		wTool->iFilter_activeContinuousForUpdate.disconnect();
		wTool->iFilter_activeContinuousForDeactivation.disconnect();
		wTool->iFilter_alwaysUpdate.disconnect();
	}
}

void LBGAT_Base::syncToolsFilters() {
	for (int i = hookedToolsListOrdered.count() - 1; i >= 0; i--) {
		std::shared_ptr<util::WrappedTool> wTool = hookedToolsListOrdered.getDirect(i);

		if (shouldManageConnectTool(wTool)) {
			managedConnectToolFilters(wTool);
		} else {
			managedDisconnectToolFilters(wTool);
		}
	}
}

bool LBGAT_Base::shouldManageConnectTool(std::shared_ptr<util::WrappedTool> wTool) {
	if (!getIsActive() || !getIsZoneGamePartsReady()) {
		return false;
	}

	return true;
}

LBGAT_Base::~LBGAT_Base() {
	//void
}
