#pragma once

#include <base/gh.h>
#include "T_Operating.h"
#include <base/container/TreeAVL.h>

namespace rpg3D {
class T_ExtraActivationListControls : public T_Operating {priv typedef T_Operating super;pub dCtor(T_ExtraActivationListControls);
	priv int entryUidCounter = 0;
	prot TreeAVL<ExtraActivationListControl>* extraActivationListControlsList = nullptr;
	priv ExtraActivationListControl* selectedExtraActivationListControl = nullptr;

	pub explicit T_ExtraActivationListControls(ToolConfigBase* config);

	pub TreeAVL<ExtraActivationListControl>* getExtraActivationListControlsList() final;
	prot ExtraActivationListControl* addExtraActivationListControl(
		std::string atlasId, std::string regionId,
		bool isSelected);

	prot void setExtraActivationListControlIsSelected(TreeAVL<ExtraActivationListControl>::Node* extraActivationControlNode, bool isSelected);

	prot inline ExtraActivationListControl* getSelectedExtraActivationListControl() {
		return selectedExtraActivationListControl;
	}

    pub ~T_ExtraActivationListControls() override;
};
};
