#pragma once

#include <base/gh.h>
#include "T_Selected.h"

namespace worldGame3D {
	namespace gw {
		class CapabilityEvent;
	};
};

namespace rpg3D {
class T_GWCapabilitiesRequirements : public T_Selected {priv typedef T_Selected super;pub dCtor(T_GWCapabilitiesRequirements);
	// 255 not used.
	// This is init only from a constructor. Set it when used.
	priv unsigned char gwCapabilityRequirementBitIndex;

	// The iniital value is false because logic requires activation is required at start.
	priv bool isGWCapabilitiesExist = false;
	priv bool lastDispatched_isSelectedAndGWCapabilitiesExist = false;

	priv std::function<void(ITool* target, bool isSelectedAndGWCapabilitiesExist)> cb_isSelectedAndGWCapabilitiesExist_changed = nullptr;
	
	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

	pub explicit T_GWCapabilitiesRequirements(ToolConfigBase* config);

	pub void setCb_isSelectedAndGWCapabilitiesExist_changed(std::function<void(ITool* target, bool isSelectedAndGWCapabilitiesExist)> cb_isSelectedAndGWCapabilitiesExist_changed);

	prot void initGWCapabilityRequirementBitIndex(unsigned char gwCapabilityRequirementBitIndex);

	// This is made final here and onSelectedAndGWCapabilitiesEnabledStateChanged should be used.
	prot void _onSelectedStateChanged(bool isToolSelected, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) final;

	pub void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	pub void disconnectFromGamePartsPre() override;

	// Note. Must keep the gwGamePartsOptional param and check agains that because the local gwGamePartsOptional is still valid when disconnectFromGameParts is called.
	priv void handleActivationChange(bool isToolSelected, worldGame3D::IGameWorldGameParts* gwGamePartsOptional);

	priv void onGWCapabilityChanged(IEventListener& eventListener, worldGame3D::gw::CapabilityEvent& event);

	prot virtual void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional);

    pub ~T_GWCapabilitiesRequirements() override;
};
};
