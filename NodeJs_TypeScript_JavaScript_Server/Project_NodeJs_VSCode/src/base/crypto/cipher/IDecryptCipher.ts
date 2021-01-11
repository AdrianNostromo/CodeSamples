import { ICipher } from "./ICipher";

export interface IDecryptCipher extends ICipher {

    decryptBase64ToString(eData: string): string;

}
