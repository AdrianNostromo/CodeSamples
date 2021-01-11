#include <base/sensor/hapticFeedback/util/HapticFeedbackType.h>
#include "VCC_TouchArea.h"

int VCC_TouchArea::TYPE = super::NewType();

VCC_TouchArea::VCC_TouchArea(
    std::string* touchDownSoundConfig_id, float touchDownSound_volume, int touchDown_hapticFeedback_type,
    std::string* touchUpSoundConfig_id, float touchUpSound_volume)
    : super(TYPE),
      touchDownSoundConfig_id(touchDownSoundConfig_id), touchDownSound_volume(touchDownSound_volume), touchDown_hapticFeedback_type(touchDown_hapticFeedback_type),
      touchUpSoundConfig_id(touchUpSoundConfig_id), touchUpSound_volume(touchUpSound_volume)
{
    //void
}

VCC_TouchArea::VCC_TouchArea(
    std::string* touchDownSoundConfig_id, float touchDownSound_volume, int touchDown_hapticFeedback_type,
    std::string* touchUpSoundConfig_id, float touchUpSound_volume,
    int touchShape)
    : super(TYPE, touchShape),
      touchDownSoundConfig_id(touchDownSoundConfig_id), touchDownSound_volume(touchDownSound_volume), touchDown_hapticFeedback_type(touchDown_hapticFeedback_type),
      touchUpSoundConfig_id(touchUpSoundConfig_id), touchUpSound_volume(touchUpSound_volume)
{
    //void
}

VCC_TouchArea* VCC_TouchArea::New(
    std::string* touchDownSoundConfig_id, float touchDownSound_volume, int touchDown_hapticFeedback_type,
    std::string* touchUpSoundConfig_id, float touchUpSound_volume,
    int touchShape)
{
    VCC_TouchArea* ret = new VCC_TouchArea(
        touchDownSoundConfig_id, touchDownSound_volume, touchDown_hapticFeedback_type,
        touchUpSoundConfig_id, touchUpSound_volume,
        touchShape);

    return ret;
}

VCC_TouchArea* VCC_TouchArea::New(
    int touchShape)
{
    VCC_TouchArea* ret = new VCC_TouchArea(
        nullptr, 1.0f, base::HapticFeedbackType::NONE,
        nullptr, 1.0f,
        touchShape);

    return ret;
}

VCC_TouchArea* VCC_TouchArea::New(
    std::string* touchDownSoundConfig_id, float touchDownSound_volume, int touchDown_hapticFeedback_type,
    std::string* touchUpSoundConfig_id, float touchUpSound_volume)
{
    VCC_TouchArea* ret = new VCC_TouchArea(
        touchDownSoundConfig_id, touchDownSound_volume, touchDown_hapticFeedback_type,
        touchUpSoundConfig_id, touchUpSound_volume);

    return ret;
}

VCC_TouchArea* VCC_TouchArea::New() {
    return New(nullptr, 0.5f, base::HapticFeedbackType::NONE, nullptr, 0.5f);
}

VCC_TouchArea::~VCC_TouchArea() {
    //void
}
