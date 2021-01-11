#include "SoundConfigsList.h"
#include <rpg3D/gw/config/entity/unit/UnitTemplates_PlayerA.h>
#include <rpg3D/gw/config/entity/platform/PlatformTemplates_TeleporterA.h>
#include <rpg3D/gw/config/entity/platform/PlatformTemplates_MatchFindA.h>
#include <rpg3D/gw/config/entity/platform/PlatformTemplates_BasicA.h>
#include <rpg3D/gw/config/entity/platform/PlatformTemplates_PaintPad.h>
#include <rpg3D/gw/config/entity/flora/FloraTemplates_TreeA.h>
#include <rpg3D/gw/config/entity/doodad/Template_Doodad_BrokenTeleporter_Stone_A_V1.h>
#include <rpg3D/gw/config/entity/text/TextTemplate_TeleporterPainter.h>
#include <rpg3D/gw/config/entity/text/TextTemplate_TeleporterRogueZone.h>
#include <base/statics/StaticsInit.h>
#include <base/audio/sound/source/CfgSoundSource_Generated.h>
#include <base/audio/sound/source/generated/generator/CfgSoundGenerator_Tone.h>
#include <base/audio/sound/source/generated/part/CfgSoundPart_Data.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace rpg3D;

std::string SoundConfigsList::NAMESPACE = "rpg3D";

bool SoundConfigsList::IsStaticsInitHooked = base::StaticsInit::AddCbGeneral<bool>(0, SoundConfigsList::InitStatics);

base::audio::CfgSound* SoundConfigsList::EntityEditor_ToolActivate_A_V1 = nullptr;
base::audio::CfgSound* SoundConfigsList::ZoneEditor_ToolActivate_A_V1 = nullptr;

base::audio::CfgSound* SoundConfigsList::StackablesHauler_Activate_A_V1 = nullptr;

base::audio::CfgSound* SoundConfigsList::WoodBow_Fire_A_V1 = nullptr;
base::audio::CfgSound* SoundConfigsList::Sword_Activate_A_V1 = nullptr;
base::audio::CfgSound* SoundConfigsList::Sword_Impact_A_V1 = nullptr;
base::audio::CfgSound* SoundConfigsList::Projectile_Arrow_Basic_Impact_A_V1 = nullptr;

void SoundConfigsList::InitStatics() {
	EntityEditor_ToolActivate_A_V1 = GetNew_EntityEditor_ToolActivate_A_V1();
	ZoneEditor_ToolActivate_A_V1 = GetNew_ZoneEditor_ToolActivate_A_V1();

	StackablesHauler_Activate_A_V1 = GetNew_StackablesHauler_Activate_A_V1();
	
	WoodBow_Fire_A_V1 = GetNew_WoodBow_Fire_A_V1();
	Sword_Activate_A_V1 = GetNew_Sword_Activate_A_V1();
	Sword_Impact_A_V1 = GetNew_Sword_Impact_A_V1();
	Projectile_Arrow_Basic_Impact_A_V1 = GetNew_Projectile_Arrow_Basic_Impact_A_V1();
}

base::audio::CfgSound* SoundConfigsList::GetNew_EntityEditor_ToolActivate_A_V1() {
	base::audio::CfgSoundSource_Generated* source = newt base::audio::CfgSoundSource_Generated();

	base::audio::CfgSoundGenerator_Tone* sineDataGenerator = newt base::audio::CfgSoundGenerator_Tone();
	sineDataGenerator->frequency = 800;
	sineDataGenerator->volume = 0.5f;

	base::audio::CfgSoundPart_Data* sinePart = newt base::audio::CfgSoundPart_Data(
		sineDataGenerator
	);

	sinePart->trackIndex = 0;

	sinePart->startTimeS = 0.0f;
	sinePart->durationS = 0.25f;

	source->addPart(sinePart);

	base::audio::CfgSound* cfgSound = base::audio::CfgSound::newEntry(NAMESPACE + ":" + "EntityEditor_ToolActivate_A_V1");
	cfgSound->source = source;

	return cfgSound;
}

base::audio::CfgSound* SoundConfigsList::GetNew_ZoneEditor_ToolActivate_A_V1() {
	base::audio::CfgSoundSource_Generated* source = newt base::audio::CfgSoundSource_Generated();

	base::audio::CfgSoundGenerator_Tone* sineDataGenerator = newt base::audio::CfgSoundGenerator_Tone();
	sineDataGenerator->frequency = 800;
	sineDataGenerator->volume = 0.5f;

	base::audio::CfgSoundPart_Data* sinePart = newt base::audio::CfgSoundPart_Data(
		sineDataGenerator
	);

	sinePart->trackIndex = 0;

	sinePart->startTimeS = 0.0f;
	sinePart->durationS = 0.25f;

	source->addPart(sinePart);

	base::audio::CfgSound* cfgSound = base::audio::CfgSound::newEntry(NAMESPACE + ":" + "ZoneEditor_ToolActivate_A_V1");
	cfgSound->source = source;

	return cfgSound;
}

