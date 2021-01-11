#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/customLogic/CustomLogicModule.h>
#include "ICustomLogicModule_EntityEditorPlatform.h"
#include <worldGame3D/gw/entity/template/customLogic/CustomLogicTemplate.h>

class IDrawableNode3D;
namespace base {
	class GWEvent;
}
namespace rpg3D {
	class IPlatformModule;
}

namespace rpg3D {
class CustomLogicModule_EntityEditorPlatform : public base::CustomLogicModule, public virtual ICustomLogicModule_EntityEditorPlatform {priv typedef base::CustomLogicModule super;pub dCtor(CustomLogicModule_EntityEditorPlatform);
	// These are editable creatures.
	// >= 1; on
	// 0; off
	// < 0; err
	priv int editableEntitiesOnPlatformCount = 0;
	
	priv IWorldEntity* playerEntityOnPlatform = nullptr;
	priv ArrayList<std::shared_ptr<IEventListener>> playerEntityOnPlatofrmEventsListener{};
	// If the player hauls an un-editable entity or hauls more than 1 entities.
	// This is used to grey the platform.
	priv bool isInvalidPlayerOnPlatform = false;

	// >= 1; on
	// 0; off
	// < 0; err
	priv int isEditorActiveCounter = 0;
	
	priv IPlatformModule* platform = nullptr;

	priv IDrawableNode3D* v_base = nullptr;
	priv IDrawableNode3D* v_base_disabled = nullptr;
	priv IDrawableNode3D* v_icon = nullptr;

	pub explicit CustomLogicModule_EntityEditorPlatform(
        IWorldEntity* entity,
        base::CustomLogicTemplate* t);
	prot void createBMain() override;

	priv bool getIsEditableEntitiesOnPlatform();
	priv void setIsEditableEntitiesOnPlatformIncremental(bool isEditableEntitiesOnPlatform);

	priv IWorldEntity* getPlayerEntityOnPlatform();
	priv void setPlayerEntityOnPlatform(IWorldEntity* playerEntityOnPlatform);

	priv bool getIsInvalidPlayerOnPlatform();
	priv void setIsInvalidPlayerOnPlatform(bool isInvalidPlayerOnPlatform);
	
	priv bool getIsEditorActive();
	priv void setIsEditorActiveIncremental(bool isEditorActive);

	priv void syncVisualsState();

	pub void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	pub void disconnectFromGamePartsPre() override;

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);
	priv void playerEntityOnPlatform_onEvent(IEventListener& eventListener, base::EntityEvent& event);
	priv void onGWEvent(IEventListener& eventListener, base::GWEvent& event);

    pub ~CustomLogicModule_EntityEditorPlatform() override;

};
};
