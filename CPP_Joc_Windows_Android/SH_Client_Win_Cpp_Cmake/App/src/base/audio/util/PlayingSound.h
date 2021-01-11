#pragma once

#include "IPlayingSound.h"
#include <memory>
#include <base/audio/sound/ISound.h>

namespace base {
namespace audio {
class PlayingSound : virtual public IPlayingSound {
public:
	std::shared_ptr<DtAudioData> data;
	float volume;

	int frameIndex = 0;

public:
    PlayingSound(PlayingSound const&) = delete;
    PlayingSound(PlayingSound &&) = default;
    PlayingSound& operator=(PlayingSound const&) = delete;
    PlayingSound& operator=(PlayingSound &&) = default;

    explicit PlayingSound(std::shared_ptr<DtAudioData> data, float volume);

    ~PlayingSound() override;
};
};
};
