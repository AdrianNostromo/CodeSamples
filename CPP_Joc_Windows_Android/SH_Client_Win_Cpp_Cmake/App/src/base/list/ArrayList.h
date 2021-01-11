#pragma once

#include <base/gh.h>
#include <vector>
#include <algorithm>
#include <memory>
#include <base/exceptions/OutOfRangeException.h>
#include <base/exceptions/LogicException.h>
#include <base/list/Array1D.h>
#include <base/object/Object.h>
#include <base/MM.h>

template <typename T>
class ArrayList final/*Note1001. calls_the_reserved_dispose*/ : public base::Object {priv typedef base::Object super;pub dCtor(ArrayList);
	// This is used to detect bad_code modify operations where there should be none.
	priv bool isLocked = false;
	
	pub std::vector<T> list{};
	
	pub explicit ArrayList();
	pub explicit ArrayList(int reserve);

	pub void reserve(int count);
	pub void resize(int count);
	// Use count instead. It seems better.
	pub int size();
	pub int count();

	pub bool getIsLocked();
	pub void lock();
	pub void unlock();

	pub T* getPointer(int index, bool mustExist=true);
	pub T* getPointer(int index, T* fallbackValue);
	pub T getDirect(int index);
	pub T getDirect(int index, T fallbackValue);
	pub T& getReference(int index);

	// An entry must already exist at the target index. It may be nullptr but it must exist.
	pub T* setDirect(int index, T val);
	pub T* setReference(int index, T& val);

	pub T* insertDirect(int index, T val);
	// Fills the range as needed with rangeFillVal until val is inserted.
	pub T* setOrInsertRangeFillDirect(int index, T val, T rangeFillVal);
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
	ArrayList<T>* append_emplace_chain(Args&&... args);
	pub void appendDirect(Array1D<T>& src);
	pub void appendDirect(ArrayList<T>& src);

	// Commodity function to append a value point that is constructed in the function call.
	pub T* appendDirect(T val);

	// Comodity function to append a value point that is constructed in the function call.
	pub ArrayList<T>* appendDirect_chain(T val);
	pub ArrayList<T>* appendDirect_chain(ArrayList<T>* val);

	pub bool remove(int index);
	pub bool removeIndex(int index);
	pub T removeAndGetDirect(int index);

	pub bool remove(T& val);
	pub bool removeDirect(T val);
	pub bool removeReference(T& val);

	pub void swap(int indexA, int indexB);

	pub int indexOf(T& val);
	pub int indexOfDirect(T val);
	pub bool contains(T& val);
	pub bool containsReference(T& val);
	pub bool containsDirect(T val);
	pub bool containsAllDirect(ArrayList<T>& valsList);

	pub void clear();

	pub Array1D<T>* toNewArray1DDirectElemCopy(int offset, int count);
	pub Array1D<T>* toNewArray1DDirectElemCopy();

	pub std::shared_ptr<ArrayList<T>> toNewArray(int offset, int count);
	pub std::shared_ptr<ArrayList<T>> toNewArray();

	pub bool equals(ArrayList<T>& src);

	pub ~ArrayList() override;
};

template <typename T>
ArrayList<T>::ArrayList()
	: super()
{
	//void
}

template <typename T>
ArrayList<T>::ArrayList(int reserve)
	: super()
{
	list.reserve(reserve);
}

template <typename T>
int ArrayList<T>::size() {
	return count();
}

template <typename T>
int ArrayList<T>::count() {
	return static_cast<int>(list.size());
}

template <typename T>
bool ArrayList<T>::getIsLocked() {
	return isLocked;
}

template <typename T>
void ArrayList<T>::lock() {
	this->isLocked = true;
}

template <typename T>
void ArrayList<T>::unlock() {
	this->isLocked = false;
}

template <typename T>
void ArrayList<T>::reserve(int count) {
	list.reserve((unsigned long)count);
}

template <typename T>
void ArrayList<T>::resize(int count) {
	list.resize((unsigned long)count);
}

template <typename T>
T* ArrayList<T>::getPointer(int index, bool mustExist) {
	if (index >= static_cast<int>(list.size())) {
		if (!mustExist) {
			return nullptr;
		}

		throw OutOfRangeException();
	}

	return &list[index];
}

template <typename T>
T* ArrayList<T>::getPointer(int index, T* fallbackValue) {
	if (index >= static_cast<int>(list.size())) {
		return fallbackValue;
	}

	return &list[index];
}

