#include "SEMActions.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>

using namespace rpg3D;

int SEMActions::ActionDoStatusEffect::TYPE = super::NewTypeIndex();

SEMActions::SEMActions(
	IWorldEntity* entity,
	StatusEffectsTemplate* t)
	: super(entity, t)
{
	//void
}

void SEMActions::createBMain() {
	super::createBMain();

	actions = entity->getComponentAs<worldGame3D::IActionsModule*>(true/*mustExist*/);

	actions->setActionProcessor(
		ActionDoStatusEffect::TYPE,
		std::bind(&SEMActions::onAction, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void SEMActions::addAction_ActionDoStatusEffect(std::shared_ptr<CfgStatusEffect> effectConfig) {
	bool isDependantTrackingUsed = false;
	for(int i=0;i<effectConfig->subStatusEffectsList.count();i++) {
		CfgStatusEffect::SubStatusEffect *subEffect = effectConfig->subStatusEffectsList.getPointer(i);

		if(subEffect->isDependantTracked) {
			isDependantTrackingUsed = true;
		}
	}

	int dependantTrackingId = isDependantTrackingUsed ? ++dependantTrackingIdCounter : -1;

	actions->addAction(std::make_shared<ActionDoStatusEffect>(
		true,
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		effectConfig,
		dependantTrackingId, true
	));
	//asd;// Maybe use pre-injecting for this.
	for(int i=0;i<effectConfig->subStatusEffectsList.count();i++) {
		CfgStatusEffect::SubStatusEffect* subEffect = effectConfig->subStatusEffectsList.getPointer(i);

		int finalDependantTrackingId = subEffect->isDependantTracked ? dependantTrackingId : -1;

		actions->addAction(std::make_shared<ActionDoStatusEffect>(
			false,
			entity->getLocalEntityId(), entity->getSyncedEntityId(),
			worldGame3D::EntityAction::Priorities::Normal,
			subEffect->subStatusEffectConfig,
			finalDependantTrackingId, false
		));
	}
}

void SEMActions::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
	if(action.getType() == ActionDoStatusEffect::TYPE) {
		onEntityAction_ActionDoStatusEffect((ActionDoStatusEffect*)action.getRawSubclassPointer(ActionDoStatusEffect::TYPE));
	}else {
		throw LogicException(LOC);
	}
}

void SEMActions::onEntityAction_ActionDoStatusEffect(ActionDoStatusEffect* action) {
	if (action->getIsReplaced() || action->getIsFinished() || !action->getIsPreRequirementsMet()) {
		// This is the last time this action is called.
		// Do action finish,cleanup logic (eg. remove active status effects from the list).

		//void
	}
	if (action->getIsReplaced() || action->getIsOverridden() || !action->getIsPreRequirementsMet()) {
		// Action replaced. This is the last time this action is called.
		// Or action is overridden. Should not do any active logic now excet for the above one in certain cases (eg. to remove active status effects from the list).

		return;
	}

	addStatusEffect(
		action->effectConfig,
		action->dependantTrackingId, action->isDependantTrackingParent
	);
}

SEMActions::~SEMActions() {
	//void
}
