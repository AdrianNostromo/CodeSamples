#pragma once

#include <base/gh.h>
#include "VEM_Base.h"

namespace rpg3D {
	class IToolsHandlerModule;
};

namespace rpg3D {
class VEM_ColorsSelectorHandler : public entityEditorModule::VEM_Base {priv typedef entityEditorModule::VEM_Base super;pub dCtor(VEM_ColorsSelectorHandler);
	priv ArrayList<std::shared_ptr<IEventListener>> local_autoListenersList{};

	priv rpg3D::IToolsHandlerModule* toolsHandler = nullptr;

    pub explicit VEM_ColorsSelectorHandler(
		IWorldEntity* entity,
		VoxelizedEditorTemplate* t);

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	prot void onStartEditPost() override;
	prot void onEndEditPre() override;
	
    pub ~VEM_ColorsSelectorHandler() override;
};
};
