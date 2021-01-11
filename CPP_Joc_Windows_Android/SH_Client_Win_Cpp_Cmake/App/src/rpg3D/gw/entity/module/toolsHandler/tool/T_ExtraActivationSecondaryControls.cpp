#include "T_ExtraActivationSecondaryControls.h"
#include <base/exceptions/LogicException.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/event/ToolEvent.h>

using namespace rpg3D;

T_ExtraActivationSecondaryControls::T_ExtraActivationSecondaryControls(ToolConfigBase* config)
	: super(config)
{
	//void
}

TreeAVL<T_ExtraActivationSecondaryControls::ExtraActivationSecondaryControl>* T_ExtraActivationSecondaryControls::getExtraActivationSecondaryControlsList() {
	return extraActivationSecondaryControlsList;
}

T_ExtraActivationSecondaryControls::ExtraActivationSecondaryControl* T_ExtraActivationSecondaryControls::addExtraActivationSecondaryControl(
	std::string atlasId, std::string& regionId)
{
	if (extraActivationSecondaryControlsList == nullptr) {
		extraActivationSecondaryControlsList = new TreeAVL<ExtraActivationSecondaryControl>();
	}

	TreeAVL<ExtraActivationSecondaryControl>::Node* extraActivationControlNode = extraActivationSecondaryControlsList->insertEmplace(entryUidCounter++/*uid*/, atlasId, regionId);

	return &extraActivationControlNode->data;
}

void T_ExtraActivationSecondaryControls::setExtraActivationSecondaryControlIcon(
	ExtraActivationSecondaryControl* extraActivationSecondaryControl,
	std::string atlasId, std::string& regionId)
{
	if (extraActivationSecondaryControl == nullptr) {
		throw LogicException(LOC);
	}
	if (extraActivationSecondaryControl->atlasId == atlasId && extraActivationSecondaryControl->regionID == regionId) {
		// No change;
		return;
	}

	extraActivationSecondaryControl->atlasId = atlasId;
	extraActivationSecondaryControl->regionID = regionId;

	events.dispatchEvent(*ToolEvent_extraActivationSecondaryControl_onConfigChanged->setSecondaryControlConfigChanged(
		this,
		extraActivationSecondaryControl
	));
}

T_ExtraActivationSecondaryControls::~T_ExtraActivationSecondaryControls() {
	if (extraActivationSecondaryControlsList != nullptr) {
		delete extraActivationSecondaryControlsList;
		extraActivationSecondaryControlsList = nullptr;
	}
}
