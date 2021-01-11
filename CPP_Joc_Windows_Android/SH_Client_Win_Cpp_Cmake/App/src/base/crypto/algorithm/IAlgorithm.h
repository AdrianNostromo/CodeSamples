#pragma once

#include <stdint.h>

namespace base {
namespace crypto {
class IAlgorithm {
public:
    virtual void processBytes(const uint8_t* src, uint8_t* dest, int count) = 0;

    virtual ~IAlgorithm() = default;
};
};
};
