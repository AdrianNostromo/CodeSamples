#pragma once

#include <base/gh.h>
#include <string>
#include <rpg3D/gw/entity/module/toolsHandler/tool/ITool.h>

class IUIView;

class ViewEvent {pub dCtor(ViewEvent);
	priv static int typeCounter;

	pub int type;

	pub std::string name;

	pub IUIView* view;

	// SnackbarView:button_pressed: DynamicTextButtonConfig* dynamicButtonConfig
	pub void* userData_voidP = nullptr;

    pub explicit ViewEvent(std::string name);

	pub void clearData();

	pub ViewEvent* setUserData_voidP(
		IUIView* view, void* userData_voidP);
	
	pub virtual ~ViewEvent();
};
