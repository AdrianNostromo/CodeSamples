#pragma once

#include <base/gh.h>
#include <rpg3D/gw/view/mainui/IMainInGameUIView.h>

namespace startarea_a_v1 {
class IMainInGameUIView : virtual public rpg3D::IMainInGameUIView {
	pub static int ID;

    pub virtual ~IMainInGameUIView() = default;
};
};
