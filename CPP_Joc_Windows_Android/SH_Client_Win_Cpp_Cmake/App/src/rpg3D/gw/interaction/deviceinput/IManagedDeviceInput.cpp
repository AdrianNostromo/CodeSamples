#include "IManagedDeviceInput.h"
#include <base/input/util/Touch.h>

using namespace rpg3D;

base::Touch::SensorType* IManagedDeviceInput::CustomSensorTypes::LocalisedScreenCursor = new base::Touch::SensorType(-1/*index*/, 0/*bitGroup*/);
base::Touch::SensorType* IManagedDeviceInput::CustomSensorTypes::DirectionalMovement = new base::Touch::SensorType(-2/*index*/, 0/*bitGroup*/);
base::Touch::SensorType* IManagedDeviceInput::CustomSensorTypes::SensitizedLockedCursor = new base::Touch::SensorType(-3/*index*/, 0/*bitGroup*/);
base::Touch::SensorType* IManagedDeviceInput::CustomSensorTypes::ToolExtraActivation = new base::Touch::SensorType(-4/*index*/, 0/*bitGroup*/);
