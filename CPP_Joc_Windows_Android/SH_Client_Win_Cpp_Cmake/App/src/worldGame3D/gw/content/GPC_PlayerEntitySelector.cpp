#include "GPC_PlayerEntitySelector.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/userEntitiesController/IGPUserEntitiesController.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionFilter.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionEvent.h>
#include <rpg3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/util/WrappedWorldEntity.h>
#include <worldGame3D/gw/sim/GamePartSimExtendable.h>
#include <worldGame3D/gw/entityGroups/util/IEntsGroup.h>
#include <base/app/config/AppConfig.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace base;

const bool GPC_PlayerEntitySelector::IS_DEBUG_LOCAL = true;

GPC_PlayerEntitySelector::GPC_PlayerEntitySelector(IGameWorld* gw)
    : super(gw)
{
    //void
}

void GPC_PlayerEntitySelector::createBMain() {
	super::createBMain();

	gpContent = gw->getGPContent();
	gpUserEntitiesController = gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/);
	dispatcherGlobal = gpUserEntitiesController->getGlobalInteractionsDispatcher().getDispatcher(rpg3D::IWorldOrEntityInteractionFilter::Type::EntityDown, true/*createIfMissing*/);

	entityFilter = new rpg3D::WorldOrEntityInteractionFilter(rpg3D::IWorldOrEntityInteractionFilter::Type::EntityDown);
	entityFilter->setEntityCustomTest(std::bind(&GPC_PlayerEntitySelector::filterTest_entityCustom, this, std::placeholders::_1));

	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_localInputPost->type,
		std::bind(&GPC_PlayerEntitySelector::onGWEvent_gameLoop_localInputPost, this, std::placeholders::_1, std::placeholders::_2),
		localAutoListenersList
	);
}

bool GPC_PlayerEntitySelector::filterTest_entityCustom(IWorldEntity* entity) {
	// Ignore if <player has no entities> or <selected entity> or <entity is not in the player entities list>;
	// Note. Don't fail the test if the selected entity is touched. This will be used when the filter priority_block is implemented. This is required or the tool will activate when self is touched.
	if (playerEntsGroup == nullptr || !playerEntsGroup->hasEntity(entity)) {
		return false;
	}

	return true;
}

void GPC_PlayerEntitySelector::onGWEvent_gameLoop_localInputPost(IEventListener& eventListener, base::GWEvent& event) {
	if (selectedPlayerWEntitySwitchRequest != nullptr) {
		gpContent->setSelectManagedPlayerEntity(selectedPlayerWEntitySwitchRequest, true/*handleBadEntity*/);

		gw->getSimAs<base::GamePartSimExtendable*>()->setCameraFollowTarget(selectedPlayerWEntitySwitchRequest->entity);

		selectedPlayerWEntitySwitchRequest = nullptr;
	}
}

void GPC_PlayerEntitySelector::onUserWorldInteraction(IEventListener& eventListener, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	selectedPlayerWEntitySwitchRequest = interaction->targetWEntity;
}

bool GPC_PlayerEntitySelector::getIsSelectorEnabled() {
	return isSelectorEnabled;
}

void GPC_PlayerEntitySelector::setIsSelectorEnabled(bool isSelectorEnabled) {
	if (this->isSelectorEnabled == isSelectorEnabled) {
		return;
	}

	this->isSelectorEnabled = isSelectorEnabled;

	checkSelectorActivationChange();
}

void GPC_PlayerEntitySelector::checkSelectorActivationChange() {
	if (!this->isSelectorEnabled || playerEntsGroup == nullptr || playerEntsGroup->getEntitiesCount() <= 1) {
		if (localAutoListenersList_filtersDispatcher.count() > 0) {
			if (AppConfig::IS_DEBUG && IS_DEBUG_LOCAL) {
				GlobalAccessLogHandler::devLogHandler->post_debug("Player entities selector deactivated.");
			}

			localAutoListenersList_filtersDispatcher.clear();
		}
	} else {
		if (localAutoListenersList_filtersDispatcher.count() == 0) {
			if (AppConfig::IS_DEBUG && IS_DEBUG_LOCAL) {
				GlobalAccessLogHandler::devLogHandler->post_debug("Player entities selector activated.");
			}

			dispatcherGlobal->addListener(
				std::bind(&GPC_PlayerEntitySelector::onUserWorldInteraction, this, std::placeholders::_1, std::placeholders::_2),
				&localAutoListenersList_filtersDispatcher, nullptr/*cb_onHandlerDisconnected*/,
				static_cast<void*>(static_cast<rpg3D::IWorldOrEntityInteractionFilter*>(entityFilter))/*extraData*/
			);
		}
	}
}

void GPC_PlayerEntitySelector::onPlayerEntityAdded() {
	checkSelectorActivationChange();
}

void GPC_PlayerEntitySelector::onPlayerEntityRemoved() {
	checkSelectorActivationChange();
}

void GPC_PlayerEntitySelector::disposePre() {
	// Make sure to disconnect the filters listeners in the disposePre;
	if (localAutoListenersList_filtersDispatcher.count() > 0) {
		localAutoListenersList_filtersDispatcher.clear();
	}

	if (entityFilter != nullptr) {
		delete entityFilter;
		entityFilter = nullptr;
	}

	super::disposePre();
}

GPC_PlayerEntitySelector::~GPC_PlayerEntitySelector() {
    //void
}
