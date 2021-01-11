#pragma once

#include <base/gh.h>
#include "../IRenderPhase.h"

class Texture;

namespace graphics {
class IRP3DShadowMap : virtual public IRenderPhase {
	pub static int TYPE;
	pub int getType() final;

	pub virtual Texture* getShadowDepthMapTexture() = 0;

	pub virtual ~IRP3DShadowMap() = default;
};
};
