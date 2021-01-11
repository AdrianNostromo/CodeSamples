#pragma once

#include "TcpRequestBlocksConnection.h"
#include "ICryptoTcpCon.h"
#include <memory>
#include <base/crypto/cipher/IDecryptCipher.h>
#include <base/crypto/cipher/IEncryptCipher.h>

namespace base {
namespace Net {
class TcpCryptoConnection : public TcpRequestBlocksConnection, virtual public ICryptoTcpCon {
private: typedef TcpRequestBlocksConnection super;
private:
    std::shared_ptr<base::crypto::IEncryptCipher> encryptionCipher = nullptr;
    std::shared_ptr<base::crypto::IDecryptCipher> decryptionCipher = nullptr;

private:
    bool isEnabledAutoCryptoRead = false;
    bool isEnabledAutoCryptoWrite = false;

public:
    TcpCryptoConnection(TcpCryptoConnection const&) = delete;
    TcpCryptoConnection(TcpCryptoConnection &&) = default;
    TcpCryptoConnection& operator=(TcpCryptoConnection const&) = delete;
    TcpCryptoConnection& operator=(TcpCryptoConnection &&) = default;

    explicit TcpCryptoConnection(std::shared_ptr<Socket>& socket);

    void enabledAutoCryptoRead();
    void enabledAutoCryptoWrite();

    void sendBlock(std::shared_ptr<NetMsgsBlock> blockDataNetBuffer) override;

    std::shared_ptr<base::crypto::IEncryptCipher> getEncryptionCipher() override;
    void setEncryptionCipher(std::shared_ptr<base::crypto::IEncryptCipher> encryptionCipher) override;

    std::shared_ptr<base::crypto::IDecryptCipher> getDecryptionCipher() override;
    void setDecryptionCipher(std::shared_ptr<base::crypto::IDecryptCipher> decryptionCipher) override;

    ~TcpCryptoConnection() override;
protected:
    std::shared_ptr<base::StreamBuffer> extractConfirmedReceivedBlock(int blockLen) override;

};
};
};
