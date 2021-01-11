#pragma once

#include <base/crypto/rawalgorithm/Salsa20.h>
#include "IAlgorithm.h"

namespace base {
namespace crypto {
class Salsa20Algorithm : virtual public IAlgorithm {
public:
    // Values are the ones from the raw algorithm. They are used to validate the key and iv sizes;
    // base::crypto::Salsa20::size_t::KEY_SIZE;
    static const int KEY_SIZE = 32;
    // base::crypto::Salsa20::size_t::IV_SIZE;
    static const int IV_SIZE = 8;

private:
    base::crypto::Salsa20 rawAlgorithm;

public:
    Salsa20Algorithm(Salsa20Algorithm const&) = delete;
    Salsa20Algorithm(Salsa20Algorithm &&) = default;
    Salsa20Algorithm& operator=(Salsa20Algorithm const&) = delete;
    Salsa20Algorithm& operator=(Salsa20Algorithm &&) = default;

    explicit Salsa20Algorithm(const std::uint8_t* key, const std::uint8_t* iv);

    void processBytes(const uint8_t* src, uint8_t* dest, int count) override;

    ~Salsa20Algorithm() override;
};
};
};
