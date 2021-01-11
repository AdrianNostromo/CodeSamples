#include "IM_Base.h"
#include "../../template/inventory/InventoryTemplate.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

IM_Base::IM_Base(
	IWorldEntity* entity,
	InventoryTemplate* t)
	: super(entity),
	t(t)
{
	//void
}

float IM_Base::getGoldAmount() {
	return goldAmount;
}

float IM_Base::addGoldAmount(float goldAmountAddition) {
	if (goldAmountAddition == 0) {
		return this->goldAmount;
	}

	this->goldAmount += goldAmountAddition;

	e->dispatchEvent(*EVENT_inventory_goldAmountChanged->setEntityData(e));

	return this->goldAmount;
}

IM_Base::~IM_Base() {
	//void
}
