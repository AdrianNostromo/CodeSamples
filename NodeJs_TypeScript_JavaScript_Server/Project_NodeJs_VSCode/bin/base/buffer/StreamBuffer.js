"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const string_decoder = require("string_decoder");
const TextUtil_1 = require("base/util/TextUtil");
class StreamBuffer {
    constructor(buffer) {
        this.setBuffer(buffer);
    }
    static stringInBufferSize(value) {
        return 2 + TextUtil_1.TextUtil.countUtf8Bytes(value);
    }
    setBuffer(buffer) {
        this.buffer = buffer;
        this.offset = 0;
        this.length = this.buffer.length;
        this.boundsLimit = this.length;
        this.postBoundsLength = this.length - this.boundsLimit;
    }
    readInt() {
        if (this.offset + 4 > this.length || this.offset + 4 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        let ret = this.buffer.readInt32BE(this.offset);
        this.offset += 4;
        return ret;
    }
    readByte() {
        if (this.offset + 1 > this.length || this.offset + 1 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        // NOTE : This is a unsigned byte. The client uses signed bytes.
        let ret = this.buffer.readUInt8(this.offset);
        this.offset += 1;
        return ret;
    }
    readFloat() {
        if (this.offset + 4 > this.length || this.offset + 4 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        let ret = this.buffer.readFloatBE(this.offset);
        this.offset += 4;
        return ret;
    }
    readShort() {
        if (this.offset + 2 > this.length || this.offset + 2 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        let ret = this.buffer.readInt16BE(this.offset);
        this.offset += 2;
        return ret;
    }
    readBoolean() {
        let ret = this.readByte();
        let boolRet;
        if (ret == 0) {
            boolRet = false;
        }
        else if (ret == 1) {
            boolRet = true;
        }
        else {
            throw new Error('Data error.');
        }
        return boolRet;
    }
    readString() {
        let stringBytesLength = this.readShort();
        let stringDataBuffer = this.readBuffer(stringBytesLength);
        let ret = StreamBuffer.decoder.end(stringDataBuffer);
        return ret;
    }
    readBuffer(len) {
        if (this.offset + len > this.length || this.offset + len > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        let ret = Buffer.allocUnsafe(len);
        this.buffer.copy(ret, 0, this.offset, this.offset + len);
        this.offset += len;
        return ret;
    }
    peekInt() {
        if (this.offset + 4 > this.length || this.offset + 4 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        let ret = this.buffer.readInt32BE(this.offset);
        //this.offset += 4;
        return ret;
    }
    peekIntUnsafe(extraOffset) {
        let finalOffset = this.offset;
        if (extraOffset !== undefined && extraOffset !== null) {
            finalOffset += extraOffset;
        }
        if (finalOffset + 4 > this.length) {
            throw new Error('Bounds error.');
        }
        let ret = this.buffer.readInt32BE(finalOffset);
        return ret;
    }
    writeInt(value) {
        if (value === undefined || value === null) {
            throw new Error('Bounds error.');
        }
        if (this.offset + 4 > this.length || this.offset + 4 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        this.buffer.writeUInt32BE(value, this.offset);
        this.offset += 4;
    }
    writeByte(value) {
        if (value === undefined || value === null) {
            throw new Error('Bounds error.');
        }
        if (this.offset + 1 > this.length || this.offset + 1 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        // NOTE : This is a unsigned byte. The client uses signed bytes.
        this.buffer.writeUInt8(value, this.offset);
        this.offset += 1;
    }
    writeFloat(value) {
        if (value === undefined || value === null) {
            throw new Error('Bounds error.');
        }
        if (this.offset + 4 > this.length || this.offset + 4 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        this.buffer.writeFloatBE(value, this.offset);
        this.offset += 4;
    }
    writeShort(value) {
        if (value === undefined || value === null) {
            throw new Error('Bounds error.');
        }
        if (this.offset + 2 > this.length || this.offset + 2 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        this.buffer.writeUInt16BE(value, this.offset);
        this.offset += 2;
    }
    writeBoolean(value) {
        let finalVal;
        if (value === true) {
            finalVal = 1;
        }
        else if (value === false) {
            finalVal = 0;
        }
        else {
            throw new Error('Data error.');
        }
        this.writeByte(finalVal);
    }
    // This method is fully overridden when crypto is used on a above layer.
    writeString(value) {
        if (value === undefined || value === null) {
            throw new Error('Bounds error.');
        }
        let stringBytesLength = Buffer.byteLength(value, 'utf8');
        this.writeShort(stringBytesLength);
        if (this.offset + stringBytesLength > this.length || this.offset + stringBytesLength > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        let bytesWritten = this.buffer.write(value, this.offset);
        if (bytesWritten != stringBytesLength) {
            throw new Error('Bounds error.');
        }
        this.offset += stringBytesLength;
    }
    writeNetBuffer(srcNetBuffer, writeLen) {
        if (srcNetBuffer === undefined || srcNetBuffer === null) {
            throw new Error('Buffer error.');
        }
        this.writeBufferPart(srcNetBuffer.buffer, srcNetBuffer.offset, writeLen);
        srcNetBuffer.setOffset(srcNetBuffer.getOffset() + writeLen);
    }
    writeBuffer(srcBuffer) {
        this.writeBufferPart(srcBuffer, 0, srcBuffer.length);
    }
    writeBufferPart(srcBuffer, srcOffset, writeLen) {
        if (srcBuffer === undefined || srcBuffer === null) {
            throw new Error('Buffer error.');
        }
        if (writeLen === undefined || writeLen === null) {
            throw new Error('Buffer error.');
        }
        // Do checks on the src and this to make sure enough data and space exists.
        if (srcOffset + writeLen > srcBuffer.length) {
            throw new Error('Buffer error.');
        }
        if (this.getOffset() + writeLen > this.getBoundsLimit()) {
            throw new Error('Buffer error.');
        }
        // Copy the bytes.
        srcBuffer.copy(this.buffer, this.offset, srcOffset, srcOffset + writeLen);
        // update the src and this offset.
        this.setOffset(this.getOffset() + writeLen);
    }
    skipInt() {
        if (this.offset + 4 > this.length || this.offset + 4 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        return this.offset += 4;
    }
    getBuffer() {
        return this.buffer;
    }
    getOffset() {
        return this.offset;
    }
    setOffset(offset) {
        this.offset = offset;
    }
    getLength() {
        return this.length;
    }
    getBoundsLimit() {
        return this.boundsLimit;
    }
    setBoundsLimit(boundsLimit) {
        this.boundsLimit = boundsLimit;
        this.postBoundsLength = this.length - this.boundsLimit;
    }
    getPostBoundsLength() {
        return this.postBoundsLength;
    }
}
StreamBuffer.decoder = new string_decoder.StringDecoder('utf8');
exports.StreamBuffer = StreamBuffer;
//# sourceMappingURL=StreamBuffer.js.map