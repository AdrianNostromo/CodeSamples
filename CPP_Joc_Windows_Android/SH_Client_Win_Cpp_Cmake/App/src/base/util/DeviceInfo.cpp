#include "DeviceInfo.h"

const int DeviceInfo::Type::DESKTOP_LINUX = 1;
const int DeviceInfo::Type::DESKTOP_WINDOWS = 2;
const int DeviceInfo::Type::MOBILE_ANDROID = 3;

int DeviceInfo::deviceType = -1;
