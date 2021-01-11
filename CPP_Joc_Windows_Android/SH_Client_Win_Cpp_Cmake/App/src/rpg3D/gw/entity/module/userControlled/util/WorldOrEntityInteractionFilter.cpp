#include "WorldOrEntityInteractionFilter.h"

using namespace rpg3D;

WorldOrEntityInteractionFilter::WorldOrEntityInteractionFilter(const int type)
	: type(type)
{
	//void
}

int WorldOrEntityInteractionFilter::getType() {
	return type;
}

WorldOrEntityInteractionFilter::~WorldOrEntityInteractionFilter() {
	//void
}
