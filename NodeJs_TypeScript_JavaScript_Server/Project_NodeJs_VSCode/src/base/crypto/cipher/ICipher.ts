export interface ICipher {

    processBytes(inBuf: ArrayBuffer, inBytesOffset: number, 
        outBuf: ArrayBuffer, outBytesOffset: number, 
        bytesCount: number): void;

}