base::audio::CfgSound* SoundConfigsList::GetNew_StackablesHauler_Activate_A_V1() {
	base::audio::CfgSoundSource_Generated* source = newt base::audio::CfgSoundSource_Generated();

	base::audio::CfgSoundGenerator_Tone* sineDataGenerator = newt base::audio::CfgSoundGenerator_Tone();
	sineDataGenerator->frequency = 800;
	sineDataGenerator->volume = 0.5f;

	base::audio::CfgSoundPart_Data* sinePart = newt base::audio::CfgSoundPart_Data(
		sineDataGenerator
	);

	sinePart->trackIndex = 0;

	sinePart->startTimeS = 0.0f;
	sinePart->durationS = 0.25f;

	source->addPart(sinePart);

	base::audio::CfgSound* cfgSound = base::audio::CfgSound::newEntry(NAMESPACE + ":" + "StackablesHauler_Activate_A_V1");
	cfgSound->source = source;

	return cfgSound;
}

base::audio::CfgSound* SoundConfigsList::GetNew_WoodBow_Fire_A_V1() {
	base::audio::CfgSoundSource_Generated* source = newt base::audio::CfgSoundSource_Generated();

	base::audio::CfgSoundGenerator_Tone* sineDataGenerator = newt base::audio::CfgSoundGenerator_Tone();
	sineDataGenerator->frequency = 800;
	sineDataGenerator->volume = 0.5f;

	base::audio::CfgSoundPart_Data* sinePart = newt base::audio::CfgSoundPart_Data(
		sineDataGenerator
	);

	sinePart->trackIndex = 0;

	sinePart->startTimeS = 0.0f;
	sinePart->durationS = 0.25f;

	source->addPart(sinePart);

	base::audio::CfgSound* cfgSound = base::audio::CfgSound::newEntry(NAMESPACE + ":" + "WoodBow_Fire_A_V1");
	cfgSound->source = source;

	return cfgSound;
}

base::audio::CfgSound* SoundConfigsList::GetNew_Sword_Activate_A_V1() {
	base::audio::CfgSoundSource_Generated* source = newt base::audio::CfgSoundSource_Generated();

	base::audio::CfgSoundGenerator_Tone* sineDataGenerator = newt base::audio::CfgSoundGenerator_Tone();
	sineDataGenerator->frequency = 600;
	sineDataGenerator->volume = 0.5f;

	base::audio::CfgSoundPart_Data* sinePart = newt base::audio::CfgSoundPart_Data(
		sineDataGenerator
	);

	sinePart->trackIndex = 0;

	sinePart->startTimeS = 0.0f;
	sinePart->durationS = 0.25f;

	source->addPart(sinePart);

	base::audio::CfgSound* cfgSound = base::audio::CfgSound::newEntry(NAMESPACE + ":" + "Sword_Activate_A_V1");
	cfgSound->source = source;

	return cfgSound;
}

base::audio::CfgSound* SoundConfigsList::GetNew_Sword_Impact_A_V1() {
	base::audio::CfgSoundSource_Generated* source = newt base::audio::CfgSoundSource_Generated();

	base::audio::CfgSoundGenerator_Tone* sineDataGenerator = newt base::audio::CfgSoundGenerator_Tone();
	sineDataGenerator->frequency = 400;
	sineDataGenerator->volume = 0.5f;

	base::audio::CfgSoundPart_Data* sinePart = newt base::audio::CfgSoundPart_Data(
		sineDataGenerator
	);

	sinePart->trackIndex = 0;

	sinePart->startTimeS = 0.0f;
	sinePart->durationS = 0.25f;

	source->addPart(sinePart);

	base::audio::CfgSound* cfgSound = base::audio::CfgSound::newEntry(NAMESPACE + ":" + "Sword_Impact_A_V1");
	cfgSound->source = source;

	return cfgSound;
}

base::audio::CfgSound* SoundConfigsList::GetNew_Projectile_Arrow_Basic_Impact_A_V1() {
	base::audio::CfgSoundSource_Generated* source = newt base::audio::CfgSoundSource_Generated();

	base::audio::CfgSoundGenerator_Tone* sineDataGenerator = newt base::audio::CfgSoundGenerator_Tone();
	sineDataGenerator->frequency = 600;
	sineDataGenerator->volume = 0.5f;

	base::audio::CfgSoundPart_Data* sinePart = newt base::audio::CfgSoundPart_Data(
		sineDataGenerator
	);

	sinePart->trackIndex = 0;

	sinePart->startTimeS = 0.0f;
	sinePart->durationS = 0.2f;

	source->addPart(sinePart);

	base::audio::CfgSound* cfgSound = base::audio::CfgSound::newEntry(NAMESPACE + ":" + "Projectile_Arrow_Basic_Impact_A_V1");
	cfgSound->source = source;

	return cfgSound;
}
