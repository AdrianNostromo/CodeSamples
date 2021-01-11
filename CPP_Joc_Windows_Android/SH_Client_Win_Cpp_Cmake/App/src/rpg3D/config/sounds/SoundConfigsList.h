#pragma once

#include <base/config/sounds/SoundConfigsList.h>

namespace rpg3D {
class SoundConfigsList : public base::SoundConfigsList {
private: typedef base::SoundConfigsList super;
private:
	static std::string NAMESPACE;

public:
	static bool IsStaticsInitHooked;

	static base::audio::CfgSound* EntityEditor_ToolActivate_A_V1;
	static base::audio::CfgSound* ZoneEditor_ToolActivate_A_V1;

	static base::audio::CfgSound* StackablesHauler_Activate_A_V1;

	static base::audio::CfgSound* WoodBow_Fire_A_V1;
	static base::audio::CfgSound* Sword_Activate_A_V1;
	static base::audio::CfgSound* Sword_Impact_A_V1;
	static base::audio::CfgSound* Projectile_Arrow_Basic_Impact_A_V1;

private:
	static void InitStatics();

	static base::audio::CfgSound* GetNew_EntityEditor_ToolActivate_A_V1();
	static base::audio::CfgSound* GetNew_ZoneEditor_ToolActivate_A_V1();
	static base::audio::CfgSound* GetNew_StackablesHauler_Activate_A_V1();

	static base::audio::CfgSound* GetNew_WoodBow_Fire_A_V1();
	static base::audio::CfgSound* GetNew_Sword_Activate_A_V1();
	static base::audio::CfgSound* GetNew_Sword_Impact_A_V1();
	static base::audio::CfgSound* GetNew_Projectile_Arrow_Basic_Impact_A_V1();

};
};
