#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IStackableModule.h"

namespace rpg3D {
	class StackableTemplate;
	class IConstraint3D;
};

namespace rpg3D {
class SM_Base : public EntityModule, virtual public IStackableModule {priv typedef EntityModule super;
	prot StackableTemplate* t;
	
	priv IStackableModule* haulerStackable = nullptr;
	priv IStackableModule* carryingStackable = nullptr;
	priv IConstraint3D* carryingPhysicsConstraint = nullptr;

	priv float haulOffsetZM;

	priv rpg3D::IPhysics3DModule* physics3DModule = nullptr;

	pub dCtor(SM_Base);
    pub explicit SM_Base(
		IWorldEntity* entity,
		StackableTemplate* t);
	prot void createBMain() override;

	pub StackableTemplate* getT() override;
	
	pub IPhysics3DModule* getPhysics3DModule() final;

	pub float getHaulOffsetZM() override;
	pub void setHaulOffsetZM(float haulOffsetZM) override;

	pub bool getIsHauled() override;
	pub bool getIsCarrying() override;

	pub bool getIsCarryingIndirect(IStackableModule* targetStackable) override;

	pub IStackableModule* getCarrying() override;
	pub void setCarrying(IStackableModule* carryingStackable, std::string& initiatorIdentifier = StringUtil::empty) override;

	pub IStackableModule* getBottomStackable() override;
	pub IStackableModule* getTopStackable() override;
		
	pub IStackableModule* getHauler() override;
	pub void setHauler(IStackableModule* haulerStackable, std::string& initiatorIdentifier = StringUtil::empty) override;

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	prot void disposePre() override;
    pub ~SM_Base() override;
};
};
