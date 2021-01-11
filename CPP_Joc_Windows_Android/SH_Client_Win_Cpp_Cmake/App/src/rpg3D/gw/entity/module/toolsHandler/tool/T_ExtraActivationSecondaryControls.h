#pragma once

#include <base/gh.h>
#include "T_ExtraActivationListControls.h"

namespace rpg3D {
class T_ExtraActivationSecondaryControls : public T_ExtraActivationListControls {priv typedef T_ExtraActivationListControls super;pub dCtor(T_ExtraActivationSecondaryControls);
	priv int entryUidCounter = 0;
	prot TreeAVL<ExtraActivationSecondaryControl>* extraActivationSecondaryControlsList = nullptr;

	pub explicit T_ExtraActivationSecondaryControls(ToolConfigBase* config);

	pub TreeAVL<ExtraActivationSecondaryControl>* getExtraActivationSecondaryControlsList() final;
	prot ExtraActivationSecondaryControl* addExtraActivationSecondaryControl(
		std::string atlasId, std::string& regionId);

	prot void setExtraActivationSecondaryControlIcon(
		ExtraActivationSecondaryControl* extraActivationSecondaryControl,
		std::string atlasId, std::string& regionId);

    pub ~T_ExtraActivationSecondaryControls() override;
};
};
