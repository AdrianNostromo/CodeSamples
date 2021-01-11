#include "TilingInformation_LinearX.h"

using namespace graphics;

const int TilingInformation_LinearX::TYPE = super::NewType();

TilingInformation_LinearX::TilingInformation_LinearX(
	Vector3Int& tileVoxelsCount, int enabledSidesTriangulationBitMask,
	std::shared_ptr<TileConfig>& tileConfig, int tilingLengthCount)
	: super(TYPE, tileVoxelsCount, enabledSidesTriangulationBitMask),
	tileConfig(tileConfig), tilingLengthCount(tilingLengthCount)
{
	rawSubclassPointer = this;
}

TilingInformation_LinearX::~TilingInformation_LinearX() {
	//void
}
