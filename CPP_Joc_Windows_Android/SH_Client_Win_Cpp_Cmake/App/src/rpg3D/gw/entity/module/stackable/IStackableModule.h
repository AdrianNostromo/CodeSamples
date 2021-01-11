#pragma once

#include <base/gh.h>
#include <base/math/Vector3.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>
#include <base/util/StringUtil.h>

namespace rpg3D {
	class StackableTemplate;
	class IPhysics3DModule;
};
namespace base {
	class WrappedWorldEntity;
};

namespace rpg3D {
class IStackableModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub static int ActionGroup_StackablePickupOrThrow;
	pub static int ActionGroup_StackableTurning;
	
	pub static base::EntityEvent* EVENT_haulerChanged;
	pub static base::EntityEvent* EVENT_carryingChanged;
	
	pub static base::EntityEvent* EVENT_stackablePickup_Start;
	
	pub static base::EntityEvent* EVENT_stackableThrow_Start;

	pub virtual StackableTemplate* getT() = 0;

	pub virtual IPhysics3DModule* getPhysics3DModule() = 0;

	pub virtual float getHaulOffsetZM() = 0;
	pub virtual void setHaulOffsetZM(float haulOffsetZM) = 0;

	pub virtual bool getIsHauled() = 0;
	pub virtual bool getIsCarrying() = 0;

	pub virtual bool getIsCarryingIndirect(IStackableModule* targetStackable) = 0;
	
	pub virtual IStackableModule* getCarrying() = 0;
	pub virtual void setCarrying(IStackableModule* carryingStackable, std::string& initiatorIdentifier = StringUtil::empty) = 0;
	
	pub virtual IStackableModule* getBottomStackable() = 0;
	pub virtual IStackableModule* getTopStackable() = 0;

	pub virtual IStackableModule* getHauler() = 0;
	pub virtual void setHauler(IStackableModule* haulerStackable, std::string& initiatorIdentifier = StringUtil::empty) = 0;

	pub virtual void addAction_StackablePickupActionContinuous(
		std::shared_ptr<base::WrappedWorldEntity> targetEntity, float actionRange) = 0;

	// If throwPosOrOffset is used, the throw occurs on the ground.
	pub virtual void addAction_StackableThrowActionContinuous(
		std::shared_ptr<base::WrappedWorldEntity>& carriedEntityToThrow,
		std::shared_ptr<base::WrappedWorldEntity> throwDestinationHaulerEntity/*optional*/, std::shared_ptr<Vector3> throwPosOrOffset,
		float throwRange,
		std::shared_ptr<float> throwRotZDeg) = 0;

    pub ~IStackableModule() override = default;
};
};
