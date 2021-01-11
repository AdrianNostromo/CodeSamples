import { ICipher } from "./ICipher";

export interface IEncryptCipher extends ICipher {

    encryptIntToBuffer(data: number): Buffer;
    encryptStringToBase64(data: string): string;

}
