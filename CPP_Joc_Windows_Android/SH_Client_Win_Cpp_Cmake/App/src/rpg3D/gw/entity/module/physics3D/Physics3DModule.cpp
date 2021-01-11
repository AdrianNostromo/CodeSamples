#include "Physics3DModule.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include "../../template/physics3D/Physics3DTemplate.h"

using namespace rpg3D;

Physics3DModule::Physics3DModule(
	IWorldEntity* entity,
	Physics3DTemplate* t,
	int physicsGroupsBitMap,
	int anyInteraction_maskBitMap, int hardImpact_maskBitMap)
	: super(entity, t,
		physicsGroupsBitMap,
		anyInteraction_maskBitMap, hardImpact_maskBitMap)
{
	//void
}

Physics3DModule::~Physics3DModule() {
	//void
}
