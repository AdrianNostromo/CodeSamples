#pragma once

#include "CfgSoundGenerator.h"

namespace base {
namespace audio {
class CfgSoundGenerator_Tone : public CfgSoundGenerator {
private: typedef CfgSoundGenerator super;
public:
	float frequency = 1000;

	float volume = 1.0f;

public:
	CfgSoundGenerator_Tone(CfgSoundGenerator_Tone const&) = delete;
	CfgSoundGenerator_Tone(CfgSoundGenerator_Tone &&) = default;
	CfgSoundGenerator_Tone& operator=(CfgSoundGenerator_Tone const&) = delete;
	CfgSoundGenerator_Tone& operator=(CfgSoundGenerator_Tone &&) = default;

    explicit CfgSoundGenerator_Tone();

	void rasterize(
		int rate, DtAudioData::Frame* oFramesList, int framesListCount,
		int startFrameIndex, int framesCount) override;

    ~CfgSoundGenerator_Tone() override;
private:
	static inline float sin_minmax_Hz(float centerOffset, float range, float freq_Hz, float t);

};
};
};
