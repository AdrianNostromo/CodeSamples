#pragma once

#include <sh/scenarios/battlezones_a_v1/gw/online/server/IServer.h>
#include <worldGame3D/gw/online/server/ServerExtendable.h>
#include <assert.h>

namespace battlezones_a_v1 {
class TSBase : public base::ServerExtendable, virtual public IServer {
private: typedef base::ServerExtendable super;
protected:
class TestNetCon : virtual public base::Net::IGeneralTcpCon {
    public:
    TestNetCon(TestNetCon const&) = delete;
    TestNetCon(TestNetCon &&) = default;
    TestNetCon& operator=(TestNetCon const&) = delete;
    TestNetCon& operator=(TestNetCon &&) = default;

    explicit TestNetCon() {
        //void
    }

    std::shared_ptr<base::StreamBuffer> popNextMsg() override {
        assert(false);
		return nullptr;
    }

    void sendBlock(std::shared_ptr<base::Net::NetMsgsBlock> blockDataNetBuffer) override {
        //void
    }

    std::shared_ptr<base::crypto::IEncryptCipher> getEncryptionCipher() override {
        assert(false);
		return nullptr;
    }

    void setEncryptionCipher(std::shared_ptr<base::crypto::IEncryptCipher> encryptionCipher) override {
        assert(false);
    }

    std::shared_ptr<base::crypto::IDecryptCipher> getDecryptionCipher() override {
        assert(false);
		return nullptr;
    }

    void setDecryptionCipher(std::shared_ptr<base::crypto::IDecryptCipher> decryptionCipher) override {
        assert(false);
    }

    ~TestNetCon() override {
        //void
    }
    };
public:
    TSBase(TSBase const&) = delete;
    TSBase(TSBase &&) = default;
    TSBase& operator=(TSBase const&) = delete;
    TSBase& operator=(TSBase &&) = default;

    explicit TSBase(std::shared_ptr<base::ServerConnectConfig> serverConnectConfig);

    ~TSBase() override;
};
};
