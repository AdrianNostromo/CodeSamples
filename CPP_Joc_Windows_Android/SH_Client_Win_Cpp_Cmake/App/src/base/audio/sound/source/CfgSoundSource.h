#pragma once

#include <base/audio/util/DtAudioData.h>

namespace base {
namespace audio {
class CfgSoundSource {
protected:
	static int TYPE_COUNTER;
	static int NewTypeIndex();

public:
	int type;

public:
    CfgSoundSource(CfgSoundSource const&) = delete;
    CfgSoundSource(CfgSoundSource &&) = default;
    CfgSoundSource& operator=(CfgSoundSource const&) = delete;
    CfgSoundSource& operator=(CfgSoundSource &&) = default;

    explicit CfgSoundSource(int type);

	virtual int computeFramesCount(int rate) = 0;
	virtual void rasterize(int rate, DtAudioData::Frame* oFramesList, int framesListCount) = 0;

    virtual ~CfgSoundSource();
};
};
};
