#include "ToolRangeInertialProjectile.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/template/userControlled/UserControlledTemplate.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <base/math/Math.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <worldGame3D/gw/entity/WorldEntity.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <base/visual3d/Visual3DUtil.h>
#include <rpg3D/gw/entity/module/projectile/ProjectileModule.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/ILogicBlock.h>
#include <rpg3D/gw/entity/module/flying/IFlyingModule.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/config/team/TeamsIndexesList.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/entity/module/userControlled/filters/IWorldInteractionFilter.h>
#include <rpg3D/gw/entity/module/userControlled/filters/custom/WIF_Touch_FeetPlaneOrHalfSphereDome.h>
#include <rpg3D/gw/entity/module/userControlled/filters/custom/WIF_Touch.h>
#include <worldGame3D/gw/util/GameCameraModeBitGroups.h>
#include <rpg3D/gw/projectile/ProjectileConfig.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <base/math/Random.h>
#include <base/memory/SharedPointer.h>

using namespace rpg3D;

ToolRangeInertialProjectile::ToolRangeInertialProjectile(ToolConfigRangeInertialProjectile* config, std::shared_ptr<ExtraData> extraData)
    : super(config, extraData)
{
	// Note. This will not link without "(int)". May be something with the variable params system.
	customFiltersList_innactiveForActivation.addFilter(std::make_shared<rpg3D::WIF_Touch_FeetPlaneOrHalfSphereDome>(
		base::Touch::CursorMode::Normal->bitGroup | base::Touch::CursorMode::Locked->bitGroup, base::Touch::ButtonCode::LeftOrTouch->bitGroup/*buttonId*/, TouchEventType::CURSOR_DOWN->bitGroup/*touchType*/,
		IWorldInteractionFilter::InputPriorities::Normal/*inputPriority*/,
		GameCameraModeBitGroups::followerFirstPersonTargetRotation | GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation/*cameraModesBitMaskAny*/,
		std::bind(&ToolRangeInertialProjectile::onFilterInteraction_innactiveForActivation_Feet_Plane_Down_BtnMain, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
		std::bind(&ToolRangeInertialProjectile::onFilterInteraction_innactiveForActivation_Feet_HalfSphereDome_Down_BtnMain, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
	));

	customFiltersList_activeContinuousForUpdate.addFilter(std::make_shared<rpg3D::WIF_Touch_FeetPlaneOrHalfSphereDome>(
		base::Touch::CursorMode::Normal->bitGroup | base::Touch::CursorMode::Locked->bitGroup, base::Touch::ButtonCode::LeftOrTouch->bitGroup/*buttonId*/, TouchEventType::CURSOR_DRAG->bitGroup/*touchType*/,
		IWorldInteractionFilter::InputPriorities::Normal/*inputPriority*/,
		GameCameraModeBitGroups::followerFirstPersonTargetRotation | GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation/*cameraModesBitMaskAny*/,
		std::bind(&ToolRangeInertialProjectile::onFilterInteraction_activeContinuousForUpdate_Feet_Plane_Down_BtnMain, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
		std::bind(&ToolRangeInertialProjectile::onFilterInteraction_activeContinuousForUpdate_Feet_HalfSphereDome_Down_BtnMain, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
	));
	
	customFiltersList_activeContinuousForDeactivation.addFilter(std::make_shared<rpg3D::WIF_Touch>(
		base::Touch::CursorMode::Normal->bitGroup | base::Touch::CursorMode::Locked->bitGroup, base::Touch::ButtonCode::LeftOrTouch->bitGroup/*buttonId*/, TouchEventType::CURSOR_UP->bitGroup/*touchType*/,
		IWorldInteractionFilter::InputPriorities::Normal/*inputPriority*/,
		std::bind(&ToolRangeInertialProjectile::onFilterInteraction_activeContinuousForDeactivation_Touch_Up, this, std::placeholders::_1, std::placeholders::_2)
	));
}

bool ToolRangeInertialProjectile::onFilterInteraction_innactiveForActivation_Feet_Plane_Down_BtnMain(Vector3& localFeetPlanePos, int cursorIndex, int cameraModeBitGroup) {
	if (cameraModeBitGroup == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
		addAction_ActionOperate_Activate(cursorIndex, std::make_shared<Vector3>(localFeetPlanePos)/*worldPos*/, nullptr/*worldDir*/, nullptr/*worldPosXY*/, false/*isEnabledPreTurning*/);
	} else if (cameraModeBitGroup == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
		// This can be use to make the unit fire projectiles straight ahead.
		//Vector3* entityPos = entity->getPos();
		//Vector3 dir{
		//	localFeetPlanePos.x - entityPos->x,
		//	localFeetPlanePos.y - entityPos->y,
		//	0.0f
		//};
		//dir.nor();
		//
		//addAction_ActionOperate_Activate(cursorIndex, nullptr/*worldPos*/, std::make_shared<Vector3>(dir)/*worldDir*/, msp<Vector2>(localFeetPlanePos.x, localFeetPlanePos.y), true/*isEnabledPreTurning*/);
		
		// This version is used to fire at the cursor location. The player can fire sideways but this is currently the desired behaviour. The side firing is mitigated with a minimum distance.
		addAction_ActionOperate_Activate(cursorIndex, nullptr/*worldPos*/, nullptr/*worldDir*/, msp<Vector2>(localFeetPlanePos.x, localFeetPlanePos.y), true/*isEnabledPreTurning*/);
	} else {
		throw LogicException(LOC);
	}
	
	return true;
}

bool ToolRangeInertialProjectile::onFilterInteraction_innactiveForActivation_Feet_HalfSphereDome_Down_BtnMain(Vector3& halfSphereDomeWorldDir, int cursorIndex, int cameraModeBitGroup) {
	if (cameraModeBitGroup == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
		addAction_ActionOperate_Activate(cursorIndex, nullptr/*worldPos*/, std::make_shared<Vector3>(halfSphereDomeWorldDir)/*worldDir*/, nullptr/*worldPosXY*/, false/*isEnabledPreTurning*/);
	} else if (cameraModeBitGroup == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
		addAction_ActionOperate_Activate(cursorIndex, nullptr/*worldPos*/, std::make_shared<Vector3>(
			halfSphereDomeWorldDir.x,
			halfSphereDomeWorldDir.y,
			0.0f
		)/*worldDir*/, nullptr/*worldPosXY*/, true/*isEnabledPreTurning*/);
	} else {
		throw LogicException(LOC);
	}

	return true;
}

bool ToolRangeInertialProjectile::onFilterInteraction_activeContinuousForUpdate_Feet_Plane_Down_BtnMain(Vector3& localFeetPlanePos, int cursorIndex, int cameraModeBitGroup) {
	if (cameraModeBitGroup == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
		addAction_ActionOperate_Update(cursorIndex, std::make_shared<Vector3>(localFeetPlanePos), nullptr, nullptr/*worldPosXY*/, false/*isEnabledPreTurning*/);
	} else if (cameraModeBitGroup == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
		// This can be use to make the unit fire projectiles straight ahead.
		//Vector3* entityPos = entity->getPos();
		//Vector3 dir{
		//	localFeetPlanePos.x - entityPos->x,
		//	localFeetPlanePos.y - entityPos->y,
		//	0.0f
		//};
		//dir.nor();
		//
		//addAction_ActionOperate_Update(cursorIndex, nullptr, std::make_shared<Vector3>(dir), nullptr/*worldPosXY*/, true/*isEnabledPreTurning*/);

		// This version is used to fire at the cursor location. The player can fire sideways but this is currently the desired behaviour. The side firing is mitigated with a minimum distance.
		addAction_ActionOperate_Update(cursorIndex, nullptr, nullptr/*worldDir*/, msp<Vector2>(localFeetPlanePos.x, localFeetPlanePos.y)/*worldPosXY*/, true/*isEnabledPreTurning*/);
	} else {
		throw LogicException(LOC);
	}

	return true;
}

bool ToolRangeInertialProjectile::onFilterInteraction_activeContinuousForUpdate_Feet_HalfSphereDome_Down_BtnMain(Vector3& halfSphereDomeWorldDir, int cursorIndex, int cameraModeBitGroup) {
	if (cameraModeBitGroup == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
		addAction_ActionOperate_Update(cursorIndex, nullptr, std::make_shared<Vector3>(halfSphereDomeWorldDir), nullptr/*worldPosXY*/, false/*isEnabledPreTurning*/);
	} else if (cameraModeBitGroup == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
		addAction_ActionOperate_Update(cursorIndex, nullptr, std::make_shared<Vector3>(
			halfSphereDomeWorldDir.x,
			halfSphereDomeWorldDir.y,
			0.0f
			), nullptr/*worldPosXY*/, true/*isEnabledPreTurning*/
		);
	} else {
		throw LogicException(LOC);
	}

	return true;
}

bool ToolRangeInertialProjectile::onFilterInteraction_activeContinuousForDeactivation_Touch_Up(int cursorIndex, Vector2& screenPos) {
	addAction_ActionStopOperation(cursorIndex);

	return true;
}

void ToolRangeInertialProjectile::onEntityChanged() {
	super::onEntityChanged();

	for (int projectileSpawnIndex = 0; projectileSpawnIndex < getLocalConfig()->projectileSpawnConfigsList.count(); projectileSpawnIndex++) {
		ToolConfigRangeInertialProjectile::ProjectileSpawnConfig& projectileSpawnConfig = getLocalConfig()->projectileSpawnConfigsList.getReference(projectileSpawnIndex);

		IContainer3D* projectileSpawnParent = getToolDrawable<IContainer3D*>(
			projectileSpawnConfig.projectileSpawnParentId,
			true/*mustExist*/
		);

		projectileSpawnParentsList.appendDirect(projectileSpawnParent);
	}
}

bool ToolRangeInertialProjectile::tickTriggerInProgress(float triggerDurationS, int tickIndex) {
	// When the cooldown is charged (FOR THE FIRST TIME) is the point of no return.
	if (!getIsTriggerPointOfNoReturnReached()) {
		// Call this before the projectile is created just in case the projectile decides to sudoku on create and dispatch the extended_activation_finish_event).
		// Dispatch this here because not all tools have cooldown or trigger mechanism.
		entity->dispatchEvent(*EVENT_tool_triggerPointOfNoReturn_start->setVoidData(static_cast<ITool*>(this)));
	}
	chargeCooldown();

	activeProjectilesCount = 0;
	for (int projectileSpawnIndex = 0; projectileSpawnIndex < getLocalConfig()->projectileSpawnConfigsList.count(); projectileSpawnIndex++) {
		ToolConfigRangeInertialProjectile::ProjectileSpawnConfig& projectileSpawnConfig = getLocalConfig()->projectileSpawnConfigsList.getReference(projectileSpawnIndex);

		IContainer3D* projectileSpawnParent = projectileSpawnParentsList.getDirect(projectileSpawnIndex);

		float finalAngZDegLocal = Math::normalizeAngleDeg(projectileSpawnConfig.spawnAngZDeg + Math::Rand::RNG->nextFloat(-projectileSpawnConfig.maxSpawnAngZDegError, projectileSpawnConfig.maxSpawnAngZDegError));

		activeProjectilesCount++;
		spawnProjectile(projectileSpawnParent, finalAngZDegLocal);
	}

	return false;
}

void ToolRangeInertialProjectile::spawnProjectile(IContainer3D* projectileSpawnParent, float finalAngZDegLocal) {
	// Launch a projectile in the player direction.
	Matrix4& projectileSpawnMat = projectileSpawnParent->getGlobalTransform(true);
	Vector3 projectileSpawnPosGlobal{};
	projectileSpawnMat.getTranslation(projectileSpawnPosGlobal);

	Vector3 projectileDir{};
	if (operating_worldPos != nullptr) {
		// Convert to a direction and use the dir logic below to compute hte projectile direction rotation.
		projectileDir.set(
			operating_worldPos->x - projectileSpawnPosGlobal.x,
			operating_worldPos->y - projectileSpawnPosGlobal.y,
			operating_worldPos->z - projectileSpawnPosGlobal.z
		);
	}else if (operating_worldDir != nullptr) {
		projectileDir.set(*operating_worldDir);
	}else if (operating_worldPosXY != nullptr) {
		projectileDir.set(
			operating_worldPosXY->x - projectileSpawnPosGlobal.x,
			operating_worldPosXY->y - projectileSpawnPosGlobal.y,
			0.0f
		);
	}else {
		throw LogicException(LOC);
	}
	projectileDir.nor();

	// Set dirRot from dir. dir contains yaw and pitch.
	float yawDeg = Math::atan2Deg(projectileDir.y, projectileDir.x);
	float pitchDeg = Math::asinToDeg(projectileDir.z);

	yawDeg += finalAngZDegLocal;

	Quaternion qPitch{ Vector3::Y, -pitchDeg };
	Quaternion dirRot{ Vector3::Z, yawDeg };
	dirRot.mul(qPitch);

	IWorldEntity* projectileEntity = entity->getHandler()->createEntity(
		EntityTemplate::fromId(peekProjectileConfig()->projectileTemplateNSpace, peekProjectileConfig()->projectileTemplateId),
		projectileSpawnPosGlobal, &dirRot/*rot*/, nullptr/*cardinalRotation*/,
		nullptr, //instanceId
		0, //ownerPlayerId
		nullptr/*extraData*/,
		true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, entity/*creatorEntity*/,
		[this](IWorldEntity* newEntity) -> void {
			newEntity->getComponentAs<IProjectileModule*>(true/*mustExist*/)->addPayloadsList(
				&peekProjectileConfig()->effectPayloadsList
			);

			newEntity->getComponentAs<ITeamModule*>(true/*mustExist*/)->setTeamIndex(
				entity->getComponentAs<ITeamModule*>(true/*mustExist*/)->getTeamIndex()
			);
		}
	);

	Vector3 flightDir(1.0f, 0.0f, 0.0f);
	flightDir.mul(dirRot);
	flightDir.nor();
	projectileEntity->getComponentAs<rpg3D::IFlyingModule*>(true/*mustExist*/)
		->addAction_ActionApplyImpulse(
			flightDir, 
			config->projectileLaunchPhysicsImpulse
		);

	if (activationSound != nullptr) {
		activationSound->play(config->activationSoundConfig_volume);
	}

	projectileEntity->addEventListener(
		rpg3D::IProjectileModule::EVENT_projectile_purposeFinished->type, IWorldEntity::EVENT_onRemoved_pre->type,
		std::bind(&ToolRangeInertialProjectile::onProjectileEvent, this, std::placeholders::_1, std::placeholders::_2),
		projectilesAutoListenersList
	);
}

void ToolRangeInertialProjectile::onProjectileEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::IProjectileModule::EVENT_projectile_purposeFinished->type) {
		if (activeProjectilesCount <= 0) {
			// All projectiles finished already.
			throw LogicException(LOC);
		}

		activeProjectilesCount--;

		eventListener.disconnect();

		if (activeProjectilesCount == 0) {
			// The entries are already disconnected by the above disconnect();
			projectilesAutoListenersList.clear();

			if (toolsHandler->getIsTurnBasedToolsModeActive()) {
				zeroCooldownIfExists();
			}

			entity->dispatchEvent(*EVENT_tool_extendedActivationFinished->setVoidData(static_cast<ITool*>(this)));
		}
	}else if (event.type == IWorldEntity::EVENT_onRemoved_pre->type) {
		projectilesAutoListenersList.clear();

		if (toolsHandler->getIsTurnBasedToolsModeActive()) {
			zeroCooldownIfExists();
		}

		entity->dispatchEvent(*EVENT_tool_extendedActivationFinished->setVoidData(static_cast<ITool*>(this)));
	}

	else {
		throw LogicException(LOC);
	}
}

void ToolRangeInertialProjectile::onActiveTriggeringCancelPre() {
	super::onActiveTriggeringCancelPre();

	projectilesAutoListenersList.clear();

	if (toolsHandler->getIsTurnBasedToolsModeActive()) {
		zeroCooldownIfExists();
	}

	if (getIsTriggerPointOfNoReturnReached()) {
		entity->dispatchEvent(*EVENT_tool_extendedActivationFinished->setVoidData(static_cast<ITool*>(this)));
	}
}

ToolRangeInertialProjectile::~ToolRangeInertialProjectile() {
    //void
}
