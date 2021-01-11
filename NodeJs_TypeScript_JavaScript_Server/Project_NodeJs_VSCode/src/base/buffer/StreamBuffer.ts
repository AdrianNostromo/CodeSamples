import * as string_decoder from "string_decoder";

import { TextUtil } from "base/util/TextUtil";

export class StreamBuffer {

    public static stringInBufferSize(value: string): number {
        return 2 + TextUtil.countUtf8Bytes(value);
    }
    
    private static decoder: string_decoder.NodeStringDecoder = new string_decoder.StringDecoder('utf8');
    
    private buffer: Buffer;
    
    private offset: number;
    protected length: number;
    private boundsLimit: number;
    private postBoundsLength: number;
    
    public constructor(buffer: Buffer) {
        this.setBuffer(buffer);
    }
    
    public setBuffer(buffer: Buffer): void {
        this.buffer = buffer;
        
        this.offset = 0;
        
        this.length = this.buffer.length;
        this.boundsLimit = this.length;
        
        this.postBoundsLength = this.length - this.boundsLimit;
    }
    
    public readInt(): number {
        if(this.offset + 4 > this.length || this.offset + 4 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        
        let ret: number = this.buffer.readInt32BE(this.offset);
        this.offset += 4;
        
        return ret;
    }
    
    public readByte(): number {
        if(this.offset + 1 > this.length || this.offset + 1 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        
        // NOTE : This is a unsigned byte. The client uses signed bytes.
        let ret: number = this.buffer.readUInt8(this.offset);
        this.offset += 1;
        
        return ret;
    }
    
    public readFloat(): number {
        if(this.offset + 4 > this.length || this.offset + 4 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        
        let ret: number = this.buffer.readFloatBE(this.offset);
        this.offset += 4;
        
        return ret;
    }
    
    public readShort(): number {
        if(this.offset + 2 > this.length || this.offset + 2 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        
        let ret: number = this.buffer.readInt16BE(this.offset);
        this.offset += 2;
        
        return ret;
    }
    
    public readBoolean(): boolean {
        let ret: number = this.readByte();
        
        let boolRet: boolean;
        if(ret == 0) {
            boolRet = false;
        }else if(ret == 1) {
            boolRet = true;
        }else {
            throw new Error('Data error.');
        }
        
        return boolRet;
    }
    
    public readString(): string {
        let stringBytesLength: number = this.readShort();
        
        let stringDataBuffer: Buffer = this.readBuffer(stringBytesLength);
        
        let ret: string = StreamBuffer.decoder.end(stringDataBuffer);
        
        return ret;
    }
    
    public readBuffer(len: number): Buffer {
        if(this.offset + len > this.length || this.offset + len > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        
        let ret: Buffer = Buffer.allocUnsafe(len);
        this.buffer.copy(ret, 0, this.offset, this.offset + len);
        this.offset += len;
    
        return ret;
    }
    
    public peekInt(): number {
        if(this.offset + 4 > this.length || this.offset + 4 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        
        let ret: number = this.buffer.readInt32BE(this.offset);
        //this.offset += 4;
        
        return ret;
    }
    
    public peekIntUnsafe(extraOffset: number): number {
        let finalOffset: number = this.offset;
        if(extraOffset !== undefined && extraOffset !== null) {
            finalOffset += extraOffset;
        }
        
        if(finalOffset + 4 > this.length) {
            throw new Error('Bounds error.');
        }
        
        let ret: number = this.buffer.readInt32BE(finalOffset);
        
        return ret;
    }
    
    public writeInt(value: number): void {
        if(value === undefined || value === null) {
            throw new Error('Bounds error.');
        }
        
        if(this.offset + 4 > this.length || this.offset + 4 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        
        this.buffer.writeUInt32BE(value, this.offset);
        this.offset += 4;
    }
    
    public writeByte(value: number): void {
        if(value === undefined || value === null) {
            throw new Error('Bounds error.');
        }
        
        if(this.offset + 1 > this.length || this.offset + 1 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        
        // NOTE : This is a unsigned byte. The client uses signed bytes.
        this.buffer.writeUInt8(value, this.offset);
        this.offset += 1;
    }
    
    public writeFloat(value: number): void {
        if(value === undefined || value === null) {
            throw new Error('Bounds error.');
        }
        
        if(this.offset + 4 > this.length || this.offset + 4 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        
        this.buffer.writeFloatBE(value, this.offset);
        this.offset += 4;
    }
    
    public writeShort(value: number): void {
        if(value === undefined || value === null) {
            throw new Error('Bounds error.');
        }
        
        if(this.offset + 2 > this.length || this.offset + 2 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        
        this.buffer.writeUInt16BE(value, this.offset);
        this.offset += 2;
    }
    
    public writeBoolean(value: boolean): void {
        let finalVal: number;
        if(value === true) {
            finalVal = 1;
        }else if(value === false) {
            finalVal = 0;
        }else {
            throw new Error('Data error.');
        }
        
        this.writeByte(finalVal);
    }
    
    // This method is fully overridden when crypto is used on a above layer.
    public writeString(value: string): void {
        if(value === undefined || value === null) {
            throw new Error('Bounds error.');
        }
        
        let stringBytesLength: number = Buffer.byteLength(value, 'utf8');
        
        this.writeShort(stringBytesLength);
        
        if(this.offset + stringBytesLength > this.length || this.offset + stringBytesLength > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        
        let bytesWritten: number = this.buffer.write(value, this.offset);
        if(bytesWritten != stringBytesLength) {
            throw new Error('Bounds error.');
        }
        
        this.offset += stringBytesLength;
    }
    
    public writeNetBuffer(srcNetBuffer: StreamBuffer, writeLen: number): void {
        if(srcNetBuffer === undefined || srcNetBuffer === null) {
            throw new Error('Buffer error.');
        }
        
        this.writeBufferPart(srcNetBuffer.buffer, srcNetBuffer.offset, writeLen);
        srcNetBuffer.setOffset(srcNetBuffer.getOffset() + writeLen);
    }
    
    public writeBuffer(srcBuffer: Buffer): void {
        this.writeBufferPart(srcBuffer, 0, srcBuffer.length);
    }
    
    public writeBufferPart(srcBuffer: Buffer, srcOffset: number, writeLen: number): void {
        if(srcBuffer === undefined || srcBuffer === null) {
            throw new Error('Buffer error.');
        }
        if(writeLen === undefined || writeLen === null) {
            throw new Error('Buffer error.');
        }
        
        // Do checks on the src and this to make sure enough data and space exists.
        if(srcOffset + writeLen > srcBuffer.length) {
            throw new Error('Buffer error.');
        }
        if(this.getOffset() + writeLen > this.getBoundsLimit()) {
            throw new Error('Buffer error.');
        }
        
        // Copy the bytes.
        srcBuffer.copy(this.buffer, this.offset, srcOffset, srcOffset + writeLen);
        
        // update the src and this offset.
        this.setOffset(this.getOffset() + writeLen);
    }
    
    public skipInt(): number {
        if(this.offset + 4 > this.length || this.offset + 4 > this.boundsLimit) {
            throw new Error('Bounds error.');
        }
        
        return this.offset += 4;
    }
    
    public getBuffer(): Buffer {
        return this.buffer;
    }
    
    public getOffset(): number {
        return this.offset;
    }
    
    public setOffset(offset: number): void {
        this.offset = offset;
    }
    
    public getLength(): number {
        return this.length;
    }
    
    public getBoundsLimit(): number {
        return this.boundsLimit;
    }
    
    public setBoundsLimit(boundsLimit: number): void {
        this.boundsLimit = boundsLimit;
        
        this.postBoundsLength = this.length - this.boundsLimit;
    }
    
    public getPostBoundsLength(): number {
        return this.postBoundsLength;
    }
    
}
    