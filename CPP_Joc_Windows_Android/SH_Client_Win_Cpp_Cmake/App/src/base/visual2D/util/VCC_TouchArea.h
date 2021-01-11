#pragma once

#include "VCC_Touchable.h"
#include <string>

class VCC_TouchArea : public VCC_Touchable {priv typedef VCC_Touchable super;pub dCtor(VCC_TouchArea);
	pub static int TYPE;

	pub std::string* touchDownSoundConfig_id;
    pub float touchDownSound_volume;
    pub int touchDown_hapticFeedback_type;

    pub std::string* touchUpSoundConfig_id;
    pub float touchUpSound_volume;

    pub explicit VCC_TouchArea(
    	std::string* touchDownSoundConfig_id, float touchDownSound_volume, int touchDown_hapticFeedback_type,
    	std::string* touchUpSoundConfig_id, float touchUpSound_volume);
    pub explicit VCC_TouchArea(
    	std::string* touchDownSoundConfig_id, float touchDownSound_volume, int touchDown_hapticFeedback_type,
    	std::string* touchUpSoundConfig_id, float touchUpSound_volume,
    	int touchShape);

    pub static VCC_TouchArea* New(
    	std::string* touchDownSoundConfig_id, float touchDownSound_volume, int touchDown_hapticFeedback_type,
    	std::string* touchUpSoundConfig_id, float touchUpSound_volume,
    	int touchShape);
    pub static VCC_TouchArea* New(
		std::string* touchDownSoundConfig_id, float touchDownSound_volume, int touchDown_hapticFeedback_type,
		std::string* touchUpSoundConfig_id, float touchUpSound_volume);
    pub static VCC_TouchArea* New(
		int touchShape);
    pub static VCC_TouchArea* New();

    pub virtual ~VCC_TouchArea();

};
