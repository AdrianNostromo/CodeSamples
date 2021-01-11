#include "CustomCubesCannonTurretModule.h"
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <graphics/visual3d/drawable/IDrawableNode3D.h>
#include <graphics/visual3d/drawable/IContainer3D.h>
#include <base/math/Math.h>
#include <sh/scenarios/towerDefense_a_v1/gw/entity/template/customCubesCannonTurret/CustomCubesCannonTurretTemplate.h>
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <rpg3D/gw/entity/module/projectile/ProjectileModule.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <base/audio/sound/ISound.h>
#include <rpg3D/gw/entity/module/flying/IFlyingModule.h>
#include <rpg3D/gw/entity/module/targetedProjectile/ITargetedProjectileModule.h>
#include <rpg3D/gw/entity/module/payloadReceiverTarget/PayloadReceiverTargetModule.h>

using namespace towerDefense_a_v1;

int CustomCubesCannonTurretModule::ActionGroup_AutoAttackTarget = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();

int CustomCubesCannonTurretModule::ActionSetAimTarget::TYPE = super::NewTypeIndex();
int CustomCubesCannonTurretModule::ActionFireProjectileAtTarget::TYPE = super::NewTypeIndex();

CustomCubesCannonTurretModule::CustomCubesCannonTurretModule(
    IWorldEntity* entity,
    CustomCubesCannonTurretTemplate* t,
	base::audio::ISound* activationSound)
    : super(entity, t, activationSound)
{
	//void
}

