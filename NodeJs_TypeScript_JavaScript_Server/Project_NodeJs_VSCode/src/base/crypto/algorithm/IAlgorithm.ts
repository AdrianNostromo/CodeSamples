export interface IAlgorithm {

    processBytes(inBuf: ArrayBuffer, inBytesOffset: number, 
        outBuf: ArrayBuffer, outBytesOffset: number, 
        bytesCount: number): void;

}
