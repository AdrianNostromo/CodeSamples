import * as random_seed from "random-seed";
import { LogicException } from "base/exception/LogicException";
import { Salsa20Algorithm } from "../algorithm/Salsa20Algorithm";
import { EncryptCipher } from "../cipher/EncryptCipher";
import { IEncryptCipher } from "../cipher/IEncryptCipher";
import { DecryptCipher } from "../cipher/DecryptCipher";
import { IDecryptCipher } from "../cipher/IDecryptCipher";

export class CryptoUtil {

    public static Algorithms_Salsa20: string = "_salsa20"

    public static RNG = random_seed.create((new Date()).getTime().toString(8));
    
    public static MD5_LENGTH: number = 32;
    public static HEXADECIMAL_CHARACTERS: string = "0123456789ABCDEF";
  
    public static randomHexString(len: number): string {
        let s: string = '';
        for(let i: number=0;i<len;i++) {
            s += CryptoUtil.HEXADECIMAL_CHARACTERS.charAt(CryptoUtil.RNG.range(CryptoUtil.HEXADECIMAL_CHARACTERS.length));
        }
        
        return s;
    }
  
    public static newEncryptCipher(algorithmType: string, key: Uint8Array, iv: Uint8Array): IEncryptCipher {
        if(algorithmType == CryptoUtil.Algorithms_Salsa20) {
            if(key.byteLength != Salsa20Algorithm.KEY_SIZE || iv.byteLength != Salsa20Algorithm.IV_SIZE) {
                throw new LogicException("Err.");
            }

            let algorithm: Salsa20Algorithm = new Salsa20Algorithm(key, iv);
    
            let cipher: EncryptCipher = new EncryptCipher(algorithm);
    
            return cipher;
        }
    
        throw new LogicException("Err.");
    }

    public static newDecryptCipher(algorithmType: string, key: Uint8Array, iv: Uint8Array): IDecryptCipher {
        if(algorithmType == CryptoUtil.Algorithms_Salsa20) {
            if(key.byteLength != Salsa20Algorithm.KEY_SIZE || iv.byteLength != Salsa20Algorithm.IV_SIZE) {
                throw new LogicException("Err.");
            }
    
            let algorithm: Salsa20Algorithm = new Salsa20Algorithm(key, iv);
    
            let cipher: DecryptCipher = new DecryptCipher(algorithm);
    
            return cipher;
        }
    
        throw new LogicException("Err.");
    }

}
