#include "CCCTM_Reload.h"
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <graphics/visual3d/drawable/IDrawableNode3D.h>
#include <graphics/visual3d/drawable/IDrawableNode3D.h>
#include <base/math/Math.h>
#include <sh/scenarios/towerDefense_a_v1/gw/entity/template/customCubesCannonTurret/CustomCubesCannonTurretTemplate.h>

using namespace towerDefense_a_v1;

int CCCTM_Reload::ActionGroup_Reload = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();
const int CCCTM_Reload::ActionReload::Phases::INITIAL_ROTATION = 1;
const int CCCTM_Reload::ActionReload::Phases::LOWERING = 2;
const int CCCTM_Reload::ActionReload::Phases::RISING = 3;
const int CCCTM_Reload::ActionReload::Phases::FINISHED = 4;

int CCCTM_Reload::ActionReload::TYPE = super::NewTypeIndex();

CCCTM_Reload::CCCTM_Reload(
	IWorldEntity* entity,
	CustomCubesCannonTurretTemplate* t,
	base::audio::ISound* activationSound)
	: super(entity, t, activationSound)
{
	//void
}

void CCCTM_Reload::createBMain() {
	super::createBMain();

	actions->setActionProcessor(
		ActionReload::TYPE,
		std::bind(&CCCTM_Reload::onAction, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	setIsProjectilePreviewVisible(isProjectileLoaded);
}

bool CCCTM_Reload::getIsProjectileLoaded() {
	return isProjectileLoaded;
}

void CCCTM_Reload::setIsProjectileLoaded(bool isProjectileLoaded) {
	if (this->isProjectileLoaded == isProjectileLoaded) {
		return;
	}
	this->isProjectileLoaded = isProjectileLoaded;

	setIsProjectilePreviewVisible(this->isProjectileLoaded);
}

void CCCTM_Reload::setIsProjectilePreviewVisible(bool isProjectilePreviewVisible) {
	if (this->isProjectilePreviewVisible == isProjectilePreviewVisible) {
		return;
	}
	this->isProjectilePreviewVisible = isProjectilePreviewVisible;

	projectile->setIsVisibleIncremental(this->isProjectilePreviewVisible);
}

void CCCTM_Reload::addAction_ActionReload() {
	actions->addAction(std::make_shared<ActionReload>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal
	));
}

void CCCTM_Reload::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
	if (action.getType() == ActionReload::TYPE) {
		onEntityAction_ActionReload((ActionReload*)action.getRawSubclassPointer(ActionReload::TYPE));
	} else {
		throw LogicException(LOC);
	}
}

void CCCTM_Reload::onEntityAction_ActionReload(ActionReload* action) {
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

	bool isReturnRequested = false;
	int newPhase = action->phase;
	while(true) {
		switch (action->phase) {
		case ActionReload::Phases::INITIAL_ROTATION: {
			if (!action->isPhaseInit) {
				//void
			} else {
				//void
			}

			float turretAng = turret->getRotation()->getAngleAround(Vector3::Z);

			float deltaZ = Math::closestAngleDeg(
				turretAng,
				t->turretReloadAnglesZ->getDirect(0)
			);
			float targetTurretAng = t->turretReloadAnglesZ->getDirect(0);

			for (int i = 1; i < t->turretReloadAnglesZ->getCount(); i++) {
				float f = Math::closestAngleDeg(
					turretAng,
					t->turretReloadAnglesZ->getDirect(i)
				);
				if (Math::abs(f) < Math::abs(deltaZ)) {
					deltaZ = f;
					targetTurretAng = t->turretReloadAnglesZ->getDirect(i);
				}
			}

			bool turretRotResult_isFinished = false;
			float turretRotResult = Math::animateRotationSmallestAngle(
				turretAng,
				targetTurretAng,
				t->turretTurnSpeedZD * action->deltaS,
				&turretRotResult_isFinished
			);
			Quaternion rot(Vector3::Z, turretRotResult);
			turret->setRotation(rot);

			bool barrelRotResult_isFinished = false;
			float barrelRotResult = Math::animateRotationSmallestAngle(
				barrel->getRotation()->getAngleAround(Vector3::Y),
				t->barrelRotY_loading,
				t->barrelTurnSpeedYD * action->deltaS,
				&barrelRotResult_isFinished
			);
			Quaternion rot2(Vector3::Y, barrelRotResult);
			barrel->setRotation(rot2);

			if (turretRotResult_isFinished && barrelRotResult_isFinished) {
				newPhase = ActionReload::Phases::LOWERING;

				break;
			}

			break;
		}
		case ActionReload::Phases::LOWERING: {
			if (!action->isPhaseInit) {
				action->phaseCompletionPercent = 0.0f;
			} else {
				action->phaseCompletionPercent = Math::min(1.0f, action->phaseCompletionPercent + action->deltaS * 1.4f);
			}

			barrel->setZ(t->barrelPosZ_normal + (t->barrelPosZ_loading - t->barrelPosZ_normal) * action->phaseCompletionPercent);

			if (action->phaseCompletionPercent >= 1.0f) {
				// Make the preview projectile visible before the reload finishes.
				setIsProjectilePreviewVisible(true);

				newPhase = ActionReload::Phases::RISING;

				break;
			}
			//void

			break;
		}
		case ActionReload::Phases::RISING: {
			if (!action->isPhaseInit) {
				action->phaseCompletionPercent = 0.0f;
			} else {
				action->phaseCompletionPercent = Math::min(1.0f, action->phaseCompletionPercent + action->deltaS * 1.4f);
			}

			barrel->setZ(t->barrelPosZ_loading + (t->barrelPosZ_normal - t->barrelPosZ_loading) * action->phaseCompletionPercent);

			if (action->phaseCompletionPercent >= 1.0f) {
				newPhase = ActionReload::Phases::FINISHED;

				break;
			}

			break;
		}
		case ActionReload::Phases::FINISHED: {
			if (!action->isPhaseInit) {
				setIsProjectileLoaded(true);

				action->persistentActionFinished();

				e->dispatchEvent(*EVENT_ReloadFinished);

				isReturnRequested = true;
				
				break;
			} else {
				//void
			}

			//void

			break;
		}
		default: {
			throw LogicException(LOC);
		}}

		if (!action->isPhaseInit) {
			action->isPhaseInit = true;
		}

		if (newPhase != action->phase) {
			action->phase = newPhase;
			action->isPhaseInit = false;
		} else {
			break;
		}

		if (isReturnRequested) {
			break;
		}
	}
}

CCCTM_Reload::~CCCTM_Reload() {
	//void
}
