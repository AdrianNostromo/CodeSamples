#include "T_Events.h"

using namespace rpg3D;

T_Events::T_Events(ToolConfigBase* config)
	: super(config)
{
	//void
}

base::TypedEventDispatcher1D<rpg3D::ToolEvent>& T_Events::getEvents() {
	return events;
}

T_Events::~T_Events() {
	//void
}
