#include "NetDataLimits.h"

using namespace base::Net;

const int NetDataLimits::BLOCK_LEN_MIN = 16;
const int NetDataLimits::BLOCK_LEN_MAX = 1024 * 256;

const int NetDataLimits::MSG_LEN_MIN = 8;
const int NetDataLimits::MSG_LEN_MAX = 1024 * 256;
