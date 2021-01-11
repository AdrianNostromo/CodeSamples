#include "DtAudioData.h"

using namespace base::audio;

DtAudioData::DtAudioData(Frame* framesList, int framesCount, float durationS)
	: framesList(framesList), framesCount(framesCount), durationS(durationS)
{
	//void
}

DtAudioData::~DtAudioData() {
	if (framesList != nullptr) {
		delete[] framesList;

		framesList = nullptr;
	}
}
