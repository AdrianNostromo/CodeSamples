#include "TcpCryptoConnection.h"
#include <assert.h>
#include <base/MM.h>

using namespace base::Net;

TcpCryptoConnection::TcpCryptoConnection(std::shared_ptr<Socket>& socket)
    : super(socket)
{
    //void
}

void TcpCryptoConnection::enabledAutoCryptoRead() {
    isEnabledAutoCryptoRead = true;
}

void TcpCryptoConnection::enabledAutoCryptoWrite() {
    isEnabledAutoCryptoWrite = true;
}

std::shared_ptr<base::StreamBuffer> TcpCryptoConnection::extractConfirmedReceivedBlock(int blockLen) {
    std::shared_ptr<base::StreamBuffer> blockNetBuffer = super::extractConfirmedReceivedBlock(blockLen);

    if(isEnabledAutoCryptoRead) {
        // Decrypt.
        // The first 2 int values are not encrypted.

        assert(false);

//        asdA3;
//        char* blockBuffer = blockNetBuffer->getBuffer();
//
//        int cryptoOffset = 4 + 4;
//        int cryptoDataLen = blockBuffer.capacity() - cryptoOffset;
//
//        // Create a temporary buffer.
//        byte[] tmpBuff = new byte[cryptoDataLen];
//        JavaUtil.byteArrayCopy(blockBuffer.array(), cryptoOffset, tmpBuff, 0, cryptoDataLen);
//
//        CryptoUtil.decryptBuffer(tmpBuff, decipher);
//
//        // copy the temporary buffer back to the original one.
//        JavaUtil.byteArrayCopy(tmpBuff, 0, blockBuffer.array(), cryptoOffset, cryptoDataLen);
    }

    return blockNetBuffer;
}

void TcpCryptoConnection::sendBlock(std::shared_ptr<NetMsgsBlock> blockDataNetBuffer) {
    if(isEnabledAutoCryptoWrite) {
        // Encrypt.
        // The first 2 int values are not encrypted.

        assert(false);

//        asdA3;
//        ByteBuffer blockBuffer = blockDataNetBuffer.getBuffer();
//
//        int cryptoOffset = 4 + 4;
//        int cryptoDataLen = blockBuffer.capacity()  - cryptoOffset;
//
//        // Create a temporary buffer.
//        byte[] tmpBuff = new byte[cryptoDataLen];
//        JavaUtil.byteArrayCopy(blockBuffer.array(), cryptoOffset, tmpBuff, 0, cryptoDataLen);
//
//        CryptoUtil.encryptBuffer(tmpBuff, cipher);
//
//        // copy the temporary buffer back to the original one.
//        JavaUtil.byteArrayCopy(tmpBuff, 0, blockBuffer.array(), cryptoOffset, cryptoDataLen);
    }

    super::sendBlock(blockDataNetBuffer);
}

std::shared_ptr<base::crypto::IEncryptCipher> TcpCryptoConnection::getEncryptionCipher() {
    return encryptionCipher;
}

void TcpCryptoConnection::setEncryptionCipher(std::shared_ptr<base::crypto::IEncryptCipher> encryptionCipher) {
    this->encryptionCipher = encryptionCipher;
}

std::shared_ptr<base::crypto::IDecryptCipher> TcpCryptoConnection::getDecryptionCipher() {
    return decryptionCipher;
}

void TcpCryptoConnection::setDecryptionCipher(std::shared_ptr<base::crypto::IDecryptCipher> decryptionCipher) {
    this->decryptionCipher = decryptionCipher;
}

TcpCryptoConnection::~TcpCryptoConnection() {
    //void
}
