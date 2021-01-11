#pragma once

#include <base/audio/util/DtAudioData.h>

namespace base {
namespace audio {
class CfgSoundGenerator {
public:
    CfgSoundGenerator(CfgSoundGenerator const&) = delete;
    CfgSoundGenerator(CfgSoundGenerator &&) = default;
    CfgSoundGenerator& operator=(CfgSoundGenerator const&) = delete;
    CfgSoundGenerator& operator=(CfgSoundGenerator &&) = default;

    explicit CfgSoundGenerator();

	virtual void rasterize(
		int rate, DtAudioData::Frame* oFramesList, int framesListCount,
		int startFrameIndex, int framesCount) = 0;

    virtual ~CfgSoundGenerator();
};
};
};
