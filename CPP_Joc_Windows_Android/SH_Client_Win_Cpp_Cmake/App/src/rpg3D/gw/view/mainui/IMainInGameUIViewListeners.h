#pragma once

#include <base/gh.h>
#include <functional>

namespace rpg3D {
class IMainInGameUIViewListeners {
	pub class IListenerDPad {
		pub virtual void onSetMoveDirection(float screenDirX, float screenDirY) = 0;

		pub virtual ~IListenerDPad() = default;
	};
	pub class CbListenerDPad : virtual public IListenerDPad {
		pub std::function<void(float moveDirVectorX, float moveDirVectorY)> cb_onSetMoveDirection = nullptr;

		pub void onSetMoveDirection(float moveDirVectorX, float moveDirVectorY) override {
			if(cb_onSetMoveDirection) {
				cb_onSetMoveDirection(moveDirVectorX, moveDirVectorY);
			}
		}

		pub ~CbListenerDPad() override = default;
	};
	
	pub class IListenerToolExtraActivation {
		pub virtual void onToolExtraActivation_listControl_Down_MainBtn(int toolInstanceId, int sourceId) = 0;
		pub virtual void onToolExtraActivation_secondaryControl_Down_MainBtn(int toolInstanceId, int sourceId) = 0;

		pub virtual ~IListenerToolExtraActivation() = default;
	};
	pub class CbListenerToolExtraActivation : virtual public IListenerToolExtraActivation {
		pub std::function<void(int toolInstanceId, int sourceId)> cb_onToolExtraActivation_listControl_Down_MainBtn = nullptr;
		pub void onToolExtraActivation_listControl_Down_MainBtn(int toolInstanceId, int sourceId) override {
			if(cb_onToolExtraActivation_listControl_Down_MainBtn) {
				cb_onToolExtraActivation_listControl_Down_MainBtn(toolInstanceId, sourceId);
			}
		}

		pub std::function<void(int toolInstanceId, int sourceId)> cb_onToolExtraActivation_secondaryControl_Down_MainBtn = nullptr;
		pub void onToolExtraActivation_secondaryControl_Down_MainBtn(int toolInstanceId, int sourceId) override {
			if(cb_onToolExtraActivation_secondaryControl_Down_MainBtn) {
				cb_onToolExtraActivation_secondaryControl_Down_MainBtn(toolInstanceId, sourceId);
			}
		}

		pub ~CbListenerToolExtraActivation() override = default;
	};

	pub virtual ~IMainInGameUIViewListeners() = default;
};
};
