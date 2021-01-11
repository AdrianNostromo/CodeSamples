#pragma once

#include <base/gh.h>

namespace graphics {
	class IRenderer;
};

class IStage {
	pub virtual int syncStageDrawSpecs(
		const int parentInvalidationBitMap,
		graphics::IRenderer& renderer) = 0;

	pub virtual ~IStage() = default;
};
