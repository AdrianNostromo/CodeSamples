#include "CfgSoundGenerator_Tone.h"
#include <base/exceptions/LogicException.h>
#include <cmath>

#define TWO_PI (3.14159265359*2)

using namespace base::audio;

CfgSoundGenerator_Tone::CfgSoundGenerator_Tone() {
	//void
}

void CfgSoundGenerator_Tone::rasterize(
	int rate, DtAudioData::Frame* oFramesList, int framesListCount,
	int startFrameIndex, int framesCount)
{
	float framesDeltaTimeS = 1.0f / (float)rate;

	for (int i = 0; i < framesCount; ++i) {
		float timeS = (float)i * framesDeltaTimeS;

		if (startFrameIndex + i >= framesListCount) {
			throw LogicException(LOC);
		}

		oFramesList[startFrameIndex + i].sample_left = sin_minmax_Hz(0.0f, volume, frequency, timeS);
		oFramesList[startFrameIndex + i].sample_right = sin_minmax_Hz(0.0f, volume, frequency, timeS);

		//printf("alpha: %f\n", framesList[i].sample_left);
	}
}

inline float CfgSoundGenerator_Tone::sin_minmax_Hz(float centerOffset, float range, float freq_Hz, float t) {
	return centerOffset + range * sin(t * freq_Hz * TWO_PI);
}

CfgSoundGenerator_Tone::~CfgSoundGenerator_Tone() {
	//void
}
