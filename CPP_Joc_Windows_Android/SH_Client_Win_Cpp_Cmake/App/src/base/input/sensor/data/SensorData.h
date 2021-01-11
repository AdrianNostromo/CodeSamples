#pragma once

#include <base/gh.h>
#include <base/exceptions/LogicException.h>

namespace base {
class SensorData {pub dCtor(SensorData);
    // This is used to avoid dynamic cast.
    // It contains the subclass void*;
    prot void* rawSubclassPointer = nullptr;

    pub explicit SensorData();

    // Note. Don't include data_type check because the same data_class is used for multiple data_types.
    pub template <typename T>
    T castToSubtype();

    pub virtual ~SensorData();
};

template <typename T>
T SensorData::castToSubtype() {
    if(rawSubclassPointer == nullptr) {
        throw LogicException(LOC);
    }

    T cVal = static_cast<T>(rawSubclassPointer);

    return cVal;
}

};
