#include "GPP3D_Contacts.h"
#include <base/exceptions/LogicException.h>
#include <rpg3D/gw/physics3D/actor/IPhysicsActor.h>

using namespace rpg3D;

GPP3D_Contacts::GPP3D_Contacts(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void GPP3D_Contacts::processContactChange(IPhysicsActor* physicsActorA, IPhysicsActor* physicsActorB, bool isContact) {
	// This must support multiple contacts with the same bodies.
	
	physicsActorA->onContactChanged(physicsActorB, isContact);
	physicsActorB->onContactChanged(physicsActorA, isContact);
}

GPP3D_Contacts::~GPP3D_Contacts() {
	//void
}
