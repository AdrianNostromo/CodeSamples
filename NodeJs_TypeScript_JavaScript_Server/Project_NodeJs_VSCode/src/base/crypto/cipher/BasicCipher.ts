import { ICipher } from "./ICipher";
import { IAlgorithm } from "../algorithm/IAlgorithm";

export class BasicCipher implements ICipher {

    private algorithm: IAlgorithm;

    public constructor(algorithm: IAlgorithm) {
        this.algorithm = algorithm;
    }

    public processBytes(inBuf: ArrayBuffer, inBytesOffset: number, 
        outBuf: ArrayBuffer, outBytesOffset: number, 
        bytesCount: number): void 
    {
        this.algorithm.processBytes(
            inBuf, inBytesOffset, 
            outBuf, outBytesOffset, 
            bytesCount
        );
    }
    
}
