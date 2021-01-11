"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const NetDataLimits_1 = require("./NetDataLimits");
const LogicError_1 = require("base/exception/LogicError");
const StreamBuffer_1 = require("base/buffer/StreamBuffer");
class NetMsgsBlock extends StreamBuffer_1.StreamBuffer {
    constructor(msgsCount, msgDataLen) {
        super(Buffer.allocUnsafe(4 + 4 + msgDataLen + 4 * msgsCount));
        this._isMsgWriteInProgress = false;
        this._currentMsgOffsetStart = -1;
        this._isLockedForSend = false;
        this._isBlockInitialized = false;
        //void
    }
    startMsgWrite() {
        if (!this._isBlockInitialized) {
            this._isBlockInitialized = true;
            // Skip the block len and block index values.
            this.skipInt();
            this.skipInt();
        }
        if (this._isMsgWriteInProgress) {
            throw new LogicError_1.LogicError('Logic error.');
        }
        this._isMsgWriteInProgress = true;
        this._currentMsgOffsetStart = this.getOffset();
        this.skipInt();
        return this;
    }
    endMsgWrite() {
        if (!this._isMsgWriteInProgress) {
            throw new LogicError_1.LogicError('Logic error.');
        }
        this._isMsgWriteInProgress = false;
        // Write the msg length.
        let msgEndOffset = this.getOffset();
        let msgLength = msgEndOffset - this._currentMsgOffsetStart;
        this.setOffset(this._currentMsgOffsetStart);
        this.writeInt(msgLength);
        //Check if the msg type is valid.
        let msgType = this.peekInt();
        if (msgType <= 0) {
            throw new LogicError_1.LogicError('Logic error.');
        }
        // Finalize
        this.setOffset(msgEndOffset);
        return this;
    }
    lockForSend(blockIndice) {
        if (!this._isBlockInitialized || this._isMsgWriteInProgress || this._isLockedForSend) {
            throw new LogicError_1.LogicError('Logic error.');
        }
        if (this._isMsgWriteInProgress || this.length < NetDataLimits_1.NetDataLimits.BLOCK_LEN_MIN || this.getOffset() != this.length || this.getBoundsLimit() != this.length) {
            throw new LogicError_1.LogicError('Logic error.');
        }
        // Write the block length.
        this.setOffset(0);
        this.writeInt(this.length);
        this.writeInt(blockIndice);
        // Finalize
        this.setOffset(this.length);
        this._isLockedForSend = true;
    }
    getIsLockedForSend() {
        return this._isLockedForSend;
    }
    writeInt(value) {
        super.writeInt(value);
        return this;
    }
    writeBuffer(srcBuffer) {
        super.writeBuffer(srcBuffer);
        return this;
    }
    writeString(value) {
        super.writeString(value);
        return this;
    }
}
exports.NetMsgsBlock = NetMsgsBlock;
//# sourceMappingURL=NetMsgsBlock.js.map