template <typename T>
T ArrayList<T>::getDirect(int index) {
	if (index >= static_cast<int>(list.size())) {
		throw OutOfRangeException();
	}

	return list[index];
}

template <typename T>
T ArrayList<T>::getDirect(int index, T fallbackValue) {
	if (index >= static_cast<int>(list.size())) {
		return fallbackValue;
	}

	return list[index];
}

template <typename T>
T& ArrayList<T>::getReference(int index) {
	if (index >= static_cast<int>(list.size())) {
		throw OutOfRangeException();
	}

	return list[index];
}

template <typename T>
T* ArrayList<T>::setDirect(int index, T val) {
	if (isLocked) {
		throw LogicException(LOC);
	}
	if (index >= static_cast<int>(list.size())) {
		throw OutOfRangeException();
	}

	list[index] = std::move(val);

	return &list[index];
}

template <typename T>
T* ArrayList<T>::setReference(int index, T& val) {
	if (isLocked) {
		throw LogicException(LOC);
	}
	if (index >= static_cast<int>(list.size())) {
		throw OutOfRangeException();
	}

	list[index] = val;

	return &list[index];
}

template <typename T>
T* ArrayList<T>::insertDirect(int index, T val) {
    return insertReference(index, val);
}

template <typename T>
T* ArrayList<T>::setOrInsertRangeFillDirect(int index, T val, T rangeFillVal) {
    while(index > count()) {
        insertReference(count(), rangeFillVal);
    }

	if (index < count()) {
		return setReference(index, val);
	} else {
		return insertReference(index, val);
	}
}

template <typename T>
T* ArrayList<T>::insertDirectRangeFill(int index, T val, T rangeFillVal) {
    while(index > count()) {
        insertReference(count(), rangeFillVal);
    }

    return insertReference(index, val);
}

template <typename T>
T* ArrayList<T>::insertReference(int index, T& val) {
	if (isLocked) {
		throw LogicException(LOC);
	}
    if (index > static_cast<int>(list.size())) {
        throw OutOfRangeException();
    }

	if (index == list.size()) {
		list.push_back(val);
	} else {
		list.insert(list.begin() + index, val);
	}

	return &list[index];
}

template <typename T>
template<typename... Args>
T* ArrayList<T>::insert_emplace(int index, Args&&... args) {
	if (isLocked) {
		throw LogicException(LOC);
	}
	if (index > static_cast<int>(list.size())) {
		throw OutOfRangeException();
	}

	if (index == list.size()) {
		// This is a special case because the below-er version will try to append bfore index 0, that causes an error. It worked with clangs, but visual_studio has a runtime error here.
		list.emplace_back(std::forward<Args>(args)...);
	}else {
		list.emplace(list.begin() + index, std::forward<Args>(args)...);
	}

	return &list[index];
}

template <typename T>
T* ArrayList<T>::appendReference(T& val) {
	return insertReference(size(), val);
}

template <typename T>
template<typename... Args>
T* ArrayList<T>::append_emplace(Args&&... args) {
	return insert_emplace(static_cast<int>(list.size()), std::forward<Args>(args)...);
}

template <typename T>
template<typename... Args>
ArrayList<T>* ArrayList<T>::append_emplace_chain(Args&&... args) {
	insert_emplace(static_cast<int>(list.size()), std::forward<Args>(args)...);

	return this;
}

template <typename T>
void ArrayList<T>::appendDirect(Array1D<T>& src) {
    for(int i=0;i<src.getLength();i++) {
        appendDirect(src.getDirect(i));
    }
}

template <typename T>
void ArrayList<T>::appendDirect(ArrayList<T>& src) {
    for(int i=0;i<src.count();i++) {
        appendDirect(src.getDirect(i));
    }
}

template <typename T>
T* ArrayList<T>::appendDirect(T val) {
	return appendReference(val);
}

template <typename T>
ArrayList<T>* ArrayList<T>::appendDirect_chain(T val) {
    appendReference(val);

    return this;
}

template <typename T>
ArrayList<T>* ArrayList<T>::appendDirect_chain(ArrayList<T>* val) {
	appendDirect(*val);

    return this;
}

template <typename T>
bool ArrayList<T>::remove(int index) {
	if (isLocked) {
		throw LogicException(LOC);
	}
	if (index >= static_cast<int>(list.size())) {
		throw OutOfRangeException();
	}

	list.erase(list.begin() + index);

	return true;
}

