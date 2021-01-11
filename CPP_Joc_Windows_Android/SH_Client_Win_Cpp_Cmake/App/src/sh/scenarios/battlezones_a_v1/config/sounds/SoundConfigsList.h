#pragma once

#include <rpg3D/config/sounds/SoundConfigsList.h>

namespace battlezones_a_v1 {
class SoundConfigsList : public rpg3D::SoundConfigsList {
private: typedef rpg3D::SoundConfigsList super;
private:
	static std::string NAMESPACE;

public:
	static bool IsStaticsInitHooked;


private:
	static void InitStatics();


};
};
