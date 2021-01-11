#pragma once

#include <base/assets/AM_Preload.h>
#include <string>

class AssetsManager : public AM_Preload {
private: typedef AM_Preload super;
public:
	AssetsManager(AssetsManager const&) = delete;
	AssetsManager(AssetsManager &&) = default;
	AssetsManager& operator=(AssetsManager const&) = delete;
	AssetsManager& operator=(AssetsManager &&) = default;

	explicit AssetsManager(IApp* app);
};
