#pragma once

#include <base/gh.h>
#include "ZEM_Base.h"

namespace rpg3D {
class ZoneEditorModule : public zoneEditorModule::ZEM_Base {priv typedef zoneEditorModule::ZEM_Base super;pub dCtor(ZoneEditorModule);
    pub explicit ZoneEditorModule(
		IWorldEntity* entity,
		ZoneEditorTemplate* t);

    pub ~ZoneEditorModule() override;
};
};
