#pragma once

#include <base/gh.h>
#include "TEE_ToolActivation.h"
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>
#include <base/event/listener/IEventListener.h>

namespace worldGame3D {
	class IActionsModule;
};
namespace rpg3D {
	class IUserControlledModule;
};

namespace rpg3D {
class TEE_InteractionFilters : public TEE_ToolActivation {priv typedef TEE_ToolActivation super;pub dCtor(TEE_InteractionFilters);
    priv class ActionContinuousActivate : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionContinuousActivate);
		pub static int TYPE;

		pub int cursorIndex;
		pub Vector2 screenPos;

		pub explicit ActionContinuousActivate(
			int localEntityId, int syncedEntityId,
			int priority,
			int cursorIndex,
			Vector2& screenPos)
			: super(
				TYPE, "TEE_InteractionFilters::ActionContinuousActivate", 
				true/*isSyncedToServer*/, 0/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				0,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				0/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			),
			cursorIndex(cursorIndex),
			screenPos(screenPos)
		{ rawSubclassPointer = this; };

		pub ~ActionContinuousActivate() override {};
	};
	priv class ActionContinuousUpdateDrag : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionContinuousUpdateDrag);
		pub static int TYPE;

		pub int cursorIndex;
		pub Vector2 screenPos;
		pub Vector2 sensitisedScreenCursorMoveDelta;

		pub explicit ActionContinuousUpdateDrag(
			int localEntityId, int syncedEntityId,
			int priority,
			int cursorIndex,
			Vector2& screenPos, Vector2& sensitisedScreenCursorMoveDelta)
			: super(
				TYPE, "TEE_InteractionFilters::ActionContinuousUpdateDrag", 
				true/*isSyncedToServer*/, 0/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				0,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				0/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			),
			cursorIndex(cursorIndex),
			screenPos(screenPos), sensitisedScreenCursorMoveDelta(sensitisedScreenCursorMoveDelta)
		{
			rawSubclassPointer = this;
		};

		pub ~ActionContinuousUpdateDrag() override {};
	};
	priv class ActionContinuousDeactivate : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionContinuousDeactivate);
		pub static int TYPE;

		pub int cursorIndex;

		pub explicit ActionContinuousDeactivate(
			int localEntityId, int syncedEntityId,
			int priority,
			int cursorIndex)
			: super(
				TYPE, "TEE_InteractionFilters::ActionContinuousDeactivate", 
				true/*isSyncedToServer*/, 0/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				0,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				0/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			),
			cursorIndex(cursorIndex)
		{ rawSubclassPointer = this; };

		pub ~ActionContinuousDeactivate() override {};
	};
	
	priv class CachedActionInfo {pub dCtor(CachedActionInfo);
		//cursorIndex
		//screenPos
		pub static const int TYPE_ActivateContinuous;
		//cursorIndex
		//screenPos
		//sensitisedScreenCursorMoveDelta
		pub static const int TYPE_UpdateDrag;
		//cursorIndex
		pub static const int TYPE_DeactivateContinuous;

		pub int type;

		pub int cursorIndex;

		pub Vector2 screenPos;

		pub Vector2 sensitisedScreenCursorMoveDelta;

		pub explicit CachedActionInfo(int type, int cursorIndex)
			: type(type),
			cursorIndex(cursorIndex)
		{};
		pub explicit CachedActionInfo(int type, int cursorIndex, Vector2& screenPos)
			: type(type),
			cursorIndex(cursorIndex),
			screenPos(screenPos)
		{};
		pub explicit CachedActionInfo(int type, int cursorIndex, Vector2& screenPos, Vector2& sensitisedScreenCursorMoveDelta)
			: type(type),
			cursorIndex(cursorIndex),
			screenPos(screenPos), sensitisedScreenCursorMoveDelta(sensitisedScreenCursorMoveDelta)
		{};

		pub virtual ~CachedActionInfo() {};
	};

	priv worldGame3D::IActionsModule* actions = nullptr;

	priv ArrayList<CachedActionInfo> cachedActionInfosList{};

	priv ArrayList<std::shared_ptr<IEventListener>> autoListenersList{};

	pub explicit TEE_InteractionFilters(ToolConfigEntityEditor* config, std::shared_ptr<ExtraData> extraData,
		bool usesAutoToolModeActivationSound);

	pub void onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) override;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

	prot void onEntityChanged() override;
	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

	priv void addAction_ActionContinuousActivate(int cursorIndex, Vector2& screenPos);
	priv void addAction_ActionContinuousUpdateDrag(int cursorIndex, Vector2& screenCursorMoveDelta, Vector2& sensitisedScreenCursorMoveDelta);
	priv void addAction_ActionContinuousDeactivate(int cursorIndex);

	priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);

	priv void onEntityAction_ActionContinuousActivate(ActionContinuousActivate* action);
	priv void onEntityAction_ActionContinuousUpdateDrag(ActionContinuousUpdateDrag* action);
	priv void onEntityAction_ActionContinuousDeactivate(ActionContinuousDeactivate* action);

    pub ~TEE_InteractionFilters() override;
};
};
