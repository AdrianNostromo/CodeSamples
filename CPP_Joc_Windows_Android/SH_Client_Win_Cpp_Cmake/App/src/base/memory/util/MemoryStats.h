#pragma once

namespace mem {
class MemoryStats {
    // This is actual ram ussage.
    pub unsigned int ram_bytes_count = 0;
    // This is allocations made by the memory manager for app variables and stuff.
    pub unsigned int activeAllocations_bytes_count = 0;

    // These are active allocations managed by:
    // malloc, realoc, free
    pub unsigned int activesCount_voidPointers = 0;
    // new, delete
    pub unsigned int activesCount_objects = 0;
    // SharedPointer<>
    // This var is located in SharedPointer_Stats::activesCount_spObjects;
    // pub unsigned int activesCount_spObjects = 0;

    // These counters are reset at 1 billion.
    // App logic may also reset these.
    // These 3 don't include memory used by objects or spObjects;
    pub unsigned int functionCallsCount_malloc = 0;
    pub unsigned int functionCallsCount_realloc = 0;
    pub unsigned int functionCallsCount_free = 0;
    //
    pub unsigned int functionCallsCount_new = 0;
    pub unsigned int functionCallsCount_delete = 0;

    // These 3 don't include memory used by objects or spObjects;
    pub unsigned int cumulativeByteChanges_malloc = 0;
    //asd_x;// There is no information until the custom mManager is implemented
    //pub unsigned int cumulativeByteChanges_realoc = 0;
    //asd_x;// There is no information until the custom mManager is implemented
    //pub unsigned int cumulativeByteChanges_free = 0;
    //
    pub unsigned int cumulativeByteChanges_new = 0;
    pub unsigned int cumulativeByteChanges_delete = 0;
};
};
