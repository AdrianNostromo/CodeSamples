#include "SoundConfigsList.h"
#include <base/statics/StaticsInit.h>
#include <base/audio/sound/source/CfgSoundSource_Generated.h>
#include <base/audio/sound/source/generated/generator/CfgSoundGenerator_Tone.h>
#include <base/audio/sound/source/generated/part/CfgSoundPart_Data.h>
#include <base/MM.h>

using namespace base;

std::string SoundConfigsList::NAMESPACE = "base";

bool SoundConfigsList::IsStaticsInitHooked = base::StaticsInit::AddCbGeneral<bool>(0, SoundConfigsList::InitStatics);

base::audio::CfgSound* SoundConfigsList::UserInterface_Btn_Down = nullptr;

void SoundConfigsList::InitStatics() {
	UserInterface_Btn_Down = GetNew_UserInterface_Btn_Down();
}

base::audio::CfgSound* SoundConfigsList::GetNew_UserInterface_Btn_Down() {
	base::audio::CfgSoundSource_Generated* source = new base::audio::CfgSoundSource_Generated();

	base::audio::CfgSoundGenerator_Tone* sineDataGenerator = new base::audio::CfgSoundGenerator_Tone();
	sineDataGenerator->frequency = 800;
	sineDataGenerator->volume = 0.5f;

	base::audio::CfgSoundPart_Data* sinePart = new base::audio::CfgSoundPart_Data(
		sineDataGenerator
	);

	sinePart->trackIndex = 0;

	sinePart->startTimeS = 0.0f;
	sinePart->durationS = 0.25f;

	source->addPart(sinePart);

	base::audio::CfgSound* cfgSound = base::audio::CfgSound::newEntry(NAMESPACE + ":" + "UserInterface_Btn_Down");
	cfgSound->source = source;

	return cfgSound;
}
