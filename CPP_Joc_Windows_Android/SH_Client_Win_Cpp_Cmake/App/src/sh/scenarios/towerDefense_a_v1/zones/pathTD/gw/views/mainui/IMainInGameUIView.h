#pragma once

#include <base/gh.h>
#include <rpg3D/gw/view/mainui/IMainInGameUIView.h>
#include <functional>

namespace towerDefense_a_v1 {
namespace PathTD {
class IMainInGameUIView : virtual public rpg3D::IMainInGameUIView {
	pub static int ID;
	
	pub virtual void setCurrentLives(int currentLives) = 0;
	pub virtual void setCurrentGold(int currentGold) = 0;
	pub virtual void setIsWaveStarterBtnActive(bool isWaveStarterBtnActive) = 0;

	pub virtual void setCb_onStartNextWaveRequest(std::function<void(
		IMainInGameUIView* target)> cb_onStartNextWaveRequest) = 0;

    pub virtual ~IMainInGameUIView() = default;
};
};
};
