#include "TurnPhaseConfig.h"

using namespace rpg3D;
using namespace rpg3D::playPhases;

TurnPhaseConfig::TurnPhaseConfig(
	std::string name,
	bool isUserEndable)
	: name(std::move(name)),
	isUserEndable(isUserEndable)
{
	//void
}

TurnPhaseConfig::~TurnPhaseConfig() {
	//void
}
