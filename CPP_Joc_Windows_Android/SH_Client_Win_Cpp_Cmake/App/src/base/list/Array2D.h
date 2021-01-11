#pragma once

#include <base/gh.h>
#include <base/math/Math.h>
#include <base/exceptions/LogicException.h>
#include <cstring>
#include <base/exceptions/OutOfRangeException.h>
#include <base/math/Vector2Int.h>
#include <base/math/Bounds2DInt.h>
#include <base/MM.h>

template <typename T>
class Array2D {pub dCtor(Array2D);
	pub Vector2Int count{};
    
    priv T* data = nullptr;

    pub explicit Array2D();
    pub explicit Array2D(int _lengthA, int _lengthB);
    pub explicit Array2D(int _lengthA, int _lengthB, T* data);
    pub explicit Array2D(int _lengthA, int _lengthB, bool invertA, bool invertB, T* data);

    pub void zeroSetData();
    pub void memSetData(int val);

    pub T* getData();

    pub inline int getLengthA();
    pub inline int getLengthB();

    pub void resizeNoElementsMove(int _lengthA, int _lengthB, bool _invertA=false, bool _invertB=false, T* data=nullptr);
    pub inline T* getPointer(int gA, int gB);
    pub inline T* getPointer(int gA, int gB, T* fallbackValue);
    pub inline T getDirect(int gA, int gB);
    pub inline T getDirect(int gA, int gB, T fallbackValue);
    pub inline T& getReference(int gA, int gB);
    pub inline T& getReference(Vector2Int const& gPos);
    
    pub inline void set(int gA, int gB, T& val);
    pub inline void set(Array2D<T>& src);
    pub inline void setDirect(int gA, int gB, T val);
    pub void set(Vector2Int& offset, Array2D<T>* src, Vector2Int& srcOffset, Vector2Int& copyCount);

    pub static inline void copy(
        T* srcData, Vector2Int& srcCount, Vector2Int& srcOffset,
        T* destData, Vector2Int& destCount, Vector2Int& destOffset,
        Vector2Int& count);
    
    pub static inline void move(
        T* srcData, Vector2Int& srcCount, Vector2Int& srcOffset,
        T* destData, Vector2Int& destCount, Vector2Int& destOffset,
        Vector2Int& count);

    pub void setRegionDirect(Bounds2DInt* bounds, T val);

    pub void invertA();
    pub void invertB();

    pub void swap(
        Vector2Int& src,
        Vector2Int& dest
    );
    pub void swap(
        int src_gA, int src_gB,
        int dest_gA, int dest_gB
    );

    ~Array2D();
private:
    inline int gridPosToDataIndex(int gA, int gB);

};

template <typename T>
Array2D<T>::Array2D()
        : Array2D<T>(0/*_lengthA*/, 0/*_lengthB*/, nullptr/*data*/)
{
    //void
}

template <typename T>
Array2D<T>::Array2D(int _lengthA, int _lengthB)
        : Array2D<T>(_lengthA, _lengthB, new T[_lengthA * _lengthB]())
{
    //void
}

template <typename T>
Array2D<T>::Array2D(int _lengthA, int _lengthB, T* data)
        : Array2D<T>(_lengthA, _lengthB, false, false, data)
{
    //void
}

template <typename T>
Array2D<T>::Array2D(int _lengthA, int _lengthB, bool _invertA, bool _invertB, T* data)
        : count(_lengthA, _lengthB), data(data)
{
    if(_invertA) {
        invertA();
    }
    if(_invertB) {
        invertB();
    }
}

template <typename T>
void Array2D<T>::zeroSetData() {
	memset(data, 0, count.x * count.y * sizeof(T));
}

template <typename T>
void Array2D<T>::memSetData(int val) {
	memset(data, val, count.x * count.y * sizeof(T));
}

template <typename T>
T* Array2D<T>::getData() {
    return data;
}

template <typename T>
inline int Array2D<T>::getLengthA() {
    return count.x;
}

template <typename T>
inline int Array2D<T>::getLengthB() {
    return count.y;
}

template <typename T>
inline T* Array2D<T>::getPointer(int gA, int gB) {
    if(gA < 0 || gA >= count.x || gB < 0 || gB >= count.y) {
        throw LogicException(LOC);
    }

    int index = gridPosToDataIndex(gA, gB);

    return &data[index];
}

template <typename T>
inline T* Array2D<T>::getPointer(int gA, int gB, T* fallbackValue) {
    if(gA < 0 || gA >= count.x || gB < 0 || gB >= count.y) {
        return fallbackValue;
    }

    int index = gridPosToDataIndex(gA, gB);

    return &data[index];
}

