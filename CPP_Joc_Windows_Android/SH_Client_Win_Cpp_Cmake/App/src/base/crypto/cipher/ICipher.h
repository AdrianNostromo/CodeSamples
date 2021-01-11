#pragma once

#include <string>

namespace base {
namespace crypto {
class ICipher {
public:
    virtual void processBytes(const char* src, char* dest, int count) = 0;

    virtual ~ICipher() = default;
};
};
};
