#include "ZoneTemplatesList.h"
#include "ZoneTemplate_Nexus_A_V1.h"
#include "ZoneTemplate_Battle_WESN_A_V1.h"
#include <base/statics/StaticsInit.h>

using namespace battlezones_a_v1;

bool ZoneTemplatesList::IsStaticsInitHooked = base::StaticsInit::AddCbGeneral<bool>(2, ZoneTemplatesList::InitStatics);

ZoneTemplate* ZoneTemplatesList::Nexus_A_V1 = nullptr;

ZoneTemplate* ZoneTemplatesList::Battle_WESN_A_V1 = nullptr;

void ZoneTemplatesList::InitStatics() {
	Nexus_A_V1 = ZoneTemplate_Nexus_A_V1::getNew();

	Battle_WESN_A_V1 = ZoneTemplate_Battle_WESN_A_V1::getNew();
}
