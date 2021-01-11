#include <base/net/buffer/NetMsgsBlock.h>
#include "TcpDataBlocksConnection.h"
#include <base/exceptions/LogicException.h>
#include <base/exceptions/DataFormatException.h>
#include <base/math/Math.h>
#include <base/net/util/NetDataLimits.h>

using namespace base::Net;

TcpDataBlocksConnection::TcpDataBlocksConnection(std::shared_ptr<Socket>& socket)
    : super(socket)
{
    //void
}

void TcpDataBlocksConnection::sendBlock(std::shared_ptr<NetMsgsBlock> blockDataNetBuffer) {
    if(socket->getIsAnyError()) {
        throw LogicException(LOC);
    }

    blockDataNetBuffer->lockForSend(++sendDataBlockIndiceCounter);

    if(blockDataNetBuffer->getLength() < NetDataLimits::BLOCK_LEN_MIN || blockDataNetBuffer->getOffset() != blockDataNetBuffer->getLength() || blockDataNetBuffer->getBoundsLimit() != blockDataNetBuffer->getLength()) {
        // "Msg block data shorter than the minimum block length or is longer or shorter than the buffer length.";

        throw LogicException(LOC);
    }

    // Save the data to a list until the write thread writes it to the socket.
    socket->write(blockDataNetBuffer->getBuffer());
}

void TcpDataBlocksConnection::handleBlockReceive() {
    if(currentBlockNetBuffer != nullptr) {
        throw DataFormatException();
    }

    int blockLen = 0;
    int blockIndice;
    bool isBlockLenComputed = false;

    int combinedPacketsLen = 0;
    int blockPacketsCount = 0;

    for(int i=0;i<socket->getReceivedNetBuffersList()->size();i++) {
        std::shared_ptr<base::StreamBuffer> packetNetBuffer = socket->getReceivedNetBuffersList()->getDirect(i);

        // In case the packet contains data from a previous block, ignore that gamepart.
        combinedPacketsLen += packetNetBuffer->getPostBoundsLength();
        blockPacketsCount++;

        if(!isBlockLenComputed) {
            isBlockLenComputed = true;

            // The bounds is before the blockLen value, unsave peek is required.
            blockLen = packetNetBuffer->peekIntUnsafe(0);
            if(blockLen < NetDataLimits::BLOCK_LEN_MIN || blockLen > NetDataLimits::BLOCK_LEN_MAX) {
                // The block exceeds the lower limit of 12 bytes or the upper limit of 256KB. A error may have occured.
                throw DataFormatException();
            }

            blockIndice = packetNetBuffer->peekIntUnsafe(4);
            if(blockIndice != this->lastReceivedDataBlockIndice + 1) {
                throw DataFormatException();
            }
        }

        if(blockLen <= combinedPacketsLen) {
            currentBlockNetBuffer = this->extractConfirmedReceivedBlock(blockLen);

            this->lastReceivedDataBlockIndice++;

            break;
        }
    }
}

std::shared_ptr<base::StreamBuffer> TcpDataBlocksConnection::extractConfirmedReceivedBlock(int blockLen) {
    // Handle special case when the first packet is a single block.
    std::shared_ptr<base::StreamBuffer> firstPacketNetBuffer = socket->getReceivedNetBuffersList()->getDirect(0);
    if(firstPacketNetBuffer->getPostBoundsLength() == blockLen) {
        socket->lockReceiveNetBuffersList();
        socket->getReceivedNetBuffersList()->remove(0);
        socket->unlockReceiveNetBuffersList();

        std::shared_ptr<base::StreamBuffer> ret = firstPacketNetBuffer;
        // Increase the bounds by 4+4 to skip over the blockLen and blockIndice values because peek was used to read them.
        ret->setBoundsLimit(ret->getBoundsLimit() + 4 + 4);

        return ret;
    }

    // Extract packets into the block buffer until the block is created.
    std::shared_ptr<base::StreamBuffer> ret = std::make_shared<base::StreamBuffer>(blockLen);

    int neededLen = blockLen;
    while(socket->getReceivedNetBuffersList()->size() > 0) {
        std::shared_ptr<base::StreamBuffer> packetNetBuffer = socket->getReceivedNetBuffersList()->getDirect(0);

        int nextWriteLen = static_cast<int>(Math::min(neededLen, packetNetBuffer->getPostBoundsLength()));

        if(packetNetBuffer->getOffset() != packetNetBuffer->getBoundsLimit()) {
            throw DataFormatException();
        }
        packetNetBuffer->setBoundsLimit(packetNetBuffer->getBoundsLimit() + nextWriteLen);

        ret->writeBuffer(*packetNetBuffer, nextWriteLen);

        neededLen -= nextWriteLen;

        // No more data in this packet, remove it from the list.
        if(packetNetBuffer->getPostBoundsLength() <= 0) {
            socket->lockReceiveNetBuffersList();
            socket->getReceivedNetBuffersList()->remove(0);
            socket->unlockReceiveNetBuffersList();
        }

        if(neededLen < 0) {
            throw DataFormatException();
        }
        if(neededLen == 0) {
            ret->setBoundsLimit(4 + 4);

            // Block data read finished.

            break;
        }
    }

    return ret;
}

TcpDataBlocksConnection::~TcpDataBlocksConnection() {
    //void
}
