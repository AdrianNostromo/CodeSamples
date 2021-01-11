#pragma once

#include <memory>
#include <base/crypto/cipher/IEncryptCipher.h>
#include <base/crypto/cipher/IDecryptCipher.h>

namespace base {
namespace crypto {
class CryptoUtil {
public:
    class Algorithms {
    public:
        static const std::string Salsa20;
    };

public:
    static std::shared_ptr<base::crypto::IEncryptCipher> newEncryptCipher(
        std::string algorithmType,
        const char* key, int keySize, const char* iv, int ivSize);
    static std::shared_ptr<base::crypto::IDecryptCipher> newDecryptCipher(
        std::string algorithmType,
        const char* key, int keySize, const char* iv, int ivSize);

};
};
};
