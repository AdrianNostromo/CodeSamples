#include "MM_Stats.h"

using namespace mem;

MM_Stats::MM_Stats()
	: super()
{
	//void
}

MemoryStats* MM_Stats::getStats() {
	return &stats;
}

MM_Stats::~MM_Stats() {
	//void
}
