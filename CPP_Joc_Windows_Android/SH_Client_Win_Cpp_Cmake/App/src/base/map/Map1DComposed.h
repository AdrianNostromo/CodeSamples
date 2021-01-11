#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include "Map1D.h"
#include <base/list/Array1D.h>
#include <base/exceptions/LogicException.h>
#include "IMap1D.h"
#include <base/exceptions/NotFoundException.h>

template <typename K, typename V>
class Map1DComposed : virtual public IMap1D<K, V> {pub dCtor(Map1DComposed);
    priv ArrayList<Map1D<K, V>*> mapsList{};

    priv int writeMapIndex = 0;

    pub explicit Map1DComposed(Map1D<K, V>* initialMap);
    pub void addMap(Map1D<K, V>* map);
    pub void setMapsList(Array1D<Map1D<K, V>*>& mapsArray);

    pub V& getReference(K const& k) override;
    pub V getDirect(K const& k) override;
    pub V getDirectB(K k) override;
    pub V getDirect(K const& key, V notFoundValue) override;
    pub V getDirectB(K key, V notFoundValue) override;
    pub V* getPointer(K const& k) override;
    pub V* getPointerB(K k) override;

    pub bool containsKey(K const& key) override;
    pub bool containsKeyB(K key) override;

    pub V* putReference(K const& k, V& v, bool errorIfPreExist = false) override;
    pub V* putReferenceB(K k, V& v, bool errorIfPreExist = false) override;

    pub V* putDirect(K const& k, V v, bool errorIfPreExist = false) override;
    pub V* putDirectB(K k, V v, bool errorIfPreExist = false) override;

    pub void clear();

    pub virtual ~Map1DComposed() override;
};

template <typename K, typename V>
Map1DComposed<K, V>::Map1DComposed(Map1D<K, V>* initialMap) {
    addMap(initialMap);
}

template <typename K, typename V>
void Map1DComposed<K, V>::addMap(Map1D<K, V>* map) {
    mapsList.appendDirect(map);
}

template <typename K, typename V>
void Map1DComposed<K, V>::setMapsList(Array1D<Map1D<K, V>*>& mapsArray) {
    clear();

    for(int i=0;i<mapsArray.size();i++) {
        Map1D<K, V>* map = mapsArray.getDirect(i);

        addMap(map);
    }
}

template <typename K, typename V>
V* Map1DComposed<K, V>::getPointer(K const& k) {
    for(int i=0;i<mapsList.size();i++) {
        Map1D<K, V>* map = mapsList.getDirect(i);
        
        V* val = map->getPointer(k);
        if(val != nullptr) {
            return val;
        }
    }
    
    return nullptr;
}

template <typename K, typename V>
V* Map1DComposed<K, V>::getPointerB(K k) {
    for(int i=0;i<mapsList.size();i++) {
        Map1D<K, V>* map = mapsList.getDirect(i);
        
        V* val = map->getPointer(k);
        if(val != nullptr) {
            return val;
        }
    }
    
    return nullptr;
}

template <typename K, typename V>
V& Map1DComposed<K, V>::getReference(K const& k) {
    for (int i = 0; i < mapsList.size(); i++) {
        Map1D<K, V>* map = mapsList.getDirect(i);

        V* val = map->getPointer(k);
        if (val != nullptr) {
            return *val;
        }
    }

    throw NotFoundException();
}

template <typename K, typename V>
V Map1DComposed<K, V>::getDirect(K const& k) {
    return getDirect(k, 0);
}

template <typename K, typename V>
V Map1DComposed<K, V>::getDirectB(K key) {
	return getDirect(key);
}

template <typename K, typename V>
V Map1DComposed<K, V>::getDirect(K const& k, V notFoundValue) {
    for(int i=0;i<mapsList.size();i++) {
        Map1D<K, V>* map = mapsList.getDirect(i);

        V* val = map->getPointer(k);
        if(val != nullptr) {
            return *val;
        }
    }

    return notFoundValue;
}

template <typename K, typename V>
V Map1DComposed<K, V>::getDirectB(K key, V fallbackValue) {
	return getDirect(key, fallbackValue);
}

template <typename K, typename V>
bool Map1DComposed<K, V>::containsKey(K const& key) {
    for(int i=0;i<mapsList.size();i++) {
        Map1D<K, V>* map = mapsList.getDirect(i);
    
        bool contains = map->containsKey(key);
        if(contains) {
            return true;
        }
    }

    return false;
}

template <typename K, typename V>
bool Map1DComposed<K, V>::containsKeyB(K key) {
    for(int i=0;i<mapsList.size();i++) {
        Map1D<K, V>* map = mapsList.getDirect(i);
    
        bool contains = map->containsKey(key);
        if(contains) {
            return true;
        }
    }

    return false;
}

template <typename K, typename V>
V* Map1DComposed<K, V>::putReference(K const& k, V& v, bool errorIfPreExist) {
    if(writeMapIndex >= mapsList.size()) {
        throw LogicException(LOC);
    }

    Map1D<K, V>* map = mapsList.getDirect(writeMapIndex);
    return map->putReference(k, v, errorIfPreExist);
}

template <typename K, typename V>
V* Map1DComposed<K, V>::putReferenceB(K key, V& v, bool errorIfPreExist) {
	return putReference(key, v, errorIfPreExist);
}

template <typename K, typename V>
V* Map1DComposed<K, V>::putDirect(K const& key, V v, bool errorIfPreExist) {
	return putReference(key, v, errorIfPreExist);
}

template <typename K, typename V>
V* Map1DComposed<K, V>::putDirectB(K key, V v, bool errorIfPreExist) {
	return putReference(key, v, errorIfPreExist);
}

template <typename K, typename V>
void Map1DComposed<K, V>::clear() {
    for(int i=mapsList.size()-1;i>=0;i--) {
        Map1D<K, V>* map = mapsList.removeAndGetDirect(i);

        delete map;
    }
}

template <typename K, typename V>
Map1DComposed<K, V>::~Map1DComposed() {
    clear();
}
