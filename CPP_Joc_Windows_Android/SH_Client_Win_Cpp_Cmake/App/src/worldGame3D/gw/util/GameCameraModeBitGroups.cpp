#include "GameCameraModeBitGroups.h"
#include <base/math/util/BoolUtil.h>

const int GameCameraModeBitGroups::followerFirstPersonTargetRotation = BoolUtil::setBitAtIndexDirect(0/*bitMap*/, 0/*bitIndex*/);
const int GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation = BoolUtil::setBitAtIndexDirect(0/*bitMap*/, 1/*bitIndex*/);
