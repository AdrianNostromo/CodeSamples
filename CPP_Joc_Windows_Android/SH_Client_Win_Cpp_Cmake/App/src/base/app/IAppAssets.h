#pragma once

#include <base/animator/IAnimator.h>

class IAssetsManager;
class INativeInput;

class IAppAssets {
public:
	virtual INativeInput* getNativeInput() = 0;

	virtual IAssetsManager* getEmbeddedAssetsManager() = 0;

    virtual base::IAnimator* getAnimator() = 0;

	virtual ~IAppAssets() = default;
};
