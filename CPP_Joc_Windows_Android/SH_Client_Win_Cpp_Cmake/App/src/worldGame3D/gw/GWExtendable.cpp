#include "GWExtendable.h"
#include <worldGame3D/gw/entityGroups/GCEntityGroups.h>

using namespace base;

GWExtendable::GWExtendable(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig)
	: super(handler, scenarioTemplate, scenarioConfig)
{
	//void
}

void GWExtendable::createGParts() {
	super::createGParts();

	addGamePartUninitialised(new worldGame3D::GCEntityGroups(this));
}

GWExtendable::~GWExtendable() {
	//void
}
