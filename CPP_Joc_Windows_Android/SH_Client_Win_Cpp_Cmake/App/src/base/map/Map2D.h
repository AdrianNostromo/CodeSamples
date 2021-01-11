#pragma once

#include <base/gh.h>
#include "Map1D.h"
#include <memory>

template <typename K1, typename K2, typename V>
class Map2D {pub dCtor(Map2D);
	pub Map1D<K1, std::shared_ptr<Map1D<K2, V>>> map;

	priv unsigned int defaultReserveCount = 10;

	pub explicit Map2D();

	pub void clear();
	pub void clear(K1& k1);
	pub int count(K1& k1);
    // Use count instead. Seems to be a more descriptive name.

	pub void reserve(unsigned int count);

	pub V& getReference(K1 const& k1, K2 const& k2);

	pub V getDirect(K1 const& k1, K2 const& k2);
	pub V getDirect(K1 const& k1, K2 const& k2, V fallbackValue);
	pub V getDirectB(K1 k1, K2 k2, V fallbackValue);

	// Returns the k1 map.
	pub Map1D<K2, V>* getPointer(K1& k1);

	pub V* getPointer(K1 const& k1, K2 const& k2);

	pub bool containsKey(K1 const& k1, K2 const& k2);
	pub V* putReference(K1 const& k1, K2 const& k2, V& v, bool errorIfPreExist = false);
	pub V* putDirect(K1 const& k1, K2 const& k2, V v, bool errorIfPreExist = false);
	pub template<class... Args>
	V* put_emplace(K1 const& k1, K2 const& k2, bool errorIfPreExist, Args&&... args);

	pub void removeMustExist(K1 const& k1, K2 const& k2);
	pub bool removeOptional(K1 const& k1, K2 const& k2);
	pub V removeAndGetDirect(K1 const& k1, K2 const& k2);

	pub Map1D<K1, std::shared_ptr<Map1D<K2, V>>>* getRawMap();

	pub ~Map2D();
};

template <typename K1, typename K2, typename V>
Map2D<K1, K2, V>::Map2D()
	: map()
{
	map.reserve(defaultReserveCount);
}

template <typename K1, typename K2, typename V>
void Map2D<K1, K2, V>::reserve(unsigned int count) {
	defaultReserveCount = count;

	map.reserve(count);
}

template <typename K1, typename K2, typename V>
int Map2D<K1, K2, V>::count(K1& k1) {
    std::shared_ptr<Map1D<K2, V>> subMap = map.getDirect(k1, nullptr);
    if (subMap) {
        return subMap->count();
    }

    return 0;
}

template <typename K1, typename K2, typename V>
void Map2D<K1, K2, V>::clear() {
    map.clear();
}

template <typename K1, typename K2, typename V>
void Map2D<K1, K2, V>::clear(K1& k1) {
	std::shared_ptr<Map1D<K2, V>> subMap = map.getDirect(k1, nullptr);
	if (subMap) {
		subMap->clear();
	}
}

template <typename K1, typename K2, typename V>
V* Map2D<K1, K2, V>::getPointer(K1 const& k1, K2 const& k2) {
    std::shared_ptr<Map1D<K2, V>> subMap = map.getDirect(k1, nullptr);
	if (subMap) {
		return subMap->getPointer(k2);
	}

	return nullptr;
}

template <typename K1, typename K2, typename V>
Map1D<K2, V>* Map2D<K1, K2, V>::getPointer(K1& k1) {
    std::shared_ptr<Map1D<K2, V>> subMap = map.getDirect(k1, nullptr);
	if (subMap) {
		return subMap.get();
	}

	return nullptr;
}

template <typename K1, typename K2, typename V>
V& Map2D<K1, K2, V>::getReference(K1 const& k1, K2 const& k2) {
    std::shared_ptr<Map1D<K2, V>> subMap = map.getDirect(k1);
    // subMap is valid here or an exception was thrown by map.getDirect(k1).
    return subMap->getReference(k2);
}

