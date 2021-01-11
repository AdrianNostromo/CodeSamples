#include "TcpRequestBlocksConnection.h"
#include <base/exceptions/LogicException.h>
#include <base/exceptions/DataFormatException.h>
#include <base/net/util/NetDataLimits.h>

using namespace base::Net;

TcpRequestBlocksConnection::TcpRequestBlocksConnection(std::shared_ptr<Socket>& socket)
    : super(socket)
{
    //void
}

std::shared_ptr<base::StreamBuffer> TcpRequestBlocksConnection::popNextMsg() {
    if(socket->getIsAnyError()) {
        throw LogicException(LOC);
    }

    if(currentBlockNetBuffer != nullptr) {
        std::shared_ptr<base::StreamBuffer> msgBuff = getNextMessageFromCurrentBlockBuffer(false);
        if(msgBuff != nullptr) {
            return msgBuff;
        }
    }

    this->handleBlockReceive();
    if(currentBlockNetBuffer != nullptr) {
        std::shared_ptr<base::StreamBuffer> msgBuff = getNextMessageFromCurrentBlockBuffer(true);
        if(msgBuff == nullptr) {
            // "Logic error. This implies a empty block received.";

            throw LogicException(LOC);
        }

        return msgBuff;
    }

    return nullptr;
}

std::shared_ptr<base::StreamBuffer> TcpRequestBlocksConnection::getNextMessageFromCurrentBlockBuffer(bool isMsgMandatory) {
    if(currentBlockNetBuffer == nullptr) {
        return nullptr;
    }

    // Check if buffer has any unprocessed messages.
    if(currentBlockNetBuffer->getBoundsLimit() >= currentBlockNetBuffer->getLength()) {
        // End of block data reached.

        if(isMsgMandatory) {
            throw DataFormatException();
        }

        currentBlockNetBuffer = nullptr;

        return nullptr;
    }

    // There are more messages. Validate the msg size to make sure the msgLen value is valid.
    currentBlockNetBuffer->setOffset(currentBlockNetBuffer->getBoundsLimit());
    int msgLen = currentBlockNetBuffer->peekIntUnsafe(0);
    if(msgLen == 0) {
        // The end of the block zero padding occured. There is no more messages in this block.

        if(isMsgMandatory) {
            throw DataFormatException();
        }

        currentBlockNetBuffer = nullptr;

        return nullptr;
    }

    if(msgLen < NetDataLimits::MSG_LEN_MIN || msgLen > NetDataLimits::MSG_LEN_MAX) {
        // The msg length exceeds the lower limit of 8 bytes or the upper limit of 256KB. A error may have occured.

        throw DataFormatException();
    }

    if(currentBlockNetBuffer->getBoundsLimit() + msgLen > currentBlockNetBuffer->getLength()) {
        // There is not enough data for this message, the msgLen value is not correct.
        throw DataFormatException();
    }

    currentBlockNetBuffer->setBoundsLimit(currentBlockNetBuffer->getBoundsLimit() + msgLen);

    return currentBlockNetBuffer;
}

TcpRequestBlocksConnection::~TcpRequestBlocksConnection() {
    //void
}
