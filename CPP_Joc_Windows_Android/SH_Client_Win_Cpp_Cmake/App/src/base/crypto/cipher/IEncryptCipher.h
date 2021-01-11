#pragma once

#include <string>
#include "ICipher.h"

namespace base {
namespace crypto {
class IEncryptCipher : virtual public ICipher {
public:
    virtual std::string encryptStringToBase64(std::string& data) = 0;

    virtual ~IEncryptCipher() = default;
};
};
};