template <typename K1, typename K2, typename V>
V Map2D<K1, K2, V>::getDirect(K1 const& k1, K2 const& k2) {
    std::shared_ptr<Map1D<K2, V>> subMap = map.getDirect(k1);
    // subMap is valid here or an exception was thrown by map.getDirect(k1).
    return subMap->getDirect(k2);
}

template <typename K1, typename K2, typename V>
V Map2D<K1, K2, V>::getDirect(K1 const& k1, K2 const& k2, V fallbackValue) {
    std::shared_ptr<Map1D<K2, V>> subMap = map.getDirect(k1, nullptr);
	if (subMap) {
		return subMap->getDirect(k2, fallbackValue);
	}

	return fallbackValue;
}

template <typename K1, typename K2, typename V>
V Map2D<K1, K2, V>::getDirectB(K1 k1, K2 k2, V fallbackValue) {
    return getDirect(k1, k2, fallbackValue);
}

template <typename K1, typename K2, typename V>
bool Map2D<K1, K2, V>::containsKey(K1 const& k1, K2 const& k2) {
    std::shared_ptr<Map1D<K2, V>> subMap = map.getDirect(k1, nullptr);
	if (subMap) {
		return subMap->containsKey(k2);
	}

	return false;
}

template <typename K1, typename K2, typename V>
V* Map2D<K1, K2, V>::putReference(K1 const& k1, K2 const& k2, V& v, bool errorIfPreExist) {
	std::shared_ptr<Map1D<K2, V>> subMap = map.getDirect(k1, nullptr);
	if (!subMap) {
		subMap = *map.putDirect(k1, std::make_shared<Map1D<K2, V>>());
	}

	return subMap->putReference(k2, v, errorIfPreExist);
}

template <typename K1, typename K2, typename V>
V* Map2D<K1, K2, V>::putDirect(K1 const& k1, K2 const& k2, V v, bool errorIfPreExist) {
    return putReference(k1, k2, v, errorIfPreExist);
}

template <typename K1, typename K2, typename V>
template<typename... Args>
V* Map2D<K1, K2, V>::put_emplace(K1 const& k1, K2 const& k2, bool errorIfPreExist, Args&&... args) {
    std::shared_ptr<Map1D<K2, V>> subMap = map.getDirect(k1, nullptr);
	if (!subMap) {
        subMap = *map.putDirect(k1, std::make_shared<Map1D<K2, V>>());
	}

	return subMap->put_emplace(k2, errorIfPreExist, std::forward<Args>(args)...);
}

template <typename K1, typename K2, typename V>
void Map2D<K1, K2, V>::removeMustExist(K1 const& k1, K2 const& k2) {
	if (!removeOptional(k1, k2)) {
		throw NotFoundException();
	}
}

template <typename K1, typename K2, typename V>
bool Map2D<K1, K2, V>::removeOptional(K1 const& k1, K2 const& k2) {
    std::shared_ptr<Map1D<K2, V>> subMap = map.getDirect(k1, nullptr);
	if (subMap != nullptr) {
	    bool b = subMap->removeOptional(k2);

	    if(b == true && subMap->size() == 0) {
	        // Entry was removed, if the submap is empty, remove it.
            map.removeMustExist(k1);
	    }

	    return b;
	}

	return false;
}

template <typename K1, typename K2, typename V>
V Map2D<K1, K2, V>::removeAndGetDirect(K1 const& k1, K2 const& k2) {
    std::shared_ptr<Map1D<K2, V>> subMap = map.getDirect(k1, nullptr);
	if (subMap != nullptr) {
	    return subMap->removeAndGetDirect(k2);
	}

    throw NotFoundException();
}

template <typename K1, typename K2, typename V>
Map1D<K1, std::shared_ptr<Map1D<K2, V>>>* Map2D<K1, K2, V>::getRawMap() {
    return &map;
}

template <typename K1, typename K2, typename V>
Map2D<K1, K2, V>::~Map2D() {
    //void
}
