#pragma once

#include <base/gh.h>
#include <base/exceptions/OutOfRangeException.h>
#include <base/exceptions/LogicException.h>
#include <cstring>
#include <base/exceptions/LogicException.h>
#include <base/MM.h>
#include <base/math/Math.h>

template <typename T>
class Array1D {pub dCtor(Array1D);
    // Don't use const because that disables the move_operator.
    priv int _count;

    pub T* data = nullptr;

    pub explicit Array1D(int _count);
    pub explicit Array1D(int _count, T* data);
    pub explicit Array1D(Array1D<T>& srcData, int srcOffset, int _count);

    pub void resizeAndClear(int _count);
    pub void resizeWithElementsMove(int _count);

    pub void zeroSetData();
    pub void memSetData(int val);

    pub T* getData();

    pub inline int getLength();
    pub inline int getCount();
    pub inline int count();
    
    pub inline T* getPointer(int index);
    pub inline T getDirect(int index);
    pub inline T& getReference(int index);

    pub inline void setPointer(int index, T* val);
    pub inline void setDirect(int index, T val);
    pub inline void setReference(int index, T& val);
    
    pub void set(int offset, Array1D<T>& src, int srcOffset, int _count);

    pub void swap(int indexA, int indexB);

    pub bool containsReference(T& value);
    pub int indexOfReference(T& value);

    pub virtual ~Array1D();
};

template <typename T>
Array1D<T>::Array1D(int _count)
    : Array1D<T>(_count, new T[_count]())
{
    //void
}

template <typename T>
Array1D<T>::Array1D(int _count, T* data)
    : _count(_count), data(data)
{
    if(_count < 0) {
        throw LogicException(LOC);
    }
}

template <typename T>
Array1D<T>::Array1D(Array1D<T>& srcData, int srcOffset, int _count)
    : _count(_count)
{
    if(_count < 0) {
        throw LogicException(LOC);
    }

    data = new T[_count]();

    for(int i=0;i< _count;i++) {
        data[i] = srcData.getDirect(srcOffset + i);
    }
}

template <typename T>
void Array1D<T>::resizeAndClear(int _count) {
    if (data) {
        delete[] data;

        data = nullptr;

        this->_count = 0;
    }

    if (_count > 0) {
        data = new T[_count]();

        this->_count = _count;
    }
}

template <typename T>
void Array1D<T>::resizeWithElementsMove(int _count) {
    T* oldData = data;
    int entriesToMoveCount = Math::min(this->_count, _count);

    if (data != nullptr) {
        data = nullptr;
        this->_count = 0;
    }

    if (_count > 0) {
        data = new T[_count]();

        this->_count = _count;

        if (oldData != nullptr) {
            for (int i = 0; i < entriesToMoveCount; i++) {
                data[i] = std::move(oldData[i]);
            }
        }
    }

    if (oldData != nullptr) {
        delete[] oldData;

        oldData = nullptr;
    }
}

template <typename T>
void Array1D<T>::zeroSetData() {
    memset(data, 0, _count * sizeof(T));
}

template <typename T>
void Array1D<T>::memSetData(int val) {
    memset(data, val, _count * sizeof(T));
}

template <typename T>
T* Array1D<T>::getData() {
    return data;
}

template <typename T>
inline int Array1D<T>::getLength() {
    return _count;
}

template <typename T>
inline int Array1D<T>::getCount() {
    return _count;
}

template <typename T>
inline int Array1D<T>::count() {
    return _count;
}

template <typename T>
inline T* Array1D<T>::getPointer(int index) {
    if(index < 0 || index >= _count) {
        throw OutOfRangeException();
    }

    return &data[index];
}

template <typename T>
inline T Array1D<T>::getDirect(int index) {
    if(index < 0 || index >= _count) {
        throw OutOfRangeException();
    }

    return data[index];
}

template <typename T>
inline T& Array1D<T>::getReference(int index) {
    if(index < 0 || index >= _count) {
        throw OutOfRangeException();
    }

    return data[index];
}

template <typename T>
inline void Array1D<T>::setDirect(int index, T val) {
    if(index < 0 || index >= _count) {
        throw OutOfRangeException();
    }

    data[index] = std::move(val);
}

template <typename T>
inline void Array1D<T>::setPointer(int index, T* val) {
    if(index < 0 || index >= _count) {
        throw OutOfRangeException();
    }

    data[index] = *val;
}

template <typename T>
inline void Array1D<T>::setReference(int index, T& val) {
    if(index < 0 || index >= _count) {
        throw OutOfRangeException();
    }

    data[index] = val;
}

template <typename T>
void Array1D<T>::set(int offset, Array1D<T>& src, int srcOffset, int _count) {
    for(int i=-0;i< _count;i++) {
        T* val = src.getPointer(srcOffset + i);

        setPointer(offset + i, val);
    }
}

template <typename T>
int Array1D<T>::indexOfReference(T& value) {
    for(int i=-0;i<_count;i++) {
        T& val = data[i];

        if(val == value) {
            return i;
        }
    }

    return -1;
}

template <typename T>
void Array1D<T>::swap(int indexA, int indexB) {
    if (indexA < 0 || indexA >= _count || indexB < 0 || indexB >= _count) {
        throw OutOfRangeException();
    }

    if (indexA == indexB) {
        return;
    }

    T t{ std::move(data[indexA]) };
    data[indexA] = std::move(data[indexB]);
    data[indexB] = std::move(t);
}

template <typename T>
bool Array1D<T>::containsReference(T& value) {
    if(indexOf(value) >= 0) {
        return true;
    }

    return false;
}

template <typename T>
Array1D<T>::~Array1D() {
    if(data != nullptr) {
        delete[] data;

        data = nullptr;
    }
}
