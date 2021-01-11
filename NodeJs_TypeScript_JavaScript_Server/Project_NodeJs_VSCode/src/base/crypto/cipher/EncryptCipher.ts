import { BasicCipher } from "./BasicCipher";
import { IEncryptCipher } from "./IEncryptCipher";
import { Base64 } from "base/encoding/base64/Base64";
import { IAlgorithm } from "../algorithm/IAlgorithm";

export class EncryptCipher extends BasicCipher implements IEncryptCipher {

    public constructor(algorithm: IAlgorithm) {
        super(algorithm);

        //void
    }

    public encryptIntToBuffer(data: number): Buffer {
        let bData: Buffer = Buffer.allocUnsafe(4);
        bData.writeInt32BE(data, 0);

        this.processBytes(
            bData.buffer, bData.byteOffset,
            bData.buffer, bData.byteOffset,
            bData.byteLength
        );

        return bData;
    }

    public encryptStringToBase64(data: string): string {
        let bData: Buffer = Buffer.from(data, "utf8");

        this.processBytes(
            bData.buffer, bData.byteOffset,
            bData.buffer, bData.byteOffset,
            bData.byteLength
        );

        let ret: string = Base64.encode(bData);

        return ret;
    }

}
