#pragma once

#include <rpg3D/config/sounds/SoundConfigsList.h>

namespace towerDefense_a_v1 {
class SoundConfigsList : public rpg3D::SoundConfigsList {
private: typedef rpg3D::SoundConfigsList super;
public:
	static bool IsStaticsInitHooked;

    static base::audio::CfgSound* CannonTurretFire_A_V1;
    static base::audio::CfgSound* CannonProjectileImpact_A_V1;

private:
	static void InitStatics();

	static base::audio::CfgSound* GetNew_CannonTurretFire_A_V1();
	static base::audio::CfgSound* GetNew_CannonProjectileImpact_A_V1();

};
};
