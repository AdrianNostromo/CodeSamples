#pragma once

#include <string>
#include "ICipher.h"

namespace base {
namespace crypto {
class IDecryptCipher : virtual public ICipher {
public:
    virtual int decryptBytesToInt(const char* eData) = 0;
    virtual std::string decryptBase64ToString(std::string& eData) = 0;

    virtual ~IDecryptCipher() = default;
};
};
};
