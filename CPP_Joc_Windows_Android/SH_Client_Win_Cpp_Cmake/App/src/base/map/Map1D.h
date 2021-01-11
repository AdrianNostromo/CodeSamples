#pragma once

#include <base/gh.h>
#include <unordered_map>
#include <base/exceptions/NotFoundException.h>
#include <base/exceptions/LogicException.h>
#include "IMap1D.h"
#include <assert.h>

template <typename K, typename V>
class Map1D : virtual public IMap1D<K, V> {pub dCtor(Map1D);
	pub std::unordered_map<K, V> map;
	
	pub explicit Map1D();

	pub void reserve(int count);

	pub void clear();
	pub int count();
	// Use count instead. Seems to be a more descriptive name.
	pub int size();

	pub V* getPointer(K const& key) override;
	pub V* getPointerB(K key) override;

	pub V& getReference(K const& key) override;

	pub V getDirect(K const& key) override;
	pub V getDirectB(K key) override;
	pub V getDirect(K const& key, V fallbackValue) override;
	pub V getDirectB(K key, V fallbackValue) override;

	pub bool containsKey(K const& key) override;
	pub bool containsKeyB(K key) override;

	// Commodity function to append a value point that is constructed in the function call.
	pub template <typename... Args>
	V* put_emplace(K const& key, bool errorIfPreExist, Args&&... args);
	pub template <typename... Args>
	V* put_emplaceB(K key, bool errorIfPreExist, Args&&... args);

	pub V* putReference(K const& k, V& v, bool errorIfPreExist = false) override;
	pub V* putReferenceB(K k, V& v, bool errorIfPreExist = false) override;

	pub V* putDirect(K const& k, V v, bool errorIfPreExist = false) override;
	pub V* putDirectB(K k, V v, bool errorIfPreExist = false) override;

	pub void removeMustExist(K const& key);
	pub void removeMustExistB(K key);

	pub bool removeOptional(K const& key);
	pub bool removeOptionalB(K key);

	pub V removeAndGetDirect(K const& key);

	pub std::unordered_map<K, V>* getRawMap();

	pub ~Map1D() override;
};

template <typename K, typename V>
Map1D<K, V>::Map1D()
	: map()
{
	//map.reserve(10);
}

template <typename K, typename V>
void Map1D<K, V>::reserve(int count) {
	map.reserve(count);
}

template <typename K, typename V>
void Map1D<K, V>::clear() {
	map.clear();
}

template <typename K, typename V>
int Map1D<K, V>::count() {
	return (int)map.size();
}

template <typename K, typename V>
int Map1D<K, V>::size() {
	return count();
}

template <typename K, typename V>
V* Map1D<K, V>::getPointer(K const& key) {
	typename std::unordered_map<K, V>::iterator got = map.find(key);
	if (got != map.end()) {
		return &got->second;
	}

	return nullptr;
}

template <typename K, typename V>
V* Map1D<K, V>::getPointerB(K key) {
	typename std::unordered_map<K, V>::iterator got = map.find(key);
	if (got != map.end()) {
		return &got->second;
	}

	return nullptr;
}

template <typename K, typename V>
V& Map1D<K, V>::getReference(K const& key) {
    typename std::unordered_map<K, V>::iterator got = map.find(key);
    if (got != map.end()) {
        return got->second;
    }

    throw NotFoundException();
}

template <typename K, typename V>
V Map1D<K, V>::getDirect(K const& key) {
    typename std::unordered_map<K, V>::iterator got = map.find(key);
    if (got != map.end()) {
        return got->second;
    }

    throw NotFoundException();
}

template <typename K, typename V>
V Map1D<K, V>::getDirectB(K key) {
	return getDirect(key);
}

template <typename K, typename V>
V Map1D<K, V>::getDirect(K const& key, V fallbackValue) {
	typename std::unordered_map<K, V>::iterator got = map.find(key);
	if (got != map.end()) {
		return got->second;
	}

	return fallbackValue;
}

template <typename K, typename V>
V Map1D<K, V>::getDirectB(K key, V fallbackValue) {
	return getDirect(key, fallbackValue);
}

template <typename K, typename V>
bool Map1D<K, V>::containsKey(K const& key) {
    if (map.find(key) == map.end()) {
		return false;
	}

	return true;
}

template <typename K, typename V>
bool Map1D<K, V>::containsKeyB(K key) {
    if (map.find(key) == map.end()) {
		return false;
	}

	return true;
}

template <typename K, typename V>
V* Map1D<K, V>::putReference(K const& key, V& v, bool errorIfPreExist) {
    if (containsKey(key)) {
		if (errorIfPreExist) {
			throw LogicException(LOC);
		}

        removeMustExist(key);
    }

    std::pair<typename std::unordered_map<K, V>::iterator, bool> got = map.insert(
        std::make_pair(key, v)
    );

    if (!got.second) {
        throw LogicException(LOC);
    }

    return &got.first->second;
}

template <typename K, typename V>
V* Map1D<K, V>::putReferenceB(K key, V& v, bool errorIfPreExist) {
	return putReference(key, v, errorIfPreExist);
}

template <typename K, typename V>
V* Map1D<K, V>::putDirect(K const& key, V v, bool errorIfPreExist) {
	return putReference(key, v, errorIfPreExist);
}

template <typename K, typename V>
V* Map1D<K, V>::putDirectB(K key, V v, bool errorIfPreExist) {
	return putReference(key, v, errorIfPreExist);
}

template <typename K, typename V>
template <typename... Args>
V* Map1D<K, V>::put_emplace(K const& key, bool errorIfPreExist, Args&&... args) {
	if (containsKey(key)) {
		if (errorIfPreExist) {
			throw LogicException(LOC);
		}

		removeMustExist(key);
	}

	std::pair<typename std::unordered_map<K, V>::iterator, bool> got = map.emplace(
		std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(std::forward<Args>(args)...)
	);

	if (!got.second) {
		throw LogicException(LOC);
	}

	return &got.first->second;
}

template <typename K, typename V>
template <typename... Args>
V* Map1D<K, V>::put_emplaceB(K key, bool errorIfPreExist, Args&&... args) {
	if (containsKey(key)) {
		if (errorIfPreExist) {
			throw LogicException(LOC);
		}

		removeMustExist(key);
	}

	std::pair<typename std::unordered_map<K, V>::iterator, bool> got = map.emplace(
		std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(std::forward<Args>(args)...)
	);

	if (!got.second) {
		throw LogicException(LOC);
	}

	return &got.first->second;
}

template <typename K, typename V>
void Map1D<K, V>::removeMustExist(K const& key) {
	if (!removeOptional(key)) {
		throw NotFoundException();
	}
}

template <typename K, typename V>
void Map1D<K, V>::removeMustExistB(K key) {
	removeMustExist(key);
}

template <typename K, typename V>
bool Map1D<K, V>::removeOptional(K const& key) {
	int removedCt = map.erase(key);
	if (removedCt == 0) {
		return false;
	}else if(removedCt != 1) {
		throw LogicException(LOC);
	}

	return true;
}

template <typename K, typename V>
bool Map1D<K, V>::removeOptionalB(K key) {
	return removeOptional(key);
}

template <typename K, typename V>
V Map1D<K, V>::removeAndGetDirect(K const& key) {
    V val = getDirect(key);

	int removedCt = map.erase(key);
	if (removedCt == 0) {
		throw LogicException(LOC);
	}

	return val;
}

template <typename K, typename V>
std::unordered_map<K, V>* Map1D<K, V>::getRawMap() {
    return &map;
}

template <typename K, typename V>
Map1D<K, V>::~Map1D() {
	//void
}
