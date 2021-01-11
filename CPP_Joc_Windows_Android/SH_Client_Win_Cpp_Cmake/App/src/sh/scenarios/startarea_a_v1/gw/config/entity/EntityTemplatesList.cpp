#include "EntityTemplatesList.h"
#include <sh/scenarios/startarea_a_v1/gw/config/entity/platform/PlatformTemplates_TeleporterA.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/platform/Templates_Platform_TeleporterDevA.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/platform/PlatformTemplates_MatchFindA.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/platform/PlatformTemplates_BasicA.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/platform/PlatformTemplates_PaintPad.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/flora/FloraTemplates_TreeA.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/doodad/DoodadTemplates_PlayerSpawnIndicatorA.h>
#include <base/statics/StaticsInit.h>

using namespace startarea_a_v1;

std::string EntityTemplatesList::NAMESPACE = "startarea_a_v1";

bool EntityTemplatesList::IsStaticsInitHooked = base::StaticsInit::AddCbGeneral<bool>(1, EntityTemplatesList::InitStatics);

EntityTemplate* EntityTemplatesList::Platform_TeleporterA = nullptr;
EntityTemplate* EntityTemplatesList::Platform_TeleporterDevA = nullptr;
EntityTemplate* EntityTemplatesList::Platform_MatchFindA = nullptr;
EntityTemplate* EntityTemplatesList::Platform_BasicA = nullptr;
EntityTemplate* EntityTemplatesList::Platform_PaintPad = nullptr;

EntityTemplate* EntityTemplatesList::Flora_TreeA = nullptr;

EntityTemplate* EntityTemplatesList::Doodad_PlayerSpawnIndicatorA = nullptr;

EntityTemplate* EntityTemplatesList::newEntry(std::string id) {
    return EntityTemplate::newEntry(NAMESPACE, id);
}

void EntityTemplatesList::InitStatics() {
    Platform_TeleporterA = PlatformTemplates_TeleporterA::GetNew();
	Platform_TeleporterDevA = Templates_Platform_TeleporterDevA::GetNew();
    Platform_MatchFindA = PlatformTemplates_MatchFindA::GetNew();
    Platform_BasicA = PlatformTemplates_BasicA::GetNew();
    Platform_PaintPad = PlatformTemplates_PaintPad::GetNew();

    Flora_TreeA = FloraTemplates_TreeA::GetNew();

    Doodad_PlayerSpawnIndicatorA = DoodadTemplates_PlayerSpawnIndicatorA::GetNew();

}
