"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicTcpConnection_1 = require("./BasicTcpConnection");
const DataFormatError_1 = require("base/net/exception/DataFormatError");
const NetDataLimits_1 = require("base/common/net/NetDataLimits");
const StreamBuffer_1 = require("base/buffer/StreamBuffer");
class TcpDataBlocksConnection extends BasicTcpConnection_1.BasicTcpConnection {
    constructor(socket) {
        super(socket);
        this.currentBlockNetBuffer = null;
        this._lastReceivedDataBlockIndice = 0;
        this._sendDataBlockIndiceCounter = 0;
        //void
    }
    sendBlock(blockDataNetBuffer) {
        blockDataNetBuffer.lockForSend(++this._sendDataBlockIndiceCounter);
        if (blockDataNetBuffer.getLength() < NetDataLimits_1.NetDataLimits.BLOCK_LEN_MIN
            || blockDataNetBuffer.getLength() > NetDataLimits_1.NetDataLimits.BLOCK_LEN_MAX
            || blockDataNetBuffer.getOffset() != blockDataNetBuffer.getLength()
            || blockDataNetBuffer.getBoundsLimit() != blockDataNetBuffer.getLength()) {
            throw new Error('Msg block data shorter than the minimum block length or is longer or shorter than the buffer length.');
        }
        this._socket.write(blockDataNetBuffer.getBuffer());
    }
    //@throws DataFormatError
    handleBlockReceive() {
        if (this.currentBlockNetBuffer != null) {
            throw new DataFormatError_1.DataFormatError("Error.");
        }
        let blockLen;
        let blockIndice;
        let isBlockLenComputed = false;
        let combinedPacketsLen = 0;
        let blockPacketsCount = 0;
        for (let i = 0; i < this._receivedNetBuffersList.length; i++) {
            let packetNetBuffer = this._receivedNetBuffersList[i];
            // In case the packet contains data from a previous block, ignore that part.
            combinedPacketsLen += packetNetBuffer.getPostBoundsLength();
            blockPacketsCount++;
            if (!isBlockLenComputed) {
                isBlockLenComputed = true;
                // The bounds is before the blockLen value, unsave peek is required.
                blockLen = packetNetBuffer.peekIntUnsafe(0);
                if (blockLen < NetDataLimits_1.NetDataLimits.BLOCK_LEN_MIN || blockLen > NetDataLimits_1.NetDataLimits.BLOCK_LEN_MAX) {
                    // The block exceeds the lower limit of 12 bytes or the upper limit of 256KB. A error may have occured.
                    throw new DataFormatError_1.DataFormatError("Error.");
                }
                blockIndice = packetNetBuffer.peekIntUnsafe(4);
                if (blockIndice != this._lastReceivedDataBlockIndice + 1) {
                    throw new DataFormatError_1.DataFormatError("Error.");
                }
            }
            if (blockLen <= combinedPacketsLen) {
                this.currentBlockNetBuffer = this.extractConfirmedReceivedBlock(blockLen);
                this._lastReceivedDataBlockIndice = blockIndice;
                break;
            }
        }
    }
    //@throws DataFormatError
    extractConfirmedReceivedBlock(blockLen) {
        // Handle special case when the first packet is a single block.
        let firstPacketNetBuffer = this._receivedNetBuffersList[0];
        if (firstPacketNetBuffer.getPostBoundsLength() == blockLen) {
            this._receivedNetBuffersList.splice(0, 1);
            let ret = firstPacketNetBuffer;
            // Increase the bounds by 4+4 to skip over the blockLen and blockIndice values because peek was used to read them.
            ret.setBoundsLimit(ret.getBoundsLimit() + 4 + 4);
            return ret;
        }
        // Extract packets into the block buffer until the block is created.
        let ret = new StreamBuffer_1.StreamBuffer(Buffer.allocUnsafe(blockLen));
        let neededLen = blockLen;
        while (this._receivedNetBuffersList.length > 0) {
            let packetNetBuffer = this._receivedNetBuffersList[0];
            let nextWriteLen = Math.min(neededLen, packetNetBuffer.getPostBoundsLength());
            if (packetNetBuffer.getOffset() != packetNetBuffer.getBoundsLimit()) {
                throw new DataFormatError_1.DataFormatError("Error.");
            }
            packetNetBuffer.setBoundsLimit(packetNetBuffer.getBoundsLimit() + nextWriteLen);
            ret.writeNetBuffer(packetNetBuffer, nextWriteLen);
            neededLen -= nextWriteLen;
            // No more data in this packet, remove it from the list.
            if (packetNetBuffer.getPostBoundsLength() <= 0) {
                this._receivedNetBuffersList.splice(0, 1);
            }
            if (neededLen < 0) {
                throw new DataFormatError_1.DataFormatError("Error.");
            }
            if (neededLen == 0) {
                ret.setBoundsLimit(4 + 4);
                // Block data read finished.
                break;
            }
        }
        return ret;
    }
}
exports.TcpDataBlocksConnection = TcpDataBlocksConnection;
//# sourceMappingURL=TcpDataBlocksConnection.js.map