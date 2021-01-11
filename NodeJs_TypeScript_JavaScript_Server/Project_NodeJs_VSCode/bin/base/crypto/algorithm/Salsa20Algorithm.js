"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Salsa20_1 = require("../rawalgorithm/Salsa20");
class Salsa20Algorithm {
    constructor(key, iv) {
        this.rawAlgorithm = new Salsa20_1.Salsa20();
        this.rawAlgorithm.setKeyIv(key, iv);
    }
    processBytes(inBuf, inBytesOffset, outBuf, outBytesOffset, bytesCount) {
        this.rawAlgorithm.processBytes(inBuf, inBytesOffset, outBuf, outBytesOffset, bytesCount);
    }
}
// Values are the ones from the raw algorithm. They are used to validate the key and iv sizes;
// base::crypto::Salsa20::size_t::KEY_SIZE;
Salsa20Algorithm.KEY_SIZE = 32;
// base::crypto::Salsa20::size_t::IV_SIZE;
Salsa20Algorithm.IV_SIZE = 8;
exports.Salsa20Algorithm = Salsa20Algorithm;
//# sourceMappingURL=Salsa20Algorithm.js.map