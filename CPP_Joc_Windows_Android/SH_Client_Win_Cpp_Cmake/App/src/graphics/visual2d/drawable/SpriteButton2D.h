#pragma once

#include <base/gh.h>
#include "ReactiveInteractiveSprite2D.h"
#include <graphics/visual/TextureRegion.h>
#include <memory>
#include <base/list/ArrayList.h>
#include <base/list/Array1D.h>

class IAssetsManager;

namespace base {
class SpriteButton2D : public ReactiveInteractiveSprite2D {priv typedef ReactiveInteractiveSprite2D super;pub dCtor(SpriteButton2D);
	priv class State {
		pub static ArrayList<State*> StatesList;

		pub static State Up;
		pub static State Down;
		pub static State Disabled;

		pub static State Selected_Up;
		pub static State Selected_Down;
		pub static State Selected_Disabled;

		pub int index;
		pub std::string name;

		pub State* fallbackState;

		pub explicit State(int index, std::string name, State* fallbackState)
			: index(index), name(name), fallbackState(fallbackState)
		{
			StatesList.appendDirect(this);
		}
	};

	priv base::audio::ISound* disabledTouchDownSound;
	priv float disabledTouchDownSound_volume;

	priv std::string btnStatesTexAtlasID;
	priv std::string btnStatesTexRegionPrefix;

	priv IAssetsManager* assetsManager;

	priv Array1D<TextureRegion*> stateVisualsArray{ State::StatesList.count() };

	priv bool selected = false;

	priv bool enabled = true;

	pub explicit SpriteButton2D(
		base::audio::ISound* touchDownSound, float touchDownSound_volume,
		base::audio::ISound* touchUpSound, float touchUpSound_volume,
		base::audio::ISound* disabledTouchDownSound, float disabledTouchDownSound_volume,
		std::string& btnStatesTexAtlasID, std::string& btnStatesTexRegionPrefix,
		IAssetsManager* assetsManager);
	prot void createMain() override;

	pub void setBtnStatesTexInfo(std::string& btnStatesTexAtlasID, std::string& btnStatesTexRegionPrefix);
	priv void updateStatesTexRegions();

	pub void setIsCursorButtonPressed(int cursorIndex, base::Touch::ButtonCode* buttonId, bool isPressed) override;

	pub bool getSelected();
	pub void setSelected(bool selected);
	
	pub bool getEnabled();
	pub void setEnabled(bool enabled);

	prot void onIsEventsEnabledChanged() override;

	prot void onParentChanged() override;

	prot void playTouchDownSound() override;
	prot void playTouchUpSound() override;

	priv void updateActiveRegion();

	pub ~SpriteButton2D() override;
};
};
