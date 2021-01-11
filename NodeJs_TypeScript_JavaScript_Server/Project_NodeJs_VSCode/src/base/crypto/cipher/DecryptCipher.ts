import { BasicCipher } from "./BasicCipher";
import { IDecryptCipher } from "./IDecryptCipher";
import { IAlgorithm } from "../algorithm/IAlgorithm";
import { addListener } from "cluster";
import { Base64 } from "base/encoding/base64/Base64";
import { LogicError } from "base/exception/LogicError";

export class DecryptCipher extends BasicCipher implements IDecryptCipher {

    public constructor(algorithm: IAlgorithm) {
        super(algorithm);

        //void
    }

    public decryptBase64ToString(eData: string): string {
        let binaryEDataString: Buffer = Base64.decode(eData);

        this.processBytes(
            binaryEDataString.buffer, binaryEDataString.byteOffset,
            binaryEDataString.buffer, binaryEDataString.byteOffset,
            binaryEDataString.byteLength
        );

        let ret: string = binaryEDataString.toString("utf8");
        
        return ret;
    }

    public decryptBufferToInt(eData: Buffer): number {
        if(eData == null || eData.length != 4) {
            throw new LogicError('Bounds error.');
        }

        let bData: Buffer = Buffer.allocUnsafe(4);
        
        this.processBytes(
            eData.buffer, eData.byteOffset,
            bData.buffer, bData.byteOffset,
            4
        );

        let data: number = bData.readUInt32BE(0);
        
        return data;
    }

}
