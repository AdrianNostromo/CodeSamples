#pragma once

#include <base/gh.h>

namespace rpg3D {
class IHandlerZoneEditorModule {
	pub virtual void zoneEditor_requestStopEditor() = 0;

    pub virtual ~IHandlerZoneEditorModule() = default;
};
};
