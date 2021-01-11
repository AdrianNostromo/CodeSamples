#include <base/buffer/util/BitsUtil.h>
#include <base/encoding/base64/Base64.h>
#include "DecryptCipher.h"
#include <base/MM.h>

using namespace base::crypto;

DecryptCipher::DecryptCipher(std::shared_ptr<IAlgorithm> algorithm)
    : super(algorithm)
{
    //void
}

int DecryptCipher::decryptBytesToInt(const char* eData) {
    char data[4];

    processBytes(eData, &data[0], 4);

    int ret = base::BitsUtil::getInt(&data[0], true);

    return ret;
}

std::string DecryptCipher::decryptBase64ToString(std::string& eData) {
    base::Buffer binaryEDataString = base::encoding::Base64::decode(eData);

    char* data = new char[binaryEDataString.getBytesCount()];

    processBytes(binaryEDataString.getBlockCharPointer(), &data[0], static_cast<int>(binaryEDataString.getBytesCount()));

    std::string ret{data, static_cast<size_t>(binaryEDataString.getBytesCount())};

    delete[] data;

    return ret;
}

DecryptCipher::~DecryptCipher() {
    //void
}
