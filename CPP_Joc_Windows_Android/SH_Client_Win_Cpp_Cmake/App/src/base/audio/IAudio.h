#pragma once

#include <base/audio/sound/ISound.h>

namespace base {
namespace audio {
class IAudio {
public:
	virtual int getRate() = 0;

	// Returns true if sound played, false if not played (eg. overlapCooldownS prohibbited playing).
	virtual bool play(ISound* sound, float volume, float overlapCooldownS = 0.0f) = 0;

    virtual ~IAudio() = default;
};
};
};
