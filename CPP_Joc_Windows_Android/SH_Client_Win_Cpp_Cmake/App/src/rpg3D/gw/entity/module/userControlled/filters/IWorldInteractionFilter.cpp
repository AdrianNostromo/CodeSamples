#include "IWorldInteractionFilter.h"
#include <base/math/util/BoolUtil.h>

using namespace rpg3D;

// Modify when more priorities are required.
// Values should start at 0 in case some insertion lists are used in the future.
const int IWorldInteractionFilter::InputPriorities::Normal = 0;
const int IWorldInteractionFilter::InputPriorities::Normal_Plus1 = 1;
