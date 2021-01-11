#include "CfgSoundSource.h"

using namespace base::audio;

int CfgSoundSource::TYPE_COUNTER = 0;

int CfgSoundSource::NewTypeIndex() {
	return ++TYPE_COUNTER;
}

CfgSoundSource::CfgSoundSource(int type)
	: type(type)
{
	//void
}

CfgSoundSource::~CfgSoundSource() {
	//void
}
