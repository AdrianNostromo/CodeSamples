#pragma once

#include <base/gh.h>

namespace rpg3D {
	class IZoneEditorM_Edit;
	class IListenerZoneEditorView;
};

namespace rpg3D {
class IZoneEditorView_Edit {
	pub virtual void setHandlerAndListener(IZoneEditorM_Edit* entityEditor, IListenerZoneEditorView* listener) = 0;
	
	pub virtual ~IZoneEditorView_Edit() = default;
};
};
