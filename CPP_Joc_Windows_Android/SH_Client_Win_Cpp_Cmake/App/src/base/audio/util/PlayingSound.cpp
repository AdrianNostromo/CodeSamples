#include "PlayingSound.h"

using namespace base::audio;

PlayingSound::PlayingSound(std::shared_ptr<DtAudioData> data, float volume)
	: data(data), volume(volume)
{
	//void
}

PlayingSound::~PlayingSound() {
	//void
}
