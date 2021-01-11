#include "CfgSoundSource_Generated.h"
#include <base/math/Math.h>

using namespace base::audio;

const int CfgSoundSource_Generated::TYPE = super::NewTypeIndex();

CfgSoundSource_Generated::CfgSoundSource_Generated()
	: super(TYPE)
{
	//void
}

void CfgSoundSource_Generated::addPart(CfgSoundPart* soundPart) {
	soundPart->uid = ++partUidCounter;
	soundPartsList.appendDirect(soundPart);
}

int CfgSoundSource_Generated::computeFramesCount(int rate) {
	int framesCount = 0;
	for (int i = 0; i < soundPartsList.count(); i++) {
		CfgSoundPart* soundPart = soundPartsList.getDirect(i);

		framesCount = Math::max(framesCount, soundPart->computStartFrameIndex(rate) + soundPart->computFramesCount(rate));
	}

	return framesCount;
}

void CfgSoundSource_Generated::rasterize(int rate, DtAudioData::Frame* oFramesList, int framesListCount) {
	for (int i = 0; i < soundPartsList.count(); i++) {
		CfgSoundPart* soundPart = soundPartsList.getDirect(i);

		soundPart->rasterize(rate, oFramesList, framesListCount);
	}
}

CfgSoundSource_Generated::~CfgSoundSource_Generated() {
	//void
}
