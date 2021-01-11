#pragma once

#include <base/buffer/StreamBuffer.h>

namespace base {
namespace Net {
class NetMsgsBlock : public StreamBuffer {
private: typedef StreamBuffer super;
private:
    bool isMsgWriteInProgress = false;
    int currentMsgOffsetStart = -1;

    bool isLockedForSend = false;

    bool isBlockInitialized = false;

public:
    NetMsgsBlock(NetMsgsBlock const&) = delete;
    NetMsgsBlock(NetMsgsBlock &&) = default;
    NetMsgsBlock& operator=(NetMsgsBlock const&) = delete;
    NetMsgsBlock& operator=(NetMsgsBlock &&) = default;

    explicit NetMsgsBlock(int msgsCount, int msgDataLen);

    NetMsgsBlock* startMsgWrite();
    NetMsgsBlock* endMsgWrite();

    void lockForSend(int blockIndice);
    bool getIsLockedForSend();

    NetMsgsBlock* writeInt(int value) override;
    NetMsgsBlock* writeString(std::string& value) override;
    NetMsgsBlock* writeBuffer(base::StreamBuffer& srcBytes) override;

    ~NetMsgsBlock() override;
};
};
};
