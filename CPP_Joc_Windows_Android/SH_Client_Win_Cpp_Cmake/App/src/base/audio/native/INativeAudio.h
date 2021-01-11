#pragma once

#include <base/gh.h>
#include <base/audio/sound/ISound.h>
#include <base/object/IManagedObject.h>

class IApp;

namespace base {
namespace audio {
class INativeAudio : virtual public base::IManagedObject {
	pub virtual void init(IApp* app) = 0;

	pub virtual int getRate() = 0;

	pub virtual void playSound(ISound* sound, float volume) = 0;

	pub virtual void loop_soundsRemoval() = 0;

	pub virtual ~INativeAudio() = default;
};
};
};
