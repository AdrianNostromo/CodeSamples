#include <base/crypto/algorithm/Salsa20Algorithm.h>
#include "CryptoUtil.h"
#include <base/exceptions/LogicException.h>
#include <base/crypto/cipher/EncryptCipher.h>
#include <base/crypto/cipher/DecryptCipher.h>

using namespace base::crypto;

const std::string CryptoUtil::Algorithms::Salsa20 = "_salsa20";

std::shared_ptr<base::crypto::IEncryptCipher> CryptoUtil::newEncryptCipher(
    std::string algorithmType,
    const char* key, int keySize, const char* iv, int ivSize)
{
    if(algorithmType == CryptoUtil::Algorithms::Salsa20) {
        if(keySize != Salsa20Algorithm::KEY_SIZE || ivSize != Salsa20Algorithm::IV_SIZE) {
            throw LogicException(LOC);
        }

        std::shared_ptr<base::crypto::Salsa20Algorithm> algorithm = std::make_shared<base::crypto::Salsa20Algorithm>(reinterpret_cast<const uint8_t*>(key), reinterpret_cast<const uint8_t*>(iv));

        std::shared_ptr<base::crypto::EncryptCipher> cipher = std::make_shared<base::crypto::EncryptCipher>(algorithm);

        return cipher;
    }

    throw LogicException(LOC);
}

std::shared_ptr<base::crypto::IDecryptCipher> CryptoUtil::newDecryptCipher(
    std::string algorithmType,
    const char* key, int keySize, const char* iv, int ivSize)
{
    if(algorithmType == CryptoUtil::Algorithms::Salsa20) {
        if(keySize != Salsa20Algorithm::KEY_SIZE || ivSize != Salsa20Algorithm::IV_SIZE) {
            throw LogicException(LOC);
        }

        std::shared_ptr<base::crypto::Salsa20Algorithm> algorithm = std::make_shared<base::crypto::Salsa20Algorithm>(reinterpret_cast<const uint8_t*>(key), reinterpret_cast<const uint8_t*>(iv));

        std::shared_ptr<base::crypto::DecryptCipher> cipher = std::make_shared<base::crypto::DecryptCipher>(algorithm);

        return cipher;
    }

    throw LogicException(LOC);
}
