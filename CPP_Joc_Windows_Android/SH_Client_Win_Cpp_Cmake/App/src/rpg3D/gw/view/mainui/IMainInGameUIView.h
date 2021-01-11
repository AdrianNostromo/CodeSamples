#pragma once

#include <base/gh.h>
#include <functional>
#include "IMainInGameUIViewListeners.h"

namespace rpg3D {
class IMainInGameUIView : public virtual IMainInGameUIViewListeners {
	prot static int GetNewID();
	pub static int ID;

	pub virtual void setListenerDPad(IListenerDPad* listenerDPad) = 0;
	pub virtual void setListenerToolExtraActivation(IListenerToolExtraActivation* listenerToolExtraActivation) = 0;

	pub virtual bool getIsCrosshair() = 0;
	pub virtual void setIsCrosshair(bool isCrosshair) = 0;
	
	pub virtual bool getIsEditorModeActive() = 0;
	pub virtual void setIsEditorModeActive(bool isEditorModeActive) = 0;

	pub virtual ~IMainInGameUIView() = default;
};
};
