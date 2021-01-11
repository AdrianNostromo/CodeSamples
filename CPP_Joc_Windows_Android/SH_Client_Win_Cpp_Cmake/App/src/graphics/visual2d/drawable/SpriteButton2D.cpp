#include "SpriteButton2D.h"
#include <graphics/visual/TextureAtlas.h>
#include <base/assets/IAssetsManager.h>

using namespace base;

ArrayList<SpriteButton2D::State*> SpriteButton2D::State::StatesList{};

SpriteButton2D::State SpriteButton2D::State::Up{ 0/*index*/, "up"/*name*/, nullptr/*fallbackState*/ };
SpriteButton2D::State SpriteButton2D::State::Down{ 1/*index*/, "down"/*name*/, &SpriteButton2D::State::Up/*fallbackState*/ };
SpriteButton2D::State SpriteButton2D::State::Disabled{ 2/*index*/, "disabled"/*name*/, &SpriteButton2D::State::Up/*fallbackState*/ };

SpriteButton2D::State SpriteButton2D::State::Selected_Up{ 3/*index*/, "selected_up"/*name*/, &SpriteButton2D::State::Up/*fallbackState*/ };
SpriteButton2D::State SpriteButton2D::State::Selected_Down{ 4/*index*/, "selected_down"/*name*/, &SpriteButton2D::State::Selected_Up/*fallbackState*/ };
SpriteButton2D::State SpriteButton2D::State::Selected_Disabled{ 5/*index*/, "selected_disabled"/*name*/, &SpriteButton2D::State::Selected_Up/*fallbackState*/ };

SpriteButton2D::SpriteButton2D(
	base::audio::ISound* touchDownSound, float touchDownSound_volume,
	base::audio::ISound* touchUpSound, float touchUpSound_volume,
	base::audio::ISound* disabledTouchDownSound, float disabledTouchDownSound_volume,
	std::string& btnStatesTexAtlasID, std::string& btnStatesTexRegionPrefix,
	IAssetsManager* assetsManager)
	: super(
		touchDownSound, touchDownSound_volume,
		touchUpSound, touchUpSound_volume),
	disabledTouchDownSound(disabledTouchDownSound), disabledTouchDownSound_volume(disabledTouchDownSound_volume),
	btnStatesTexAtlasID(btnStatesTexAtlasID), btnStatesTexRegionPrefix(btnStatesTexRegionPrefix),
	assetsManager(assetsManager)
{
	setIsInputInteractive(true);
}

void SpriteButton2D::createMain() {
	super::createMain();

	updateStatesTexRegions();
}

void SpriteButton2D::setBtnStatesTexInfo(std::string& btnStatesTexAtlasID, std::string& btnStatesTexRegionPrefix) {
	if (this->btnStatesTexAtlasID == btnStatesTexAtlasID && this->btnStatesTexRegionPrefix == btnStatesTexRegionPrefix) {
		return ;
	}
	this->btnStatesTexAtlasID = btnStatesTexAtlasID;
	this->btnStatesTexRegionPrefix = btnStatesTexRegionPrefix;

	updateStatesTexRegions();

	updateActiveRegion();
}

void SpriteButton2D::updateStatesTexRegions() {
	stateVisualsArray.zeroSetData();

	if (btnStatesTexAtlasID.length() == 0) {
		// There is not visuals.
		return;
	}

	TextureAtlas* atlas = assetsManager->getTextureAtlas(btnStatesTexAtlasID);
	if (atlas == nullptr) {
		throw LogicException(LOC);
	}

	for (int i = 0; i < State::StatesList.count(); i++) {
		State* state = State::StatesList.getDirect(i);

		std::string regionId = btnStatesTexRegionPrefix + state->name;

		TextureRegion* atlasRegion = atlas->getRegion(regionId);
		if (atlasRegion == nullptr) {
			if (state == &State::Up) {
				if (state->fallbackState != nullptr) {
					// The Up state can't have any fallback values.
					throw LogicException(LOC);
				}

				// If no _Up state exists, try to fetch one that has the prefix with no suffix.
				atlasRegion = atlas->getRegion(btnStatesTexRegionPrefix);
				if (atlasRegion == nullptr) {
					throw LogicException(LOC);
				}
			} else {
				if (state->fallbackState == nullptr) {
					throw LogicException(LOC);
				}
				// Try to get a up state that is the actual prefix only.
				atlasRegion = stateVisualsArray.getDirect(state->fallbackState->index);
				if (atlasRegion == nullptr) {
					throw LogicException(LOC);
				}
			}
		}

		if (atlasRegion == nullptr) {
			throw LogicException(LOC);
		}
		stateVisualsArray.setDirect(state->index, atlasRegion);
	}
}

void SpriteButton2D::setIsCursorButtonPressed(int cursorIndex, base::Touch::ButtonCode* buttonId, bool isPressed) {
	super::setIsCursorButtonPressed(cursorIndex, buttonId, isPressed);

	if (buttonId == base::Touch::ButtonCode::LeftOrTouch) {
		updateActiveRegion();
	}
}

bool SpriteButton2D::getSelected() {
	return selected;
}

void SpriteButton2D::setSelected(bool selected) {
	if (this->selected == selected) {
		return;
	}

	this->selected = selected;

	updateActiveRegion();
}

bool SpriteButton2D::getEnabled() {
	return enabled;
}

void SpriteButton2D::setEnabled(bool enabled) {
	if (this->enabled == enabled) {
		return;
	}

	if (getIsEventsEnabled() != this->enabled) {
		// Using the current system, "isEventsEnabled" must be set only here and so must be the same as "enabled".
		// If a different behavious is required, use a isEventsEnabled_b sub variable. I think "isEventsEnabled" should not be a int version.
		throw LogicException(LOC);
	}

	this->enabled = enabled;

	setIsEventsEnabled(this->enabled);

	updateActiveRegion();
}

void SpriteButton2D::onIsEventsEnabledChanged() {
	super::onIsEventsEnabledChanged();

	updateActiveRegion();
}

void SpriteButton2D::onParentChanged() {
	super::onParentChanged();

	if (getParent() != nullptr) {
		updateActiveRegion();
	}
}

void SpriteButton2D::updateActiveRegion() {
	State* targetState = nullptr;

	if (!selected) {
		if (!enabled) {
			targetState = &State::Disabled;
		} else {
			if (!getIsButtonPressed(base::Touch::ButtonCode::LeftOrTouch)) {
				targetState = &State::Up;
			} else {
				targetState = &State::Down;
			}
		}
	} else {
		if (!enabled) {
			targetState = &State::Selected_Disabled;
		} else {
			if (!getIsButtonPressed(base::Touch::ButtonCode::LeftOrTouch)) {
				targetState = &State::Selected_Up;
			} else {
				targetState = &State::Selected_Down;
			}
		}
	}

	if (targetState == nullptr) {
		throw LogicException(LOC);
	}

	TextureRegion* activeStateRegion = stateVisualsArray.getDirect(targetState->index);
	// Note. The texture can be nullptr. This occurs when the btn has no texture set.

	setTextureRegion(activeStateRegion);
}

void SpriteButton2D::playTouchDownSound() {
	if (getEnabled()) {
		super::playTouchDownSound();
	} else {
		if (disabledTouchDownSound != nullptr) {
			disabledTouchDownSound->play(disabledTouchDownSound_volume);
		}
	}
}

void SpriteButton2D::playTouchUpSound() {
	if (getEnabled()) {
		super::playTouchUpSound();
	}
}

SpriteButton2D::~SpriteButton2D() {
	//void
}
