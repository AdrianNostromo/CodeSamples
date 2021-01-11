#include "GCPP_Base.h"

using namespace rpg3D;
using namespace rpg3D::playPhases;

GCPP_Base::GCPP_Base(
	base::IGameWorld* gw,
	sp<Array1D<TurnPhaseConfig>> playPhaseConfigsList)
	: super(gw),
	playPhaseConfigsList(playPhaseConfigsList)
{
	//void
}

GCPP_Base::~GCPP_Base() {
	//void
}
