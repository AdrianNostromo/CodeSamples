#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include <base/math/Bounds1DInt.h>

class CompositesBuffer final/*Note1001. calls_the_reserved_dispose*/ : public base::Object {priv typedef base::Object super;pub dCtor(CompositesBuffer);
    priv unsigned int _stride;

    // This uses table doubling logic.
    priv void* data = nullptr;

    priv int _count = 0;
    priv int _minCapacity = 0;
    priv int _capacity = 0;
    
    // This is used by mem managements that have a left(stable) mem region and a right(volatile) mem region with a center area that is unallocated.
    /// If this is used, on_load_to_gl will do 2 buffer_sub_data and not upload this center region.
    /// This expects the values to be set correctly by the mem manager that uses this buffer.
    // This is in entries, NOT_BYTES;
    priv Bounds1DInt unusedCenterRegion{};

    // This is used because v_ram resources allocation must be at the size of this buffer.
    priv bool isLockedSize = false;

    pub explicit CompositesBuffer(unsigned int _stride, int _minCapacity = 0, int initialCapacity=0);

    pub unsigned int stride();
    
    pub unsigned int count();
    // This is used when this buffer is memory managed.
    pub void setCount(int newCount);

    pub Bounds1DInt& getUnusedCenterRegionRef();

    pub unsigned int byteCount();
    pub unsigned int capacity();
    // Number of entries that can be inserted with the current count and capacity.
    pub unsigned int insertCapacity();
    pub unsigned int byteCapacity();

    pub bool getIsLockedSize();
    pub void lockSize();

    pub void* getData(unsigned int offsetEntriesCount=0, unsigned int validationAccessedEntriesCount=0);
    // This return char* for more convenient pointer math.
    pub char* getDataBlockAsCharP();

    pub void reserve(int reserveCapacity);
    pub void reserveInsertCapacity(int reserveInsertCapacity);

    pub void* appendUninitialised(int elementsCount=1);
    pub void appendUShort(unsigned short val);
    pub void append(void* srcData, int srcCount, unsigned int validationStride);
    pub void appendAsUShortWithOffset(void* srcData, int srcCount, unsigned int validationStride, unsigned short offset);

 //   pub void* get(int index);

 //   pub void* operator[](int index);
 //   
	//pub void* insert(int index);

	//pub void* append();

 //   pub bool remove(int index);

 //   pub void swap(int indexA, int indexB);

    pub void clear();

    prot void disposeMain() override;
    pub ~CompositesBuffer() override;
};
