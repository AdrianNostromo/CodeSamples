#pragma once

#include <base/gh.h>

class BoolUtil {
    pub static inline bool isBitAtIndex(
      int bitMap, 
      int bitIndex
    );
    pub static inline bool isBitAtIndex(
      int bitMap, 
      int bitIndex0,
      int bitIndex1
    );
    pub static inline bool isBitAtIndex(
      int bitMap,
      int bitIndex0,
      int bitIndex1,
      int bitIndex2
    );
    
    pub static inline bool isBitsAll(
      int bitMap,
      int testBits
    );

    pub static inline int combine(
      int bitMap0, 
      int bitMap1
    );

    pub static inline int setBitAtIndexDirect(
      int bitMap, 
      int bitIndex
    );

    pub static inline void setBitAtIndexReference(
      int& bitMap, 
      int bitIndex
    );
    pub static inline int setBitAtIndexDirect(
      int bitMap, 
      int bitIndex0, 
      int bitIndex1
    );
    pub static inline void setBitAtIndexReference(
      int& bitMap, 
      int bitIndex0, 
      int bitIndex1
    );
    pub static inline int setBitAtIndexDirect(
      int bitMap, 
      int bitIndex0, 
      int bitIndex1, 
      int bitIndex2
    );
    pub static inline int setBitAtIndexDirect(
      int bitMap, 
      int bitIndex0, 
      int bitIndex1, 
      int bitIndex2,
      int bitIndex3
    );
    pub static inline void setBitAtIndexReference(
      int& bitMap, 
      int bitIndex0, 
      int bitIndex1, 
      int bitIndex2
    );

    pub static inline int clearBitAtIndexDirect(
        int bitMap,
        int bitIndex
    );
    pub static inline void clearBitAtIndexReference(
        int& bitMap,
        int bitIndex
    );

    pub static inline int toggleDirect(
        int bitMap,
        int bitIndex
    );

};

inline bool BoolUtil::isBitAtIndex(int bitMap, int bitIndex) {
    bool b = (bitMap & (1 << bitIndex)) != 0;
    return b;
}

inline bool BoolUtil::isBitAtIndex(
    int bitMap,
    int bitIndex0,
    int bitIndex1
) {
    bool ret = false;

    ret |= (bitMap & (1 << bitIndex0)) != 0;
    ret |= (bitMap & (1 << bitIndex1)) != 0;

    return ret;
}

inline bool BoolUtil::isBitAtIndex(
    int bitMap,
    int bitIndex0,
    int bitIndex1,
    int bitIndex2
) {
    bool ret = false;

    ret |= (bitMap & (1 << bitIndex0)) != 0;
    ret |= (bitMap & (1 << bitIndex1)) != 0;
    ret |= (bitMap & (1 << bitIndex2)) != 0;

    return ret;
}

inline bool BoolUtil::isBitsAll(
    int bitMap,
    int testBits)
{
    bool ret = (testBits & bitMap) == testBits ? true : false;

    return ret;
}

inline int BoolUtil::combine(
    int bitMap0,
    int bitMap1
) {
    return bitMap0 | bitMap1;
}

inline int BoolUtil::setBitAtIndexDirect(
    int bitMap,
    int bitIndex)
{
    return bitMap | (1 << bitIndex);
}

inline void BoolUtil::setBitAtIndexReference(
    int& bitMap,
    int bitIndex)
{
    bitMap = bitMap | (1 << bitIndex);
}

inline int BoolUtil::setBitAtIndexDirect(
    int bitMap,
    int bitIndex0,
    int bitIndex1)
{
    int ret = bitMap;

    ret = ret | (1 << bitIndex0);
    ret = ret | (1 << bitIndex1);

    return ret;
}

inline void BoolUtil::setBitAtIndexReference(
    int& bitMap,
    int bitIndex0,
    int bitIndex1)
{
    bitMap = bitMap | (1 << bitIndex0);
    bitMap = bitMap | (1 << bitIndex1);
}

inline int BoolUtil::setBitAtIndexDirect(
    int bitMap,
    int bitIndex0,
    int bitIndex1,
    int bitIndex2)
{
    int ret = bitMap;

    ret = ret | (1 << bitIndex0);
    ret = ret | (1 << bitIndex1);
    ret = ret | (1 << bitIndex2);

    return ret;
}

inline int BoolUtil::setBitAtIndexDirect(
    int bitMap,
    int bitIndex0,
    int bitIndex1,
    int bitIndex2,
    int bitIndex3)
{
    int ret = bitMap;

    ret = ret | (1 << bitIndex0);
    ret = ret | (1 << bitIndex1);
    ret = ret | (1 << bitIndex2);
    ret = ret | (1 << bitIndex3);

    return ret;
}

inline void BoolUtil::setBitAtIndexReference(
    int& bitMap,
    int bitIndex0,
    int bitIndex1,
    int bitIndex2)
{
    bitMap = bitMap | (1 << bitIndex0);
    bitMap = bitMap | (1 << bitIndex1);
    bitMap = bitMap | (1 << bitIndex2);
}

inline int BoolUtil::clearBitAtIndexDirect(
    int bitMap,
    int bitIndex)
{
    return bitMap & ~(1 << bitIndex);
}

inline void BoolUtil::clearBitAtIndexReference(
    int& bitMap,
    int bitIndex)
{
    bitMap = bitMap & ~(1 << bitIndex);
}

inline int BoolUtil::toggleDirect(
    int bitMap,
    int bitIndex)
{
    return bitMap ^ (1 << bitIndex);
}
