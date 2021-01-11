#pragma once

#include <base/audio/util/DtAudioData.h>

namespace base {
namespace audio {
class CfgSoundPart {
public:
	// This is unique in the sound.
	// This is set when this part is added to the sound.
	int uid;

public:
    CfgSoundPart(CfgSoundPart const&) = delete;
    CfgSoundPart(CfgSoundPart &&) = default;
    CfgSoundPart& operator=(CfgSoundPart const&) = delete;
    CfgSoundPart& operator=(CfgSoundPart &&) = default;

    explicit CfgSoundPart();

	virtual int computStartFrameIndex(int rate) = 0;
	virtual int computFramesCount(int rate) = 0;

	virtual void rasterize(int rate, DtAudioData::Frame* oFramesList, int framesListCount) = 0;

    virtual ~CfgSoundPart();
};
};
};
