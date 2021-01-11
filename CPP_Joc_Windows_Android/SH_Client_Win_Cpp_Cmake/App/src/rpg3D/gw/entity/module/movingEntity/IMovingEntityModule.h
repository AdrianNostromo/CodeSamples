#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>
#include <base/math/Vector1.h>
#include <base/math/Vector2.h>
#include <base/util/StringUtil.h>

namespace base {
	class WrappedWorldEntity;
};
namespace worldGame3D {
	class EntityAction;
};

class IMovingEntityModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub class ControlType {
		pub static const int Global_Dir;
		pub static const int LocalEntityDirection_Strafe;
		// This is the same as strafe but the entity can move only back or front in the direction it is facing.
		//asdA2;// this is currently not implemented, it just global_strafe-s to the destination and instant turns. Implement when this type of movement is really needed (eg: planes, rockets, innertial stuff);
		//asdA2;// The min_move_speed_s and slow down when turning is handled by the logic block, not this.
		//asdA2;// Maybe remove this and make the logickBlock with some custom code and LocalEntityDirection_Strafe.
		pub static const int LocalEntityDirection_BackForwardOnly;
	};

	pub static int ActionGroup_Move;
	pub static int ActionGroup_Turn;

	pub static int ActionGroup_DirectionalTurnZ;

	pub static base::EntityEvent* EVENT_move_Start;
	pub static base::EntityEvent* EVENT_move_Stop;
	pub static base::EntityEvent* EVENT_move_destinationReached;

	pub static base::EntityEvent* EVENT_teleport_Start;

	pub static base::EntityEvent* EVENT_turn_Start;
	pub static base::EntityEvent* EVENT_turn_Stop;

	pub virtual void addAction_ActionDirectRotateZElevationDelta(float rotZDegDelta, float rotElevationDelta) = 0;
	pub virtual void addAction_ActionDirectRotateZ(int priority, float rotZDeg) = 0;
	pub virtual void addAction_ActionDirectRotateZElevation(float rotZDeg, float rotElevationDeg) = 0;
	pub virtual void addAction_ActionDirectRotateElevation(float rotElevationDeg) = 0;

	pub virtual std::shared_ptr<worldGame3D::EntityAction> buildAction_ActionToTargetMovementContinuous(
		int controlType,
		Vector2& targetPosOrOffset, std::shared_ptr<base::WrappedWorldEntity> targetEntity,
		float maxRange, bool isStopAtDestination,
		bool isAutoTurn, bool isInstantTurn,
		std::string& initiatorIdentifier = StringUtil::empty,
		int movementTypeOverride = -1) = 0;
	pub virtual void addAction_ActionToTargetMovementContinuous(
		int controlType,
		Vector2& targetPosOrOffset, std::shared_ptr<base::WrappedWorldEntity> targetEntity,
		float maxRange, bool isStopAtDestination,
		bool isAutoTurn, bool isInstantTurn,
		std::string& initiatorIdentifier = StringUtil::empty,
		int movementTypeOverride = -1) = 0;

	pub virtual void addAction_ActionSetDirectionalMovementContinuous(
		int controlType,
		Vector2& dir,
		bool isAutoTurn, bool isInstantTurn) = 0;

	pub virtual void addAction_ActionDirectionalMovementBackFrontContinuous(
		int controlType,
		float backFrontMoveVector) = 0;

	pub virtual std::shared_ptr<worldGame3D::EntityAction> buildAction_ActionToTargetTurning(
		int persistenceDurationMS,
		int priority,
		int extraGroupsBitMap,
		int extraPreRequirementsGroupsBitMask,
		int extraReplaceGroupsBitMask,
		int extraOverridesGroupsBitMask,
		float targetRotZDeg, std::shared_ptr<base::WrappedWorldEntity> targetEntity,
		bool isStopAtDestination,
		bool isInstantTurn) = 0;
	pub virtual void addAction_ActionToTargetTurning(
		int persistenceDurationMS,
		int priority,
		int extraGroupsBitMap,
		int extraPreRequirementsGroupsBitMask,
		int extraReplaceGroupsBitMask,
		int extraOverridesGroupsBitMask,
		float targetRotZDeg, std::shared_ptr<base::WrappedWorldEntity> targetEntity,
		bool isStopAtDestination,
		bool isInstantTurn) = 0;

	pub virtual void addAction_teleportToDestination(Vector3& destPos) = 0;

	pub virtual void addAction_stopMovement(bool stopPhysicsLinearVelocitiesAlso) = 0;
	pub virtual void addAction_ActionGeneralStop() = 0;
	pub virtual void addAction_ActionCustomStop(
		int priority,
		int extraReplaceGroupsBitMask) = 0;

	pub virtual void addAction_ActionDirectionalTurnZContinuous(
		Vector1& dir) = 0;

	pub ~IMovingEntityModule() override = default;
};
