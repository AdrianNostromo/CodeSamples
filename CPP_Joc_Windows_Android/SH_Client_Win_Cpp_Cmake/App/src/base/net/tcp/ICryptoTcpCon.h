#pragma once

#include "IDataBlocksTcpCon.h"
#include <memory>
#include <base/crypto/cipher/IEncryptCipher.h>
#include <base/crypto/cipher/IDecryptCipher.h>

namespace base {
namespace Net {
class ICryptoTcpCon : virtual public IDataBlocksTcpCon {
public:
    virtual std::shared_ptr<base::crypto::IEncryptCipher> getEncryptionCipher() = 0;
    virtual void setEncryptionCipher(std::shared_ptr<base::crypto::IEncryptCipher> encryptionCipher) = 0;

    virtual std::shared_ptr<base::crypto::IDecryptCipher> getDecryptionCipher() = 0;
    virtual void setDecryptionCipher(std::shared_ptr<base::crypto::IDecryptCipher> decryptionCipher) = 0;

    virtual ~ICryptoTcpCon() = default;
};
};
};
