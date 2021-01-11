#include "NetMsgsBlock.h"
#include <base/exceptions/LogicException.h>
#include <base/net/util/NetDataLimits.h>

using namespace base::Net;

NetMsgsBlock::NetMsgsBlock(int msgsCount, int msgDataLen)
    : super(4 + 4 + msgDataLen + 4 * msgsCount)
{
    //void
}

NetMsgsBlock* NetMsgsBlock::startMsgWrite() {
    if(!isBlockInitialized) {
        isBlockInitialized = true;

        // Skip the block len and block index values.
        this->skipInt();
        this->skipInt();
    }

    if(isMsgWriteInProgress) {
        throw LogicException(LOC);
    }

    isMsgWriteInProgress = true;
    currentMsgOffsetStart = this->getOffset();

    this->skipInt();

    return this;
}

NetMsgsBlock* NetMsgsBlock::endMsgWrite() {
    if(!isMsgWriteInProgress) {
        throw LogicException(LOC);
    }

    isMsgWriteInProgress = false;

    // Write the msg length.
    int msgEndOffset = this->getOffset();
    int msgLength = msgEndOffset - currentMsgOffsetStart;

    this->setOffset(currentMsgOffsetStart);
    this->writeInt(msgLength);

    //Check if the msg type is valid.
    int msgType = this->peekInt();
    if(msgType <= 0) {
        throw LogicException(LOC);
    }

    // Finalize
    this->setOffset(msgEndOffset);

    return this;
}

void NetMsgsBlock::lockForSend(int blockIndice) {
    if(!isBlockInitialized || isMsgWriteInProgress || isLockedForSend) {
        throw LogicException(LOC);
    }
    if(isMsgWriteInProgress || this->getLength() < NetDataLimits::BLOCK_LEN_MIN || this->getOffset() != getLength() || this->getBoundsLimit() != getLength()) {
        throw LogicException(LOC);
    }

    // Write the block length.
    this->setOffset(0);
    this->writeInt(this->getLength());

    this->writeInt(blockIndice);

    // Finalize
    this->setOffset(this->getLength());

    isLockedForSend = true;
}

bool NetMsgsBlock::getIsLockedForSend() {
    return isLockedForSend;
}

NetMsgsBlock* NetMsgsBlock::writeInt(int value) {
    super::writeInt(value);

    return this;
}

NetMsgsBlock* NetMsgsBlock::writeString(std::string& value) {
    super::writeString(value);

    return this;
}

NetMsgsBlock* NetMsgsBlock::writeBuffer(base::StreamBuffer& srcBytes) {
    super::writeBuffer(srcBytes);

    return this;
}

NetMsgsBlock::~NetMsgsBlock() {
    //void
}
