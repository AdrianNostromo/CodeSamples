#include <base/audio/IAudio.h>
#include "Sound.h"

using namespace base::audio;

Sound::Sound(
	base::audio::IAudio* audio,
	std::shared_ptr<DtAudioData>& data)
	: super(),
	audio(audio),
	data(data)
{
	//void
}

std::shared_ptr<DtAudioData> Sound::getData() {
	return data;
}

bool Sound::play(float volume, float overlapCooldownS) {
	//zxc;// this returns a control interface. The interfaces are saved locally in a list also.

	return audio->play(this, volume, overlapCooldownS);
}

Sound::~Sound() {
	//void
}
