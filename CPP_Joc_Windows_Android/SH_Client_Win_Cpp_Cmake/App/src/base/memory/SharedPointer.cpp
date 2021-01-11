#include "SharedPointer.h"

using namespace mem;

unsigned int SharedPointer_Stats::activesCount_spObjects = 0;
unsigned int SharedPointer_Stats::functionCallsCount_newSP = 0;
unsigned int SharedPointer_Stats::functionCallsCount_deleteSP = 0;
