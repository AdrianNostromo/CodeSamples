#include "ADD_Base.h"
#include "../../template/autoDestroyDuration/AutoDestroyDurationTemplate.h"
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>

using namespace rpg3D;

EntityRemoveReason* const ADD_Base::ENT_REM_REASON_PURPOSE_FINISHED = new EntityRemoveReason("ENT_REM_REASON_PURPOSE_FINISHED", true, true);

ADD_Base::ADD_Base(
	IWorldEntity* entity,
	AutoDestroyDurationTemplate* t)
	: super(entity),
	t(t)
{
	//void
}

void ADD_Base::createBMain() {
	super::createBMain();

	//void
}

ADD_Base::~ADD_Base() {
	//void
}