template <typename T>
bool ArrayList<T>::removeIndex(int index) {
	if (isLocked) {
		throw LogicException(LOC);
	}
	if (index >= static_cast<int>(list.size())) {
		throw OutOfRangeException();
	}

	list.erase(list.begin() + index);

	return true;
}

template <typename T>
T ArrayList<T>::removeAndGetDirect(int index) {
	if (isLocked) {
		throw LogicException(LOC);
	}
	if (index >= static_cast<int>(list.size())) {
		throw OutOfRangeException();
	}

	T val = list[index];

	list.erase(list.begin() + index);

	return val;
}

template <typename T>
bool ArrayList<T>::remove(T& val) {
	return removeReference(val);
}

template <typename T>
bool ArrayList<T>::removeDirect(T val) {
	return removeReference(val);
}

template <typename T>
bool ArrayList<T>::removeReference(T& val) {
    int index = indexOf(val);

	if (index < 0) {
		return false;
	}

	if(!removeIndex(index)) {
        throw LogicException(LOC);
	}

	return true;
}

template <typename T>
void ArrayList<T>::swap(int indexA, int indexB) {
	if (indexA < 0 || indexA >= static_cast<int>(list.size()) || indexB < 0 || indexB >= static_cast<int>(list.size())) {
		throw OutOfRangeException();
	}

	if (indexA == indexB) {
		return;
	}

	T t{ std::move(list[indexA]) };
	list[indexA] = std::move(list[indexB]);
	list[indexB] = std::move(t);
}

template <typename T>
int ArrayList<T>::indexOf(T& val) {
    if(static_cast<int>(list.size()) == 0) {
        // This is required because std::distance will return 0 if the list has a size of 0;
        return -1;
    }

    typename std::vector<T>::iterator it = std::find(list.begin(), list.end(), val);
    if (it != list.end()) {
        int index = std::distance(list.begin(), it);

        return index;
    }

	return -1;
}

template <typename T>
int ArrayList<T>::indexOfDirect(T val) {
    return indexOf(val);
}

template <typename T>
bool ArrayList<T>::contains(T& val) {
	if (indexOf(val) >= 0) {
		return true;
	}

	return false;
}

template <typename T>
bool ArrayList<T>::containsReference(T& val) {
	if (indexOf(val) >= 0) {
		return true;
	}

	return false;
}

template <typename T>
bool ArrayList<T>::containsDirect(T val) {
	return containsReference(val);
}

template <typename T>
bool ArrayList<T>::containsAllDirect(ArrayList<T>& valsList) {
	for(int i=0;i<valsList.size();i++) {
	    T val = valsList.getDirect(i);
	    if(!containsReference(val)) {
	        return false;
	    }
	}

    return true;
}

template <typename T>
void ArrayList<T>::clear() {
	list.clear();
}

template <typename T>
std::shared_ptr<ArrayList<T>> ArrayList<T>::toNewArray(int offset, int count) {
    std::shared_ptr<ArrayList<T>> ret = std::make_shared<ArrayList<T>>();

    for(int i=0;i<count;i++) {
        T val = getDirect(offset + i);

        ret->appendReference(val);
    }

    return ret;
}

template <typename T>
std::shared_ptr<ArrayList<T>> ArrayList<T>::toNewArray() {
    return toNewArrayDirectElemCopy(0, count());
}

template <typename T>
Array1D<T>* ArrayList<T>::toNewArray1DDirectElemCopy(int offset, int count) {
    T* data = newt T[count];
    for(int i=0;i<count;i++) {
        data[i] = list[offset + i];
    }

    Array1D<T>* ret = newt Array1D<T>(count, data);

    return ret;
}

template <typename T>
Array1D<T>* ArrayList<T>::toNewArray1DDirectElemCopy() {
    return toNewArray1DDirectElemCopy(0, size());
}

template <typename T>
bool ArrayList<T>::equals(ArrayList<T>& valsList) {
    if(size() != valsList.size()) {
        return false;
    }

    for(int i=0;i<size();i++) {
        T& valA = getReference(i);
        T& valB = valsList.getReference(i);

        if(valA != valB) {
            return false;
        }
    }

    return true;
}

template <typename T>
ArrayList<T>::~ArrayList() {
	reservedDisposeIfNeeded();// Note1001
}
