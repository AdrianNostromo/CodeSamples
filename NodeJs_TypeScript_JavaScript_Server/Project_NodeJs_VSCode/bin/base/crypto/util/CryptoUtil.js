"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const random_seed = require("random-seed");
const LogicException_1 = require("base/exception/LogicException");
const Salsa20Algorithm_1 = require("../algorithm/Salsa20Algorithm");
const EncryptCipher_1 = require("../cipher/EncryptCipher");
const DecryptCipher_1 = require("../cipher/DecryptCipher");
class CryptoUtil {
    static randomHexString(len) {
        let s = '';
        for (let i = 0; i < len; i++) {
            s += CryptoUtil.HEXADECIMAL_CHARACTERS.charAt(CryptoUtil.RNG.range(CryptoUtil.HEXADECIMAL_CHARACTERS.length));
        }
        return s;
    }
    static newEncryptCipher(algorithmType, key, iv) {
        if (algorithmType == CryptoUtil.Algorithms_Salsa20) {
            if (key.byteLength != Salsa20Algorithm_1.Salsa20Algorithm.KEY_SIZE || iv.byteLength != Salsa20Algorithm_1.Salsa20Algorithm.IV_SIZE) {
                throw new LogicException_1.LogicException("Err.");
            }
            let algorithm = new Salsa20Algorithm_1.Salsa20Algorithm(key, iv);
            let cipher = new EncryptCipher_1.EncryptCipher(algorithm);
            return cipher;
        }
        throw new LogicException_1.LogicException("Err.");
    }
    static newDecryptCipher(algorithmType, key, iv) {
        if (algorithmType == CryptoUtil.Algorithms_Salsa20) {
            if (key.byteLength != Salsa20Algorithm_1.Salsa20Algorithm.KEY_SIZE || iv.byteLength != Salsa20Algorithm_1.Salsa20Algorithm.IV_SIZE) {
                throw new LogicException_1.LogicException("Err.");
            }
            let algorithm = new Salsa20Algorithm_1.Salsa20Algorithm(key, iv);
            let cipher = new DecryptCipher_1.DecryptCipher(algorithm);
            return cipher;
        }
        throw new LogicException_1.LogicException("Err.");
    }
}
CryptoUtil.Algorithms_Salsa20 = "_salsa20";
CryptoUtil.RNG = random_seed.create((new Date()).getTime().toString(8));
CryptoUtil.MD5_LENGTH = 32;
CryptoUtil.HEXADECIMAL_CHARACTERS = "0123456789ABCDEF";
exports.CryptoUtil = CryptoUtil;
//# sourceMappingURL=CryptoUtil.js.map