template <typename T>
inline T Array2D<T>::getDirect(int gA, int gB) {
    if(gA < 0 || gA >= count.x || gB < 0 || gB >= count.y) {
        throw LogicException(LOC);
    }

    int index = gridPosToDataIndex(gA, gB);

    return data[index];
}

template <typename T>
inline T Array2D<T>::getDirect(int gA, int gB, T fallbackValue) {
    if(gA < 0 || gA >= count.x || gB < 0 || gB >= count.y) {
        return fallbackValue;
    }

    int index = gridPosToDataIndex(gA, gB);

    return data[index];
}

template <typename T>
inline T& Array2D<T>::getReference(int gA, int gB) {
    if(gA < 0 || gA >= count.x || gB < 0 || gB >= count.y) {
        throw LogicException(LOC);
    }

    int index = gridPosToDataIndex(gA, gB);

    return data[index];
}

template <typename T>
inline T& Array2D<T>::getReference(Vector2Int const& gPos) {
    if(gPos.x < 0 || gPos.x >= count.x || gPos.y < 0 || gPos.y >= count.y) {
        throw LogicException(LOC);
    }

    int index = gridPosToDataIndex(gPos.x, gPos.y);

    return data[index];
}

template <typename T>
void Array2D<T>::resizeNoElementsMove(int _lengthA, int _lengthB, bool _invertA, bool _invertB, T* _data) {
    if(data) {
        delete[] data;
        data = nullptr;

        this->count.set(0, 0);
    }

    if(_lengthA > 0 && _lengthB > 0) {
        this->count.set(_lengthA, _lengthB);

        if (_data != nullptr) {
            this->data = _data;

            if (_invertA) {
                invertA();
            }
            if (_invertB) {
                invertB();
            }
        } else {
            this->data = new T[_lengthA * _lengthB]();
        }
    } else {
        if (_data != nullptr) {
            // There shouldn't be any data provided if a size component is 0.
            throw LogicException(LOC);
        }
    }
}

template <typename T>
inline void Array2D<T>::set(int gA, int gB, T& val) {
    if(gA < 0 || gA >= count.x || gB < 0 || gB >= count.y) {
        throw OutOfRangeException();
    }

    int index = gridPosToDataIndex(gA, gB);

    data[index] = val;
}

template <typename T>
inline void Array2D<T>::set(Array2D<T>& src) {
    if(count.x != src.count.x || count.y != src.count.y) {
        resizeNoElementsMove(src.count.x, src.count.y);
    }

    // Copy all data using memcpy.
    memcpy(data, src.data, sizeof(T) * count.x * count.y);
}

template <typename T>
inline void Array2D<T>::setDirect(int gA, int gB, T val) {
    if (gA < 0 || gA >= count.x || gB < 0 || gB >= count.y) {
        throw OutOfRangeException();
    }

    int index = gridPosToDataIndex(gA, gB);

    data[index] = std::move(val);
}

template <typename T>
void Array2D<T>::set(Vector2Int& offset, Array2D<T>* src, Vector2Int& srcOffset, Vector2Int& copyCount) {
    // Hard check this and src bounds.
    copy(
        src->data, src->count, srcOffset,
        data, count, offset,
        copyCount
    );
}

template <typename T>
inline void Array2D<T>::copy(
    T* srcData, Vector2Int& srcCount, Vector2Int& srcOffset,
    T* destData, Vector2Int& destCount, Vector2Int& destOffset,
    Vector2Int& copyCount)
{
    if (copyCount.x < 0 || copyCount.y < 0) {
        throw LogicException(LOC);
    }
    if (srcOffset.x < 0 || srcOffset.y < 0
        || srcOffset.x + copyCount.x > srcCount.x || srcOffset.y + copyCount.y > srcCount.y)
    {
        throw LogicException(LOC);
    }
    if (destOffset.x < 0 || destOffset.y < 0
        || destOffset.x + copyCount.x > destCount.x || destOffset.y + copyCount.y > destCount.y)
    {
        throw LogicException(LOC);
    }

    for (int gA = 0; gA < copyCount.x; gA++) {
        for (int gB = 0; gB < copyCount.y; gB++) {
            int srcG_a = srcOffset.x + gA;
            int srcG_b = srcOffset.y + gB;

            int destG_a = destOffset.x + gA;
            int destG_b = destOffset.y + gB;

            int srcIndex = srcG_b * srcCount.x + srcG_a;
            int destIndex = destG_b * destCount.x + destG_a;

            // Use thge copy operator, don't do move.
            destData[destIndex] = srcData[srcIndex];
        }
    }
}

