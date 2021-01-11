#pragma once

#include <base/gh.h>
#include "TZE_ToolActivation.h"
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>
#include <base/event/listener/IEventListener.h>
#include <base/memory/SharedPointer.h>

namespace worldGame3D {
	class IActionsModule;
};
namespace rpg3D {
	class ToolActivationInfo_Base;
	class IUserControlledModule;
};

namespace rpg3D {
class TZE_InteractionFilters : public TZE_ToolActivation {priv typedef TZE_ToolActivation super;pub dCtor(TZE_InteractionFilters);
    priv class ActionContinuousActivate : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionContinuousActivate);
		pub static int TYPE;

		pub sp<ToolActivationInfo_Base> data;
		
		pub explicit ActionContinuousActivate(
			int localEntityId, int syncedEntityId,
			int priority,
			sp<ToolActivationInfo_Base> data)
			: super(
				TYPE, "TZE_InteractionFilters::ActionContinuousActivate", 
				true/*isSyncedToServer*/, 0/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				0,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				0/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			),
			data(data)
		{ rawSubclassPointer = this; };

		pub ~ActionContinuousActivate() override {};
	};
	priv class ActionContinuousUpdateDrag : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionContinuousUpdateDrag);
		pub static int TYPE;

		pub sp<ToolActivationInfo_Base> data;

		pub explicit ActionContinuousUpdateDrag(
			int localEntityId, int syncedEntityId,
			int priority,
			sp<ToolActivationInfo_Base> data)
			: super(
				TYPE, "TZE_InteractionFilters::ActionContinuousUpdateDrag", 
				true/*isSyncedToServer*/, 0/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				0,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				0/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			),
			data(data)
		{
			rawSubclassPointer = this;
		};

		pub ~ActionContinuousUpdateDrag() override {};
	};
	priv class ActionContinuousDeactivate : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionContinuousDeactivate);
		pub static int TYPE;

		pub sp<ToolActivationInfo_Base> data;

		pub explicit ActionContinuousDeactivate(
			int localEntityId, int syncedEntityId,
			int priority,
			sp<ToolActivationInfo_Base> data)
			: super(
				TYPE, "TZE_InteractionFilters::ActionContinuousDeactivate", 
				true/*isSyncedToServer*/, 0/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				0,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				0/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			),
			data(data)
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

		pub sp<ToolActivationInfo_Base> data = nullptr;

		pub explicit CachedActionInfo(int type, sp<ToolActivationInfo_Base> data)
			: type(type),
			data(data)
		{};

		pub virtual ~CachedActionInfo() {};
	};

	priv worldGame3D::IActionsModule* actions = nullptr;

	priv ArrayList<CachedActionInfo> cachedActionInfosList{};

	priv ArrayList<std::shared_ptr<IEventListener>> autoListenersList{};

	pub explicit TZE_InteractionFilters(ToolConfigZoneEditor* config, std::shared_ptr<ExtraData> extraData,
		bool usesAutoToolModeActivationSound);

	pub void onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) override;

	prot void onFilterInteraction_innactiveForActivation(sp<ToolActivationInfo_Base> touchData);
	prot void onFilterInteraction_activeContinuousForUpdate(sp<ToolActivationInfo_Base> touchData);
	prot void onFilterInteraction_activeContinuousForDeactivation(sp<ToolActivationInfo_Base> touchData);
	
	prot void onEntityChanged() override;
	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

	priv void addAction_ActionContinuousActivate(sp<ToolActivationInfo_Base> touchData);
	priv void addAction_ActionContinuousUpdateDrag(sp<ToolActivationInfo_Base> touchData);
	priv void addAction_ActionContinuousDeactivate(sp<ToolActivationInfo_Base> touchData);

	priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);

	priv void onEntityAction_ActionContinuousActivate(ActionContinuousActivate* action);
	priv void onEntityAction_ActionContinuousUpdateDrag(ActionContinuousUpdateDrag* action);
	priv void onEntityAction_ActionContinuousDeactivate(ActionContinuousDeactivate* action);

    pub ~TZE_InteractionFilters() override;
};
};
