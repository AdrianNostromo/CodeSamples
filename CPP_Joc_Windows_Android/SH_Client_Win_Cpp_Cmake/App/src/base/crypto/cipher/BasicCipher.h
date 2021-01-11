#pragma once

#include "ICipher.h"
#include <memory>
#include <base/crypto/algorithm/IAlgorithm.h>

namespace base {
namespace crypto {
class BasicCipher : virtual public ICipher {
private:
    std::shared_ptr<IAlgorithm> algorithm;

public:
    BasicCipher(BasicCipher const&) = delete;
    BasicCipher(BasicCipher &&) = default;
    BasicCipher& operator=(BasicCipher const&) = delete;
    BasicCipher& operator=(BasicCipher &&) = default;

    explicit BasicCipher(std::shared_ptr<IAlgorithm>& algorithm);

    void processBytes(const char* src, char* dest, int count) override;

    ~BasicCipher() override;
};
};
};
