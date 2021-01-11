#include "GCPlayPhases.h"

using namespace rpg3D;
using namespace rpg3D::playPhases;

GCPlayPhases::GCPlayPhases(
	base::IGameWorld* gw,
	sp<Array1D<TurnPhaseConfig>> playPhaseConfigsList)
	: super(
		gw,
		playPhaseConfigsList
	)
{
	//void
}

GCPlayPhases::~GCPlayPhases() {
	//void
}
