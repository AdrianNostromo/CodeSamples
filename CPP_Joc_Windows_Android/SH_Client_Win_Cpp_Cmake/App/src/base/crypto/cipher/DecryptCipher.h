#pragma once

#include "BasicCipher.h"
#include "IDecryptCipher.h"

namespace base {
namespace crypto {
class DecryptCipher : public BasicCipher, virtual public IDecryptCipher {
private: typedef BasicCipher super;
public:
    DecryptCipher(DecryptCipher const&) = delete;
    DecryptCipher(DecryptCipher &&) = default;
    DecryptCipher& operator=(DecryptCipher const&) = delete;
    DecryptCipher& operator=(DecryptCipher &&) = default;

    explicit DecryptCipher(std::shared_ptr<IAlgorithm> algorithm);

    int decryptBytesToInt(const char* eData) override;
    std::string decryptBase64ToString(std::string& eData) override;

    ~DecryptCipher() override;
};
};
};
