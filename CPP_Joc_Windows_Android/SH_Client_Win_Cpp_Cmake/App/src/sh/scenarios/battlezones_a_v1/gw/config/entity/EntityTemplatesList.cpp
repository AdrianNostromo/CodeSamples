#include <sh/scenarios/battlezones_a_v1/gw/config/entity/platform/PlatformTemplate_ZoneTeleporter_Wood.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/mob/Template_Mob_General_A_V1.h>
#include "EntityTemplatesList.h"
#include <base/statics/StaticsInit.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/platform/EntityTemplate_Platform_NexusTeleporter_Wood_A_V1.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/doodad/EntityTemplate_Doodad_BrokenZoneTeleporter_Stone_A_V1.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/doodad/EntityTemplate_Doodad_BrokenNexusTeleporter_Stone_A_V1.h>

using namespace battlezones_a_v1;

std::string EntityTemplatesList::NAMESPACE = "battlezones_a_v1";

bool EntityTemplatesList::IsStaticsInitHooked = base::StaticsInit::AddCbGeneral<bool>(1, EntityTemplatesList::InitStatics);

EntityTemplate* EntityTemplatesList::Mob_General_A_V1 = nullptr;

EntityTemplate* EntityTemplatesList::Doodad_BrokenZoneTeleporter_Stone_A_V1 = nullptr;
EntityTemplate* EntityTemplatesList::Doodad_BrokenNexusTeleporter_Stone_A_V1 = nullptr;

EntityTemplate* EntityTemplatesList::Platform_ZoneTeleporter_Wood = nullptr;
EntityTemplate* EntityTemplatesList::Platform_NexusTeleporter_Wood_A_V1 = nullptr;

EntityTemplate* EntityTemplatesList::newEntry(std::string id) {
    return EntityTemplate::newEntry(NAMESPACE, id);
}

void EntityTemplatesList::InitStatics() {
    Mob_General_A_V1 = Template_Mob_General_A_V1::getNew();

    Doodad_BrokenZoneTeleporter_Stone_A_V1 = EntityTemplate_Doodad_BrokenZoneTeleporter_Stone_A_V1::GetNew();
    Doodad_BrokenNexusTeleporter_Stone_A_V1 = EntityTemplate_Doodad_BrokenNexusTeleporter_Stone_A_V1::GetNew();

    Platform_ZoneTeleporter_Wood = PlatformTemplate_ZoneTeleporter_Wood::getNew();
    Platform_NexusTeleporter_Wood_A_V1 = EntityTemplate_Platform_NexusTeleporter_Wood_A_V1::getNew();
}
