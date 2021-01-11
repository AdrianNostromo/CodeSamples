#pragma once

#include <base/gh.h>
#include "TilingInformation.h"
#include <memory>

namespace graphics {
class TilingInformation_LinearX : public TilingInformation {priv typedef TilingInformation super;pub dCtor(TilingInformation_LinearX);
	pub static const int TYPE;

    pub std::shared_ptr<TileConfig> tileConfig;
    pub int tilingLengthCount;
    
    pub explicit TilingInformation_LinearX(
        Vector3Int& tileVoxelsCount, int enabledSidesTriangulationBitMask,
        std::shared_ptr<TileConfig>& tileConfig, int tilingLengthCount);

    pub ~TilingInformation_LinearX() override;
};
};
