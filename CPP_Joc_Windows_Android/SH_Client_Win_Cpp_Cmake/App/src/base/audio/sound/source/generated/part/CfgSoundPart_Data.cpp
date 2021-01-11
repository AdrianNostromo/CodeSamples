#include "CfgSoundPart_Data.h"
#include <base/math/Math.h>

using namespace base::audio;

CfgSoundPart_Data::CfgSoundPart_Data(CfgSoundGenerator* dataGenerator)
	: dataGenerator(dataGenerator)
{
	//void
}

int CfgSoundPart_Data::computStartFrameIndex(int rate) {
	int ret = Math::round(startTimeS * (float)rate);

	return ret;
}

int CfgSoundPart_Data::computFramesCount(int rate) {
	int ret = Math::round(durationS * (float)rate);

	return ret;
}

void CfgSoundPart_Data::rasterize(int rate, DtAudioData::Frame* oFramesList, int framesListCount) {
	int startFrameIndex = computStartFrameIndex(rate);
	int framesCount = computFramesCount(rate);
	dataGenerator->rasterize(
		rate, oFramesList, framesListCount, 
		startFrameIndex, framesCount
	);
}

CfgSoundPart_Data::~CfgSoundPart_Data() {
	//void
}
