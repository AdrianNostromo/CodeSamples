#pragma once

#include <base/audio/sound/source/generated/generator/CfgSoundGenerator.h>
#include "CfgSoundPart.h"

namespace base {
namespace audio {
class CfgSoundPart_Data : public CfgSoundPart {
private: typedef CfgSoundPart super;
public:
	CfgSoundGenerator* dataGenerator;

	int trackIndex = 0;

	float startTimeS = 0.0f;
	float durationS = 1.0f;

public:
    CfgSoundPart_Data(CfgSoundPart_Data const&) = delete;
    CfgSoundPart_Data(CfgSoundPart_Data &&) = default;
    CfgSoundPart_Data& operator=(CfgSoundPart_Data const&) = delete;
    CfgSoundPart_Data& operator=(CfgSoundPart_Data &&) = default;

    explicit CfgSoundPart_Data(CfgSoundGenerator* dataGenerator);

	int computStartFrameIndex(int rate) override;
	int computFramesCount(int rate) override;

	void rasterize(int rate, DtAudioData::Frame* oFramesList, int framesListCount) override;

    ~CfgSoundPart_Data() override;
};
};
};
