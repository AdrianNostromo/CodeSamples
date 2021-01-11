#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include "IZoneEditorM_Edit.h"
#include "IZoneEditorM_View.h"

namespace rpg3D {
	class IEditableVoxelisedModule;
	class IZoneEditorView_Edit;
	class IHandlerZoneEditorModule;
};

namespace rpg3D {
namespace zoneEditorModule {
class IZoneEditorModule : public virtual IEntityModule, virtual public IZoneEditorM_Edit, virtual public IZoneEditorM_View {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub virtual void startEdit(
		IHandlerZoneEditorModule* handler,
		IZoneEditorView_Edit* zoneEditorView,
		std::shared_ptr<graphics::ICinematicController> cinematicController) = 0;
    pub virtual void endEditIfAny() = 0;

    pub ~IZoneEditorModule() override = default;
};
};
};
