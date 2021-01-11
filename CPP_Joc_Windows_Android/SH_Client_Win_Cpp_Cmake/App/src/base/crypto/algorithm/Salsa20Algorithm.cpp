#include "Salsa20Algorithm.h"

using namespace base::crypto;

Salsa20Algorithm::Salsa20Algorithm(const std::uint8_t* key, const std::uint8_t* iv)
    : rawAlgorithm()
{
    rawAlgorithm.setKey(key);
    rawAlgorithm.setIv(iv);
}

void Salsa20Algorithm::processBytes(const std::uint8_t* src, std::uint8_t* dest, int count) {
    rawAlgorithm.processBytes(src, dest, count);
}

Salsa20Algorithm::~Salsa20Algorithm() {
    //void
}
