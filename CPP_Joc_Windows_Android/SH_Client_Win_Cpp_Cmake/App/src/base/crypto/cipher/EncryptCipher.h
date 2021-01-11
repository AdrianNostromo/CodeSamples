#pragma once

#include "BasicCipher.h"
#include "IEncryptCipher.h"

namespace base {
namespace crypto {
class EncryptCipher : public BasicCipher, virtual public IEncryptCipher {
private: typedef BasicCipher super;
public:
    EncryptCipher(EncryptCipher const&) = delete;
    EncryptCipher(EncryptCipher &&) = default;
    EncryptCipher& operator=(EncryptCipher const&) = delete;
    EncryptCipher& operator=(EncryptCipher &&) = default;

    explicit EncryptCipher(std::shared_ptr<IAlgorithm> algorithm);

    std::string encryptStringToBase64(std::string& data) override;

    ~EncryptCipher() override;
};
};
};
