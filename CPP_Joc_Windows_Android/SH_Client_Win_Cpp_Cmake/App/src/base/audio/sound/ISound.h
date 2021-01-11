#pragma once

#include <base/gh.h>
#include <base/object/IObject.h>
#include <memory>
#include <base/audio/util/DtAudioData.h>

namespace base {
namespace audio {
class ISound : virtual public IObject {
	pub virtual std::shared_ptr<DtAudioData> getData() = 0;

	priv float lastPlayTimerSRef = -1.0f;
	pub float& getLastPlayTimerSRef() { return lastPlayTimerSRef; };
	
	// Returns true if sound played, false if not played (eg. overlapCooldownS prohibbited playing).
	pub virtual bool play(float volume, float overlapCooldownS = 0.0f) = 0;

	pub ~ISound() override = default;
};
};
};
