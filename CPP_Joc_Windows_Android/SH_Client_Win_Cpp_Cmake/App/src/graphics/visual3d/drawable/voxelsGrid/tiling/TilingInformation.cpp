#include "TilingInformation.h"

using namespace graphics;

int TilingInformation::NewType() {
	static int TYPE_COUNTER = 0;

	TYPE_COUNTER++;
	return TYPE_COUNTER;
}

TilingInformation::TilingInformation(int type, Vector3Int& tileVoxelsCount, int enabledSidesTriangulationBitMask)
	: type(type), tileVoxelsCount(tileVoxelsCount), enabledSidesTriangulationBitMask(enabledSidesTriangulationBitMask)
{
	//void
}

int TilingInformation::getType() {
	return type;
}


TilingInformation::~TilingInformation() {
	//void
}
