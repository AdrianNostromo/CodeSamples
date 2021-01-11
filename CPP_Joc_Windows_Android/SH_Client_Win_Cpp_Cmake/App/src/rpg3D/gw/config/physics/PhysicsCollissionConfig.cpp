#include "PhysicsCollissionConfig.h"
#include <base/math/util/BoolUtil.h>
#include <base/statics/StaticsInit.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace rpg3D;

int PhysicsCollissionConfig::NewInteractionGroup() {
	static int BIT_INDEX_COUNTER = 0;

	int id = BoolUtil::setBitAtIndexDirect(0, BIT_INDEX_COUNTER);

	BIT_INDEX_COUNTER++;

	return id;
}

PhysicsCollissionConfig* PhysicsCollissionConfig::Environment = nullptr;

PhysicsCollissionConfig* PhysicsCollissionConfig::Unit = nullptr;
PhysicsCollissionConfig* PhysicsCollissionConfig::Projectile = nullptr;
PhysicsCollissionConfig* PhysicsCollissionConfig::Sword = nullptr;
PhysicsCollissionConfig* PhysicsCollissionConfig::Platform = nullptr;

PhysicsCollissionConfig* PhysicsCollissionConfig::Unit_Projectile = nullptr;

PhysicsCollissionConfig* PhysicsCollissionConfig::MODIFIER_MainPlayerUnit = nullptr;

bool PhysicsCollissionConfig::IsStaticsInitHooked = base::StaticsInit::AddCbGeneral<bool>(0, []() {
	IsStaticsInitHooked = true;

	// Groups
	Environment = newt PhysicsCollissionConfig(NewInteractionGroup());

	Unit = newt PhysicsCollissionConfig(NewInteractionGroup());
	Projectile = newt PhysicsCollissionConfig(NewInteractionGroup());
	Sword = newt PhysicsCollissionConfig(NewInteractionGroup());
	Unit_Projectile = newt PhysicsCollissionConfig(Unit->groups | Projectile->groups);
	Platform = newt PhysicsCollissionConfig(NewInteractionGroup());

	MODIFIER_MainPlayerUnit = newt PhysicsCollissionConfig(NewInteractionGroup());

	// Masks
	Environment->anyInteraction_mask = Unit->groups | Projectile->groups;
	Environment->hardImpacts_mask = Unit->groups | Projectile->groups;
	
	Unit->anyInteraction_mask = Environment->groups | Projectile->groups | Sword->groups | Platform->groups;
	Unit->hardImpacts_mask = Environment->groups;
	
	Projectile->anyInteraction_mask = Environment->groups | Unit->groups;
	Projectile->hardImpacts_mask = Environment->groups;
	
	Sword->anyInteraction_mask = Unit->groups;
	Sword->hardImpacts_mask = 0;
	
	Platform->anyInteraction_mask = Unit->groups;
	Platform->hardImpacts_mask = 0;

	Unit_Projectile->anyInteraction_mask = Unit->anyInteraction_mask | Projectile->anyInteraction_mask;
	Unit_Projectile->hardImpacts_mask = Unit->hardImpacts_mask | Projectile->hardImpacts_mask;

	MODIFIER_MainPlayerUnit->anyInteraction_mask = 0;
	MODIFIER_MainPlayerUnit->hardImpacts_mask = 0;
});

PhysicsCollissionConfig::PhysicsCollissionConfig(
	int groups)
	: groups(groups), 
	anyInteraction_mask(0), hardImpacts_mask(0)
{
	//void
}
