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

using namespace battlezones_a_v1;

std::string SoundConfigsList::NAMESPACE = "battlezones_a_v1";

bool SoundConfigsList::IsStaticsInitHooked = base::StaticsInit::AddCbGeneral<bool>(0, SoundConfigsList::InitStatics);

void SoundConfigsList::InitStatics() {
	//void
}
