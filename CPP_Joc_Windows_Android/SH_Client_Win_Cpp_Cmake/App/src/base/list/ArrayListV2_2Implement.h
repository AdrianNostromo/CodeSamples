#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include <base/exceptions/LogicException.h>

// This is to implement, not usable.

template <typename T>
class ArrayListV2 final/*Note1001. calls_the_reserved_dispose*/ : public base::Object {priv typedef base::Object super;pub dCtor(ArrayListV2);
    // This uses table doubling logic.
    priv T** dataTable = nullptr;
    priv int tableCount = 0;

    priv int _count = 0;
    priv int _minCapacity = 0;
    priv int _capacity = 0;

	pub explicit ArrayListV2(int _minCapacity=0, int reserve=0);
	
    pub unsigned short count();
    pub unsigned short capacity();

    pub void reserve(int reserveCapacity);
    pub void reserveInsertCapacity(int reserveInsertCapacity);

    pub T& getReference(int index);
    pub T* getPointer(int index);
    pub T* getPointer(int index, T* fallbackValue);
    pub T getDirect(int index);
    pub T getDirect(int index, T* fallbackValue);

    pub T& operator[](int index);
    
	pub T* insertDirect(int index, T val);
    // Fills the range as needed with rangeFillVal until val is inserted.
    // Use this to avoid skipped undefined entries.
	pub T* insertDirectRangeFill(int index, T val, T rangeFillVal);
	pub T* insertReference(int index, T& val);
	pub template <typename... Args>
	T* insert_emplace(int index, Args&&... args);

	pub T* appendReference(T& val);
	pub template<typename... Args>
	T* append_emplace(Args&&... args);
	pub template<typename... Args>
	ArrayListV2<T>* append_emplace_chain(Args&&... args);
	pub void appendDirect(Array1D<T>& src);
	pub void appendDirect(ArrayListV2<T>& src);

	// Commodity function to append a value point that is constructed in the function call.
	pub T* appendDirect(T val);

    // An entry must already exist at the target index. It may be nullptr but it must exist.
    pub T* setDirect(int index, T val);
    pub T* setReference(int index, T& val);

    pub bool removeIndex(int index);
    pub T removeIndexAndGetDirect(int index);

    pub bool removeDirect(T val);
    pub bool removeReference(T& val);

    pub void swap(int indexA, int indexB);

    pub void clear();

	pub ~ArrayListV2() override;
};

template <typename T>
ArrayListV2<T>::ArrayListV2(int _minCapacity = 0, int reserve = 0)
    : super(),
    _minCapacity(_minCapacity)
{
    if (_minCapacity > 0 || initialCapacity > 0) {
        reserve(Math::max(_minCapacity, initialCapacity));
    }
}

template <typename T>
unsigned short ArrayListV2<T>::count() {
    return _count;
}

template <typename T>
unsigned short ArrayListV2<T>::capacity() {
    return _capacity;
}

template <typename T>
void ArrayListV2<T>::reserve(int reserveCapacity) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
void ArrayListV2<T>::reserveInsertCapacity(int reserveInsertCapacity) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T& ArrayListV2<T>::getReference(int index) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T* ArrayListV2<T>::getPointer(int index) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T* ArrayListV2<T>::getPointer(int index, T* fallbackValue) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T ArrayListV2<T>::getDirect(int index) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T ArrayListV2<T>::getDirect(int index, T* fallbackValue) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T& ArrayListV2<T>::operator[](int index) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T* ArrayListV2<T>::insertDirect(int index, T val) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T* ArrayListV2<T>::insertDirectRangeFill(int index, T val, T rangeFillVal) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T* ArrayListV2<T>::insertReference(int index, T& val) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
template <typename... Args>
T* ArrayListV2<T>::insert_emplace(int index, Args&&... args) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T* ArrayListV2<T>::appendReference(T& val) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
template<typename... Args>
T* ArrayListV2<T>::append_emplace(Args&&... args) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
template<typename... Args>
ArrayListV2<T>* ArrayListV2<T>::append_emplace_chain(Args&&... args) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
void ArrayListV2<T>::appendDirect(Array1D<T>& src) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
void ArrayListV2<T>::appendDirect(ArrayListV2<T>& src) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T* ArrayListV2<T>::appendDirect(T val) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T* ArrayListV2<T>::setDirect(int index, T val) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T* ArrayListV2<T>::setReference(int index, T& val) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
bool ArrayListV2<T>::removeIndex(int index) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
T ArrayListV2<T>::removeIndexAndGetDirect(int index) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
bool ArrayListV2<T>::removeDirect(T val) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
bool ArrayListV2<T>::removeReference(T& val) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
void ArrayListV2<T>::swap(int indexA, int indexB) {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
void ArrayListV2<T>::clear() {
    throw LogicException(LOC);
    //asd;
}

template <typename T>
ArrayListV2<T>::~ArrayListV2() {
    reservedDisposeIfNeeded();// Note1001
}
