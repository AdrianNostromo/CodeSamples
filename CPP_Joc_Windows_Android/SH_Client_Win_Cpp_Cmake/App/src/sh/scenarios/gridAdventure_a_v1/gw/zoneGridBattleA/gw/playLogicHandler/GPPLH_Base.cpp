#include "GPPLH_Base.h"
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>
#include <rpg3D/gw/ui/IGamePartUserInterface.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/ui/GPUIMainUIView.h>
#include <sh/scenarios/startarea_a_v1/gw/config/ScenarioFactory.h>
#include <memory>
#include <worldGame3D/gw/handler/GWEventDataBase.h>

using namespace gridAdventure_a_v1;
using namespace gridAdventure_a_v1::GridBattleA;

GPPLH_Base::GPPLH_Base(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

GPPLH_Base::~GPPLH_Base() {
	//void
}
