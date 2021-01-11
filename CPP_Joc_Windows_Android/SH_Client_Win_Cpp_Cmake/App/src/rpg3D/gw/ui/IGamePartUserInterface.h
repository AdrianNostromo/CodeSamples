#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>
#include <rpg3D/gw/view/mainui/IMainInGameUIView.h>
#include <base/math/Vector3.h>

class IWorldEntity;
namespace base {
	class GWEvent;
}
namespace rpg3D {
	class MainInGameUIView;
};

namespace rpg3D {
class IGamePartUserInterface : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

	pub static base::GWEvent* GWEvent_onEntityEditor_activated;
	pub static base::GWEvent* GWEvent_onEntityEditor_deactivated;
	
	pub static base::GWEvent* GWEvent_onZoneEditor_activated;
	pub static base::GWEvent* GWEvent_onZoneEditor_deactivated;

	pub virtual void showViewIncremental() = 0;
	pub virtual void hideViewIncremental() = 0;

	pub virtual void setMainUiDPadListener(IMainInGameUIView::IListenerDPad* listenerDPad) = 0;
	pub virtual void setMainUiToolExtraActivationListener(IMainInGameUIView::IListenerToolExtraActivation* listenerToolExtraActivation) = 0;

	// editorMode : IEntityEditorView::EditorType::EDIT_OTHER;
	pub virtual bool getIsEntityEditorActive() = 0;
	pub virtual void startEntityEditor(int editorMode, bool isEditorStructureEnabled, Vector3* pos, IWorldEntity* existingEntityToEditOptional, IWorldEntity* existingEntityToEditOptionalB) = 0;
    
	pub virtual void startZoneEditor() = 0;
	pub virtual void stopZoneEditor(bool doAnimation) = 0;

	pub virtual void setNewMainUIViewUninitialised(MainInGameUIView* view) = 0;
	pub virtual IMainInGameUIView* getMainUIView() = 0;
	pub template <typename T>
		T getMainUiViewAs();

    ~IGamePartUserInterface() override = default;
};

template <typename T>
T IGamePartUserInterface::getMainUiViewAs() {
	T cVal = dynamic_cast<T>(getMainUIView());
	if (!cVal) {
		throw LogicException(LOC);
	}

	return cVal;
}

};
