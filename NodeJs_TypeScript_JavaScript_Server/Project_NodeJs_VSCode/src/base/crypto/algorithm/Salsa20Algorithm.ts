import { IAlgorithm } from "./IAlgorithm";
import { Salsa20 } from "../rawalgorithm/Salsa20";

export class Salsa20Algorithm implements IAlgorithm {

    // Values are the ones from the raw algorithm. They are used to validate the key and iv sizes;
    // base::crypto::Salsa20::size_t::KEY_SIZE;
    public static readonly KEY_SIZE = 32;
    // base::crypto::Salsa20::size_t::IV_SIZE;
    public static readonly IV_SIZE = 8;
    
    private rawAlgorithm: Salsa20;

    public constructor(key: Uint8Array, iv: Uint8Array) {
        this.rawAlgorithm = new Salsa20();
        this.rawAlgorithm.setKeyIv(key, iv);
    }
    
    public processBytes(inBuf: ArrayBuffer, inBytesOffset: number, 
        outBuf: ArrayBuffer, outBytesOffset: number, 
        bytesCount: number): void
    {
        this.rawAlgorithm.processBytes(
            inBuf, inBytesOffset, 
            outBuf, outBytesOffset, 
            bytesCount
        );
    }

}
