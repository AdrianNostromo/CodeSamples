#include "GWLBase.h"
#include <base/math/Math.h>

using namespace battlezones_a_v1;

GWLBase::GWLBase(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig)
	: super(handler, scenarioTemplate, scenarioConfig)
{
	//void
}

void GWLBase::disposeMain() {
    //void

    super::disposeMain();
}

GWLBase::~GWLBase() {
    //void
}
