#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>
#include "ToolMelleArea.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/actions/IActionsModule.h>
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
#include <base/exceptions/NotImplementedException.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/entity/module/userControlled/filters/IWorldInteractionFilter.h>
#include <rpg3D/gw/entity/module/userControlled/filters/custom/WIF_Touch_FeetPlaneOrHalfSphereDome.h>
#include <rpg3D/gw/entity/module/userControlled/filters/custom/WIF_Touch.h>
#include <worldGame3D/gw/util/GameCameraModeBitGroups.h>
#include <rpg3D/gw/projectile/ProjectileConfig.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <rpg3D/gw/entity/module/physics3D/pBodyUpdateHandler/DirectPBodyUpdateHandler.h>
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <rpg3D/gw/entity/template/toolshandler/util/ToolSlotConfig.h>
#include <base/ease/Ease.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/actor/PhysicsActor.h>

using namespace rpg3D;

ToolMelleArea::ToolMelleArea(ToolConfigMelleArea* config, std::shared_ptr<ExtraData> extraData)
    : super(config, extraData)
{
	// Note. This will not link without "(int)". May be something with the variable params system.
	customFiltersList_innactiveForActivation.addFilter(std::make_shared<rpg3D::WIF_Touch_FeetPlaneOrHalfSphereDome>(
		base::Touch::CursorMode::Normal->bitGroup | base::Touch::CursorMode::Locked->bitGroup, base::Touch::ButtonCode::LeftOrTouch->bitGroup/*buttonId*/, TouchEventType::CURSOR_DOWN->bitGroup/*touchType*/,
		IWorldInteractionFilter::InputPriorities::Normal/*inputPriority*/,
		GameCameraModeBitGroups::followerFirstPersonTargetRotation | GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation/*cameraModesBitMaskAny*/,
		std::bind(&ToolMelleArea::onFilterInteraction_innactiveForActivation_Feet_Plane_Down_BtnMain, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
		std::bind(&ToolMelleArea::onFilterInteraction_innactiveForActivation_Feet_HalfSphereDome_Down_BtnMain, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
	));

	customFiltersList_activeContinuousForUpdate.addFilter(std::make_shared<rpg3D::WIF_Touch_FeetPlaneOrHalfSphereDome>(
		base::Touch::CursorMode::Normal->bitGroup | base::Touch::CursorMode::Locked->bitGroup, base::Touch::ButtonCode::LeftOrTouch->bitGroup/*buttonId*/, TouchEventType::CURSOR_DRAG->bitGroup/*touchType*/,
		IWorldInteractionFilter::InputPriorities::Normal/*inputPriority*/,
		GameCameraModeBitGroups::followerFirstPersonTargetRotation | GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation/*cameraModesBitMaskAny*/,
		std::bind(&ToolMelleArea::onFilterInteraction_activeContinuousForUpdate_Feet_Plane_Down_BtnMain, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
		std::bind(&ToolMelleArea::onFilterInteraction_activeContinuousForUpdate_Feet_HalfSphereDome_Down_BtnMain, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
	));

	customFiltersList_activeContinuousForDeactivation.addFilter(std::make_shared<rpg3D::WIF_Touch>(
		base::Touch::CursorMode::Normal->bitGroup | base::Touch::CursorMode::Locked->bitGroup, base::Touch::ButtonCode::LeftOrTouch->bitGroup/*buttonId*/, TouchEventType::CURSOR_UP->bitGroup/*touchType*/,
		IWorldInteractionFilter::InputPriorities::Normal/*inputPriority*/,
		std::bind(&ToolMelleArea::onFilterInteraction_activeContinuousForDeactivation_Touch_Up, this, std::placeholders::_1, std::placeholders::_2)
	));
}

bool ToolMelleArea::onFilterInteraction_innactiveForActivation_Feet_Plane_Down_BtnMain(Vector3& localFeetPlanePos, int cursorIndex, int cameraModeBitGroup) {
	if (cameraModeBitGroup == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
		addAction_ActionOperate_Activate(cursorIndex, std::make_shared<Vector3>(localFeetPlanePos), nullptr, nullptr/*worldPosXY*/, false/*isEnabledPreTurning*/);
	} else if (cameraModeBitGroup == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
		Vector3* entityPos = entity->getPos();
		Vector3 dir{
			localFeetPlanePos.x - entityPos->x,
			localFeetPlanePos.y - entityPos->y,
			0.0f
		};
		dir.nor();
		addAction_ActionOperate_Activate(cursorIndex, nullptr, std::make_shared<Vector3>(dir), nullptr/*worldPosXY*/, true/*isEnabledPreTurning*/);
	} else {
		throw LogicException(LOC);
	}
	
	return true;
}

bool ToolMelleArea::onFilterInteraction_innactiveForActivation_Feet_HalfSphereDome_Down_BtnMain(Vector3& halfSphereDomeWorldDir, int cursorIndex, int cameraModeBitGroup) {
	if (cameraModeBitGroup == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
		addAction_ActionOperate_Activate(cursorIndex, nullptr, std::make_shared<Vector3>(halfSphereDomeWorldDir), nullptr/*worldPosXY*/, false/*isEnabledPreTurning*/);
	} else if (cameraModeBitGroup == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
		addAction_ActionOperate_Activate(cursorIndex, nullptr, std::make_shared<Vector3>(
			halfSphereDomeWorldDir.x,
			halfSphereDomeWorldDir.y,
			0.0f
		), nullptr/*worldPosXY*/, true/*isEnabledPreTurning*/);
	} else {
		throw LogicException(LOC);
	}

	return true;
}

bool ToolMelleArea::onFilterInteraction_activeContinuousForUpdate_Feet_Plane_Down_BtnMain(Vector3& localFeetPlanePos, int cursorIndex, int cameraModeBitGroup) {
	if (cameraModeBitGroup == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
		addAction_ActionOperate_Update(cursorIndex, std::make_shared<Vector3>(localFeetPlanePos), nullptr, nullptr/*worldPosXY*/, false/*isEnabledPreTurning*/);
	} else if (cameraModeBitGroup == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
		Vector3* entityPos = entity->getPos();
		Vector3 dir{
			localFeetPlanePos.x - entityPos->x,
			localFeetPlanePos.y - entityPos->y,
			0.0f
		};
		dir.nor();

		addAction_ActionOperate_Update(cursorIndex, nullptr, std::make_shared<Vector3>(dir), nullptr/*worldPosXY*/, true/*isEnabledPreTurning*/);
	} else {
		throw LogicException(LOC);
	}

	return true;
}

bool ToolMelleArea::onFilterInteraction_activeContinuousForUpdate_Feet_HalfSphereDome_Down_BtnMain(Vector3& halfSphereDomeWorldDir, int cursorIndex, int cameraModeBitGroup) {
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

bool ToolMelleArea::onFilterInteraction_activeContinuousForDeactivation_Touch_Up(int cursorIndex, Vector2& screenPos) {
	addAction_ActionStopOperation(cursorIndex);

	return true;
}

bool ToolMelleArea::tickTriggerInProgress(float triggerDurationS, int tickIndex) {
	if (tickIndex == 0) {
		entity->dispatchEvent(*EVENT_tool_triggerPointOfNoReturn_start->setVoidData(static_cast<ITool*>(this)));

		if (activationSound != nullptr) {
			activationSound->play(config->activationSoundConfig_volume);
		}

		// Need to set the pActor always because it gets removed in many places and also then the pBody is removed from the pWorld.
		IPhysicsActor* p = physicsActor;
		pBody->setUserPointer(p);

		pWorld->addRigidBody(pBody, pCollisionConfig->groups/*group*/, pCollisionConfig->anyInteraction_mask/*mask*/);
	}

	float cp = Math::min(1.0f, triggerDurationS / config->triggerDurationS);

	Matrix4& matHolder = slotVisualHolder->getGlobalTransform(true/*updateTransformStackIfDirty*/);

	Matrix4 matOffset{};
	matOffset.idt();
	Vector3 pos{};
	Ease::interpolate(
		pos,
		cp,
		config->trigger_pBody_pos_start, config->trigger_pBody_pos_end
	);
	matOffset.translate(pos);

	Matrix4 finalMat{};
	finalMat.set(matHolder);
	finalMat.mul(matOffset);

	Vector3 globalPos{};
	finalMat.getTranslation(globalPos);
	btVector3 btGlobalPos{ globalPos.x, globalPos.y, globalPos.z };
	pBodyUpdateHandler->setVisualPos(btGlobalPos);

	Quaternion globalRot{};
	finalMat.getRotation(globalRot);
	btQuaternion btGlobalRot{ globalRot.x, globalRot.y, globalRot.z, globalRot.w };
	pBodyUpdateHandler->setVisualRot(btGlobalRot);

	float newScale = Ease::interpolate(
		cp,
		config->trigger_pBody_scale_start, config->trigger_pBody_scale_end
	);
	btVector3 btScale{ newScale, newScale, newScale };
	pShape->setLocalScaling(btScale);
	if (pBody->getActivationState() != ACTIVE_TAG) {
		// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
		pBody->activate(true);
	}

	// This is to get the new bounding box of the scale to take effect. Static objects don't update their AABB for performance reasons. This is called to avoid removing and re-adding the body to the pWorld.
	if (pBody->isInWorld()) {
		pWorld->updateSingleAabb(pBody);
	}

	// Charge the cooldown each frame until finish.
	chargeCooldown();

	if (cp < 1.0f) {
		// Still animating.
		return true;
	}

	if (pBody->isInWorld()) {
		physicsActor->disconnectAllContacts();

		// Remove the pActor so the disconnect isn't processed because contacts already disconnected and also if the tool is disposed, an error would occur because a physics contact break would be saved with a pActor that is disposed when this tool gets disposed.
		pBody->setUserPointer(nullptr);
		pWorld->removeRigidBody(pBody);
	}

	if (toolsHandler->getIsTurnBasedToolsModeActive()) {
		zeroCooldownIfExists();
	}

	entity->dispatchEvent(*EVENT_tool_extendedActivationFinished->setVoidData(static_cast<ITool*>(this)));

	return false;
}

void ToolMelleArea::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

	// Show or hide the sword visual. Keep the model in memory, just remove it from the display list.
	if (!isSelectedAndGWCapabilitiesExist) {
		slotVisualHolder = nullptr;
	} else {
		worldGame3D::IVisual3DModule* visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
		slotVisualHolder = visual3D->getDrawableOptionalAs<IContainer3D*>(peekSelectedToolSlot()->toolHolderVisualId);
	}
}

void ToolMelleArea::onActiveTriggeringCancelPre() {
	super::onActiveTriggeringCancelPre();

	if (pBody->isInWorld()) {
		physicsActor->disconnectAllContacts();

		// Remove the pActor so the disconnect isn't processed because contacts already disconnected and also if the tool is disposed, an error would occur because a physics contact break would be saved with a pActor that is disposed when this tool gets disposed.
		pBody->setUserPointer(nullptr);
		pWorld->removeRigidBody(pBody);
	}

	if (toolsHandler->getIsTurnBasedToolsModeActive()) {
		zeroCooldownIfExists();
	}

	entity->dispatchEvent(*EVENT_tool_extendedActivationCancelled->setVoidData(static_cast<ITool*>(this)));
}

ToolMelleArea::~ToolMelleArea() {
    //void
}
