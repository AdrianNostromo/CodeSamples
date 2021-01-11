#include "ProjectileConfigsList.h"
#include <base/statics/StaticsInit.h>
#include <sh/scenarios/battlezones_a_v1/util/Namespace.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <base/list/ArrayList.h>

using namespace battlezones_a_v1;

bool ProjectileConfigsList::IsStaticsInitHooked = base::StaticsInit::AddCbGeneral<bool>(2, ProjectileConfigsList::InitStatics);

rpg3D::ProjectileConfig* ProjectileConfigsList::newEntry(std::string id) {
	return rpg3D::ProjectileConfig::newEntry(Namespace::LOCAL, id);
}

void ProjectileConfigsList::InitStatics() {
	//void
}
