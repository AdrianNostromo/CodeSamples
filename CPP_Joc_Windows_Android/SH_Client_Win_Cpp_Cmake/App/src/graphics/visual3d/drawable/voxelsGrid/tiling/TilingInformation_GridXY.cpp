#include "TilingInformation_GridXY.h"

using namespace graphics;

const int TilingInformation_GridXY::TYPE = super::NewType();

TilingInformation_GridXY::TilingInformation_GridXY(
	Vector3Int& tileVoxelsCount, int enabledSidesTriangulationBitMask,
	std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>>& tilesConfigList,
	std::shared_ptr<Array2D<signed char>>& tilingGrid, sp<Vector3> geometryGridOffset)
	: super(TYPE, tileVoxelsCount, enabledSidesTriangulationBitMask),
	tilesConfigList(tilesConfigList), tilingGrid(tilingGrid),
	geometryGridOffset(geometryGridOffset)
{
	rawSubclassPointer = this;
}

TilingInformation_GridXY::~TilingInformation_GridXY() {
	//void
}
