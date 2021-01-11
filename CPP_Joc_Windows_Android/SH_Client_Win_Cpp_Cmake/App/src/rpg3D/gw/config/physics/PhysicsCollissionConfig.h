#pragma once

#include <base/gh.h>

namespace rpg3D {
class PhysicsCollissionConfig {
	prot static int NewInteractionGroup();

	priv static bool IsStaticsInitHooked;

	pub static PhysicsCollissionConfig* Environment;

	pub static PhysicsCollissionConfig* Unit;
	pub static PhysicsCollissionConfig* Projectile;
	// Sword is not combined with projectile because projectiles also interract with the environment.
	pub static PhysicsCollissionConfig* Sword;
	pub static PhysicsCollissionConfig* Platform;

	// This is only related to projectiles and not swords also because projectiles interact with the environment and are similar to units. Swords are not combined because they are too different.
	pub static PhysicsCollissionConfig* Unit_Projectile;

	pub static PhysicsCollissionConfig* MODIFIER_MainPlayerUnit;

	pub int groups;
	
	pub int anyInteraction_mask;
	pub int hardImpacts_mask;

	pub explicit PhysicsCollissionConfig(
		int groups);

};
};
