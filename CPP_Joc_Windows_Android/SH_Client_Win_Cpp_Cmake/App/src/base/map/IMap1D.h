#pragma once

#include <base/gh.h>

template <typename K, typename V>
class IMap1D {
    pub virtual V& getReference(K const& key) = 0;

    pub virtual V getDirect(K const& key) = 0;
    pub virtual V getDirectB(K key) = 0;
    pub virtual V getDirect(K const& key, V notFoundValue) = 0;
    pub virtual V getDirectB(K key, V notFoundValue) = 0;

    pub virtual V* getPointer(K const& k) = 0;
    pub virtual V* getPointerB(K k) = 0;

    pub virtual bool containsKey(K const& key) = 0;
    pub virtual bool containsKeyB(K key) = 0;

    pub virtual V* putReference(K const& k, V& v, bool errorIfPreExist=false) = 0;
    pub virtual V* putReferenceB(K k, V& v, bool errorIfPreExist = false) = 0;

    pub virtual V* putDirect(K const& k, V v, bool errorIfPreExist = false) = 0;
    pub virtual V* putDirectB(K k, V v, bool errorIfPreExist = false) = 0;

    pub virtual ~IMap1D() = default;
};