template <typename T>
inline void Array2D<T>::move(
    T* srcData, Vector2Int& srcCount, Vector2Int& srcOffset,
    T* destData, Vector2Int& destCount, Vector2Int& destOffset,
    Vector2Int& copyCount)
{
    if (copyCount.x < 0 || copyCount.y < 0) {
        throw LogicException(LOC);
    }
    if (srcOffset.x < 0 || srcOffset.y < 0
        || srcOffset.x + copyCount.x > srcCount.x || srcOffset.y + copyCount.y > srcCount.y)
    {
        throw LogicException(LOC);
    }
    if (destOffset.x < 0 || destOffset.y < 0
        || destOffset.x + copyCount.x > destCount.x || destOffset.y + copyCount.y > destCount.y)
    {
        throw LogicException(LOC);
    }

    for (int gA = 0; gA < copyCount.x; gA++) {
        for (int gB = 0; gB < copyCount.y; gB++) {
            int srcG_a = srcOffset.x + gA;
            int srcG_b = srcOffset.y + gB;

            int destG_a = destOffset.x + gA;
            int destG_b = destOffset.y + gB;

            int srcIndex = srcG_b * srcCount.x + srcG_a;
            int destIndex = destG_b * destCount.x + destG_a;

            destData[destIndex] = std::move(srcData[srcIndex]);
        }
    }
}

template <typename T>
void Array2D<T>::setRegionDirect(Bounds2DInt* bounds, T val) {
    for (int x = bounds->min.x; x < bounds->max.x; x++) {
        for (int y = bounds->min.y; y < bounds->max.y; y++) {
            int index = gridPosToDataIndex(x, y);

            data[index] = val;
        }
    }
}

template <typename T>
void Array2D<T>::invertA() {
    int gHalfA = (int) Math::floor(count.x / 2.0f);

    int indexSrc;
    int indexDst;
    // Enumerate half-ish of the grid and invert.
    for(int a=0;a<gHalfA;a++) {
        for(int b=0;b<count.y;b++) {
            indexSrc = gridPosToDataIndex(a, b);
            indexDst = gridPosToDataIndex((count.x - 1) - a, b);

            T t{ std::move(data[indexSrc]) };
            data[indexSrc] = std::move(data[indexDst]);
            data[indexDst] = std::move(t);
        }
    }
}

template <typename T>
void Array2D<T>::invertB() {
    int gHalfB = (int) Math::floor(count.y / 2.0f);

    int indexSrc;
    int indexDst;
    // Enumerate half-ish of the grid and invert.
    for(int a=0;a<count.x;a++) {
        for(int b=0;b<gHalfB;b++) {
            indexSrc = gridPosToDataIndex(a, b);
            indexDst = gridPosToDataIndex(a, (count.y - 1) - b);

            T t{ std::move(data[indexSrc]) };
            data[indexSrc] = std::move(data[indexDst]);
            data[indexDst] = std::move(t);
        }
    }
}

template <typename T>
void Array2D<T>::swap(
    Vector2Int& src,
    Vector2Int& dest)
{
    return swap(
        src.x, src.y,
        dest.x, dest.y
    );
}

template <typename T>
void Array2D<T>::swap(
    int src_gA, int src_gB,
    int dest_gA, int dest_gB)
{
    if ((src_gA < 0 || src_gA >= count.x || src_gB < 0 || src_gB >= count.y)
        || (dest_gA < 0 || dest_gA >= count.x || dest_gB < 0 || dest_gB >= count.y))
    {
        throw OutOfRangeException();
    }

    if (src_gA == dest_gA && src_gB == dest_gB) {
        return;
    }

    int indexA = gridPosToDataIndex(src_gA, src_gB);
    int indexB = gridPosToDataIndex(dest_gA, dest_gB);

    T t{ std::move(data[indexA]) };
    data[indexA] = std::move(data[indexB]);
    data[indexB] = std::move(t);
}

template <typename T>
inline int Array2D<T>::gridPosToDataIndex(int gA, int gB) {
    if(gA < 0  || gB < 0 || gA >= count.x || gB >= count.y) {
        throw LogicException(LOC);
    }
    int index = (gB * count.x) + gA;

    return index;
}

template <typename T>
Array2D<T>::~Array2D() {
    if(data) {
        delete[] data;

        data = nullptr;
    }
}

#include <base/math/Bounds2DInt.h>
