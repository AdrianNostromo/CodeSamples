#pragma once
#include <base/assets/IAssetsManager.h>

namespace base {
namespace audio {
class DynamicSound {
private:
	IAssetsManager* assetsManager;

	std::string* sound_id = nullptr;
	base::audio::ISound* sound = nullptr;

public:
    DynamicSound(DynamicSound const&) = delete;
    DynamicSound(DynamicSound &&) = default;
    DynamicSound& operator=(DynamicSound const&) = delete;
    DynamicSound& operator=(DynamicSound &&) = default;

    explicit DynamicSound(IAssetsManager* assetsManager);

	bool play(float volume, float overlapCooldownS = 0.0f);

	std::string* getSound_id();
	void setSound_id(std::string* sound_id);

    virtual ~DynamicSound();
};
};
};
