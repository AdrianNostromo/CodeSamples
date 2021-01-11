#include "EntityTemplatesList.h"
#include <base/statics/StaticsInit.h>

using namespace base;

std::string EntityTemplatesList::NAMESPACE = "base";

bool EntityTemplatesList::IsStaticsInitHooked = base::StaticsInit::AddCbGeneral<bool>(1, EntityTemplatesList::InitStatics);

EntityTemplate* EntityTemplatesList::newEntry(std::string id) {
	return EntityTemplate::newEntry(NAMESPACE, id);
}

void EntityTemplatesList::InitStatics() {
	//void
}
