#pragma once

#include <base/object/Object.h>
#include <base/audio/sound/util/CfgSound.h>
#include "ISound.h"

namespace base {
namespace audio {
class IAudio;
};
};

namespace base {
namespace audio {
class Sound : public Object, virtual public ISound {
private: typedef Object super;
public:
	base::audio::IAudio* audio;

	std::shared_ptr<DtAudioData> data;
	
public:
    Sound(Sound const&) = delete;
    Sound(Sound &&) = default;
    Sound& operator=(Sound const&) = delete;
    Sound& operator=(Sound &&) = default;

    explicit Sound(
		base::audio::IAudio* audio, 
		std::shared_ptr<DtAudioData>& data);

	std::shared_ptr<DtAudioData> getData() override;

	bool play(float volume, float overlapCooldownS = 0.0f) final;

    ~Sound() override;
};
};
};
