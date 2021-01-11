#pragma once

#include <base/math/Vector2.h>
#include <string>
#include <base/screen/IScreen.h>

class IAppMetrics {
public:
	virtual base::IScreen* getScreen() = 0;

	virtual ~IAppMetrics() = default;
};
