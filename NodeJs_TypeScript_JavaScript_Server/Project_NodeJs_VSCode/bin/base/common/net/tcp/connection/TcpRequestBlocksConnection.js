"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const TcpDataBlocksConnection_1 = require("./TcpDataBlocksConnection");
const LogicError_1 = require("base/exception/LogicError");
const DataFormatError_1 = require("base/net/exception/DataFormatError");
const NetDataLimits_1 = require("base/common/net/NetDataLimits");
class TcpRequestBlocksConnection extends TcpDataBlocksConnection_1.TcpDataBlocksConnection {
    constructor(socket) {
        super(socket);
        //void
    }
    //@throws DataFormatError, Error
    popNextMsg() {
        if (this.currentBlockNetBuffer != null) {
            let msgBuff = this._getNextMessageFromCurrentBlockBuffer(false);
            if (msgBuff != null) {
                return msgBuff;
            }
        }
        this.handleBlockReceive();
        if (this.currentBlockNetBuffer != null) {
            let msgBuff = this._getNextMessageFromCurrentBlockBuffer(true);
            if (msgBuff == null) {
                throw new LogicError_1.LogicError('Logic error. This implies a empty block received.');
            }
            return msgBuff;
        }
        return null;
    }
    _getNextMessageFromCurrentBlockBuffer(isMsgMandatory) {
        if (this.currentBlockNetBuffer == null) {
            return null;
        }
        // Check if buffer has any unprocessed messages.
        if (this.currentBlockNetBuffer.getBoundsLimit() >= this.currentBlockNetBuffer.getLength()) {
            // End of block data reached.
            if (isMsgMandatory) {
                throw new DataFormatError_1.DataFormatError("Error.");
            }
            this.currentBlockNetBuffer = null;
            return null;
        }
        // There are more messages. Validate the msg size to make sure the msgLen value is valid.
        this.currentBlockNetBuffer.setOffset(this.currentBlockNetBuffer.getBoundsLimit());
        let msgLen = this.currentBlockNetBuffer.peekIntUnsafe(0);
        if (msgLen == 0) {
            // The end of the block zero padding occured. There is no more messages in this block.
            if (isMsgMandatory) {
                throw new DataFormatError_1.DataFormatError("Error.");
            }
            this.currentBlockNetBuffer = null;
            return null;
        }
        if (msgLen < NetDataLimits_1.NetDataLimits.MSG_LEN_MIN || msgLen > NetDataLimits_1.NetDataLimits.MSG_LEN_MAX) {
            // The msg length exceeds the lower limit of 8 bytes or the upper limit of 256KB. A error may have occured.
            throw new DataFormatError_1.DataFormatError("Error.");
        }
        if (this.currentBlockNetBuffer.getBoundsLimit() + msgLen > this.currentBlockNetBuffer.getLength()) {
            // There is not enough data for this message, the msgLen value is not correct.
            throw new DataFormatError_1.DataFormatError("Error.");
        }
        this.currentBlockNetBuffer.setBoundsLimit(this.currentBlockNetBuffer.getBoundsLimit() + msgLen);
        return this.currentBlockNetBuffer;
    }
}
exports.TcpRequestBlocksConnection = TcpRequestBlocksConnection;
//# sourceMappingURL=TcpRequestBlocksConnection.js.map