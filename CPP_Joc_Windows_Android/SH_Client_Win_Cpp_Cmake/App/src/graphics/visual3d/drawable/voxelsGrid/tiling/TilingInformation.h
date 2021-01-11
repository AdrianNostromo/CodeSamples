#pragma once

#include <base/gh.h>
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>

namespace graphics {
class TilingInformation {pub dCtor(TilingInformation);
    pub class TileConfig {pub dCtor(TileConfig);
        pub Array3D<signed char>* voxelsGrid;

        pub explicit TileConfig(Array3D<signed char>* voxelsGrid)
            : voxelsGrid(voxelsGrid)
        {}
    };
    prot static int NewType();

    prot int type;

    pub Vector3Int tileVoxelsCount;

    pub int enabledSidesTriangulationBitMask;

    // This is used to avoid dynamic cast.
    // It contains the subclass void*;
    prot void* rawSubclassPointer = nullptr;

    pub explicit TilingInformation(int type, Vector3Int& tileVoxelsCount, int enabledSidesTriangulationBitMask);

    pub int getType();

    template <typename T>
    T castAs(int validationType);

    pub virtual ~TilingInformation();
};

template <typename T>
T TilingInformation::castAs(int validationType) {
    if (this->type != validationType || rawSubclassPointer == nullptr) {
        throw LogicException(LOC);
    }

    T cVal = static_cast<T>(rawSubclassPointer);

    return cVal;
}

};
