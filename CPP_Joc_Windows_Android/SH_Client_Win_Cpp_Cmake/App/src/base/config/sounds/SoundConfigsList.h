#pragma once

#include <base/audio/sound/util/CfgSound.h>

namespace base {
class SoundConfigsList {
private:
	static std::string NAMESPACE;

public:
	static bool IsStaticsInitHooked;

	static base::audio::CfgSound* UserInterface_Btn_Down;

private:
	static void InitStatics();

	static base::audio::CfgSound* GetNew_UserInterface_Btn_Down();

};
};