void CustomCubesCannonTurretModule::createBMain() {
	super::createBMain();

	actions->setActionProcessor(
		ActionSetAimTarget::TYPE,
		std::bind(&CustomCubesCannonTurretModule::onAction, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
	actions->setActionProcessor(
		ActionFireProjectileAtTarget::TYPE,
		std::bind(&CustomCubesCannonTurretModule::onAction, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

bool CustomCubesCannonTurretModule::isPositionInAimAnglesRange(Vector3& pos, float maxYawOffset, float maxPitchOffset) {
	float parentAng = turret->getParent()->getRotation()->getAngleAround(Vector3::Z);
	float ang = turret->getRotation()->getAngleAround(Vector3::Z);
	float composedTurretAng = Math::normalizeAngleDeg(parentAng + ang);
	float targetComposedAng = Math::atan2Deg(pos.y - e->getY(), pos.x - e->getX());

	ang = barrel->getRotation()->getAngleAround(Vector3::Y);
	float composedBarrelAng = Math::normalizeAngleDeg(ang);
	// Compute pitch.
	// NOTE : add a offset of 180 degrees because the model hast the barrel facing up and need to rotate it to face down because down is 0 degrees.
	float targetComposedBarrelAng = 180.0f - 90.0f;

	float turretYawDeltaFromRequestedAngDeg = Math::abs(Math::closestAngleDeg(composedTurretAng, targetComposedAng));
	float barrelPitchDeltaFromRequestedAngDeg = Math::abs(Math::closestAngleDeg(composedBarrelAng, targetComposedBarrelAng));
	if (turretYawDeltaFromRequestedAngDeg <= maxYawOffset && barrelPitchDeltaFromRequestedAngDeg <= maxPitchOffset) {
		return true;
	}

	return false;
}

void CustomCubesCannonTurretModule::addAction_ActionSetAimTarget(IWorldEntity* aimTargetEnity) {
	actions->addAction(std::make_shared<ActionSetAimTarget>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		aimTargetEnity != nullptr ? aimTargetEnity->getWrapper() : nullptr
	));
}

void CustomCubesCannonTurretModule::addAction_ActionFireProjectileAtTarget(IWorldEntity* attackTargetEnity) {
	if (attackTargetEnity == nullptr) {
		throw LogicException(LOC);
	}

	actions->addAction(std::make_shared<ActionFireProjectileAtTarget>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		attackTargetEnity->getWrapper()
	));
}

void CustomCubesCannonTurretModule::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
	if (action.getType() == ActionSetAimTarget::TYPE) {
		onEntityAction_ActionSetAimTarget((ActionSetAimTarget*)action.getRawSubclassPointer(ActionSetAimTarget::TYPE));
	}else if (action.getType() == ActionFireProjectileAtTarget::TYPE) {
		onEntityAction_ActionFireProjectileAtTarget((ActionFireProjectileAtTarget*)action.getRawSubclassPointer(ActionFireProjectileAtTarget::TYPE));
	}else {
		throw LogicException(LOC);
	}
}

void CustomCubesCannonTurretModule::onEntityAction_ActionSetAimTarget(ActionSetAimTarget* action) {
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

	if (action->aimTargetEnity != nullptr && action->aimTargetEnity->entity == nullptr) {
		// A target existend and was lost, stop.
		action->persistentActionFinished();

		return;
	}
	// This also occurs for a nullptr target. In this case there will be a tilt up to the horizon line.

	aimTick_turret(action->aimTargetEnity != nullptr ? action->aimTargetEnity->entity : nullptr, action, action->deltaS);
	bool isFinished = aimTick_barrel(action->aimTargetEnity != nullptr ? action->aimTargetEnity->entity : nullptr, action, action->deltaS);
	if (action->aimTargetEnity == nullptr && isFinished) {
		// Rotating the barrel up to the horizon finished. This action finished.
		action->persistentActionFinished();

		return;
	}
}

bool CustomCubesCannonTurretModule::aimTick_turret(IWorldEntity* aimTargetEnity, ActionSetAimTarget* action, float deltaS) {
	if (aimTargetEnity == nullptr) {
		return true;
	}

	bool isFinished = false;

	float parentAng = turret->getParent()->getRotation()->getAngleAround(Vector3::Z);
	float ang = turret->getRotation()->getAngleAround(Vector3::Z);
	float composedTurretAng = Math::normalizeAngleDeg(parentAng + ang);

	float targetComposedAng = Math::atan2Deg(aimTargetEnity->getY() - e->getY(), aimTargetEnity->getX() - e->getX());

	float rotResult = Math::animateRotationSmallestAngle(
		composedTurretAng,
		targetComposedAng,
		t->turretTurnSpeedZD * deltaS,
		&isFinished
	);

	// Remove the entity offset angle.
	float newRot = Math::normalizeAngleDeg(rotResult - parentAng);

	Quaternion rot(Vector3::Z, newRot);
	turret->setRotation(rot);

	return isFinished;
}

bool CustomCubesCannonTurretModule::aimTick_barrel(IWorldEntity* aimTargetEnity, ActionSetAimTarget* action, float deltaS) {
	bool isFinished = false;

	float ang = barrel->getRotation()->getAngleAround(Vector3::Y);
	float composedTurretAng = Math::normalizeAngleDeg(ang);

	// Compute pitch.
	float targetComposedAng;
	if (aimTargetEnity != nullptr) {
		Matrix4& previewProjectileMat = projectile->getGlobalTransform(true);
		Vector3 previewProjectileGlobalPos{};
		previewProjectileMat.getTranslation(previewProjectileGlobalPos);

		if (action->aimTargetEnity_projectileTarget == nullptr) {
			action->aimTargetEnity_projectileTarget = aimTargetEnity->getComponentAs< rpg3D::IPayloadReceiverTargetModule*>(true/*mustExist*/);
		}

		Vector3 projectileDir{};
		projectileDir.set(
			aimTargetEnity->getX() - previewProjectileGlobalPos.x,
			aimTargetEnity->getY() - previewProjectileGlobalPos.y,
			aimTargetEnity->getZ() - previewProjectileGlobalPos.z
		);
		projectileDir.nor();

		// This asin works because it uses a component from a normalised direction vector.
		float pitchDeg = Math::asinToDeg(projectileDir.z);

		// asin will return angle down from the horizon.
		targetComposedAng = -90.0f - pitchDeg;
	} else {
		targetComposedAng = -90.0f;
	}
	// NOTE : add a offset of 180 degrees because the model has the barrel facing up and need to rotate it to face down because down is 0 degrees.
	targetComposedAng += 180.0f;

	float rotResult = Math::animateRotationSmallestAngle(
		composedTurretAng,
		targetComposedAng,
		t->barrelTurnSpeedYD * deltaS,
		&isFinished
	);

	// Remove the entity offset angle.
	float newRot = Math::normalizeAngleDeg(rotResult);

	Quaternion rot(Vector3::Y, newRot);
	barrel->setRotation(rot);

	return isFinished;
}

void CustomCubesCannonTurretModule::onEntityAction_ActionFireProjectileAtTarget(ActionFireProjectileAtTarget* action) {
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

	if (action->attackTargetEnity->entity == nullptr) {
		e->dispatchEvent(*EVENT_ProjectileLaunchCancelled_TargetLost);

		return;
	}

	// Launch a projectile in the player direction.
	Matrix4& previewProjectileMat = projectile->getGlobalTransform(true);
	Vector3 previewProjectileGlobalPos{};
	previewProjectileMat.getTranslation(previewProjectileGlobalPos);

	rpg3D::IPayloadReceiverTargetModule* attackTargetEnity_projectileTarget = action->attackTargetEnity->entity->getComponentAs< rpg3D::IPayloadReceiverTargetModule*>(true/*mustExist*/);

	Vector3 projectileDir{};
	projectileDir.set(
		action->attackTargetEnity->entity->getX() - previewProjectileGlobalPos.x,
		action->attackTargetEnity->entity->getY() - previewProjectileGlobalPos.y,
		action->attackTargetEnity->entity->getZ() - previewProjectileGlobalPos.z
	);
	projectileDir.nor();

	// Set dirRot from dir. dir contains yaw and pitch.
	float yawDeg = Math::atan2Deg(projectileDir.y, projectileDir.x);
	// This asin works because it uses a component from a normalised direction vector.
	float pitchDeg = Math::asinToDeg(projectileDir.z);

	Quaternion qPitch{ Vector3::Y, -pitchDeg };
	Quaternion dirRot{ Vector3::Z, yawDeg };
	dirRot.mul(qPitch);

	IWorldEntity* projectileEntity = e->getHandler()->createEntity(
		EntityTemplate::fromId(projectileTemplate_namespace, projectileTemplate_id),
		previewProjectileGlobalPos, &dirRot/*rot*/, nullptr/*cardinalRotation*/,
		nullptr, //instanceId
		0, //ownerPlayerId
		nullptr, //extraData
		true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		nullptr
	);

	rpg3D::ITargetedProjectileModule* targetedProjectile = projectileEntity->getComponentAs< rpg3D::ITargetedProjectileModule*>(true/*mustExist*/);
	targetedProjectile->addPayloadsList(
		&projectilePayloadsList
	);

	targetedProjectile->initTargetEntity(
		action->attackTargetEnity->entity
	);

	Vector3 targetPosOrGlobalOffset{};
	projectileEntity->getComponentAs<rpg3D::IFlyingModule*>(true/*mustExist*/)->addAction_ActionDestinationFlight(
		action->attackTargetEnity->entity, targetPosOrGlobalOffset,
		true/*flyToLastKnownTargetPosOnTargetLoss*/, false/*stopFlyingAtDestination*/
	);
	
	if (activationSound != nullptr) {
		activationSound->play(t->activationSoundConfig_volume);
	}

	setIsProjectileLoaded(false);

	e->dispatchEvent(*EVENT_ProjectileLaunched);
}

CustomCubesCannonTurretModule::~CustomCubesCannonTurretModule() {
    //void
}
