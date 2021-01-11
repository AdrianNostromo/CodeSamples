#pragma once

#include <base/gh.h>
#include <rpg3D/gw/userEntitiesController/util/EntityControlType.h>
#include <graphics/environment/environment3D/Environment3D.h>
#include <worldGame3D/gw/main/util/IEntitiesFilter.h>
#include <worldGame3D/gw/event/GWEvent.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include "GPUEC_Base.h"

class IWorldEntity;
class IMovingEntityModule;
class IAppUtils;
namespace base {
	class IGameWorld;
};
namespace rpg3D {
	class IUserControlledModule;
	class WorldOrEntityInteractionEvent;
	class IToolsHandlerModule;
};

namespace rpg3D {
class GPUEC_ControlledEntity : public GPUEC_Base {priv typedef GPUEC_Base super;pub dCtor(GPUEC_ControlledEntity);
	prot class ControledTarget {pub dCtor(ControledTarget);
		pub IWorldEntity* entity = nullptr;
		pub IToolsHandlerModule* toolsHandler = nullptr;
		pub IUserControlledModule* userControlledModule = nullptr;
		pub IMovingEntityModule* movingEntityModule = nullptr;

		//asd_x;// Implement this in the future when the filters system is updated pub asd* interactionsDispatcher = nullptr;

		pub IListEntry* listEntry = nullptr;

		pub explicit ControledTarget(IWorldEntity* entity)
			: entity(entity)
		{}
	};

	pub static base::GWEvent* GWEvent_onEntityControlTypeChangePre;
	pub static base::GWEvent* GWEvent_onEntityControlTypeChanged;

	// In the future remove this var and use only the below list (That requires converting all cursor_and_other checks).
	prot ControledTarget* controledEntityTarget = nullptr;
	//asd_x;// Implement this in the future when the filters system is updated; // Hardcode the first entry to be the below global one.
	prot ListDL<ControledTarget> controledTargetsList{};
	prot bool isControlledEntityChangeLocked = false;
	// This is used to detect inputs for non-player targets (eg. the party member switcher uses a entity_down in this dispatcher).
	// asd_x;// Not all inputs are implemented, implement when needed). Currently only entity_down is implemented.
	prot base::TypedEventDispatcher1D<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>> globalInteractionsDispatcher{};
	
	// This is used by the tool filters to allow for a blocking mechanism.
	// This is incremented on events (eg: mouse, keyboard_key).
	prot int filterTriggerEventIndexCounter = 0;

	prot bool localCache_isCursorBlockedMode = false;

	// This must be None at start and change it using the setter function.
	priv int entityControlType = EntityControlType::None;

	// 1 : true;
	// <1 : false;
	priv int isWorldInteractorEnabledCounter = 1;

	pub explicit GPUEC_ControlledEntity(base::IGameWorld* gw);
	prot void createMain() override;

	pub bool getIsWorldInteractorEnabled();
	pub void setIsWorldInteractorEnabledIncremental(bool isWorldInteractorEnabledIncremental);

	pub int getEntityControlType() final;
	pub void setEntityControlType(int entityControlType) final;

	pub base::TypedEventDispatcher1D<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>& getGlobalInteractionsDispatcher() final;

	pub void setControlledEntity(IWorldEntity* controlledEntity);

	pub virtual void onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS);

    // This is called on game unpause.
	pub virtual void clearInput();

	prot static int dispatchInteraction_NotFiltered(
		std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent> entityInteraction,
		base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher,
		int validationType);

	prot static int dispatchInteraction_Filtered_Entity(
		IWorldEntity* selfEntity,
		std::shared_ptr<ArrayList<base::IGamePartMain::DT_Entity_Distance>> filterableEntitiesList,
		int interactionType, int triggerEventIndex, int cursorIndex,
		base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher);
	
	prot static void dispatchInteraction_Filtered_WorldPos(
		IWorldEntity* selfEntity,
		std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent> entityInteraction,
		base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher,
		int validationType);

	prot virtual void onControlledEntityChange_pre();
	prot virtual void onControlledEntityChanged();

	prot virtual void onEntityControlTypeChangePre();
	prot virtual void onEntityControlTypeChanged();

	prot virtual void deactivateActiveEntityControlType();
	prot virtual void activateActiveEntityControlType();

	priv void onIsWorldInteractorEnabledChanged();

	prot void disposeMain() override;
	pub ~GPUEC_ControlledEntity() override;
};
};
