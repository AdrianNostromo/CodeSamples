#include "BasicCipher.h"

using namespace base::crypto;

BasicCipher::BasicCipher(std::shared_ptr<IAlgorithm>& algorithm)
    : algorithm(algorithm)
{
    //void
}

void BasicCipher::processBytes(const char* src, char* dest, int count) {
    algorithm->processBytes(reinterpret_cast<const uint8_t*>(src), reinterpret_cast<uint8_t*>(dest), count);
}

BasicCipher::~BasicCipher() {
    //void
}
