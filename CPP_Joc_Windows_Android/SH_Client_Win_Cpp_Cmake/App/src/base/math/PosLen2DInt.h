#pragma once

#include <base/gh.h>
#include "Vector2Int.h"

class PosLen2DInt {pub dCtor(PosLen2DInt);
	pub Vector2Int pos{};
	pub Vector2Int len{};

	pub explicit PosLen2DInt();

    ~PosLen2DInt();
};
