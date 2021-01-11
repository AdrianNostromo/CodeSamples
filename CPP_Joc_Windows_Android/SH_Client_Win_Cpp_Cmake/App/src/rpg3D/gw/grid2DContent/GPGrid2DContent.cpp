#include "GPGrid2DContent.h"

using namespace rpg3D;

GPGrid2DContent::GPGrid2DContent(
	base::IGameWorld* gw,
	Vector2 gCellSize, bool allowMultipleEntitiesPerGCell, bool allowEntitiesOutsideOfTheGridArea)
	: super(gw, gCellSize, allowMultipleEntitiesPerGCell, allowEntitiesOutsideOfTheGridArea)
{
	//void
}

GPGrid2DContent::~GPGrid2DContent() {
	//void
}
