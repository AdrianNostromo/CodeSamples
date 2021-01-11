#pragma once

#include <base/gh.h>
#include "T_GWCapabilitiesRequirements.h"

namespace rpg3D {
class T_Operating : public T_GWCapabilitiesRequirements {priv typedef T_GWCapabilitiesRequirements super;pub dCtor(T_Operating);
	priv bool isOperating = false;
	// Must be -1 if tool is inactive.
	priv int operating_cursorIndex = -1;

	pub explicit T_Operating(ToolConfigBase* config);

	pub bool getIsOperating(int cursorIndex);
	pub bool getIsOperatingAny() final;
	pub int getOperating_cursorIndex();
	pub void setIsOperating(bool isOperating, int cursorIndex);
	prot virtual void onIsOperatingChanged(bool isOperating);

	prot void onEntityChangePre() override;

	// This is here so that movement, turn, entity_editor tools work.
	/// Those tools extend this and add their local cooldown check.
	//asd_1;// remove after test. also remove extenders.
	// This should be pure_virtual bu the new tools don't implement this. Local variant should never be called, only the extenders.
	pub virtual bool canTrigger() {
		throw LogicException(LOC);
	}

	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

    pub ~T_Operating() override;
};
};
