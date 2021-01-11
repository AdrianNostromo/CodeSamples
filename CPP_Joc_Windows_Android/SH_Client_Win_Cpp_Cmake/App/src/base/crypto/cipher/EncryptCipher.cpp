#include <base/encoding/base64/Base64.h>
#include "EncryptCipher.h"
#include <base/MM.h>

using namespace base::crypto;

EncryptCipher::EncryptCipher(std::shared_ptr<IAlgorithm> algorithm)
    : super(algorithm)
{
    //void
}

std::string EncryptCipher::encryptStringToBase64(std::string& data) {
    char* eData = new char[data.size()];

    processBytes(data.data(), &eData[0], static_cast<int>(data.size()));

    std::string base64String = base::encoding::Base64::encode(&eData[0], data.size());

    delete[] eData;

    return base64String;
}

EncryptCipher::~EncryptCipher() {
    //void
}
