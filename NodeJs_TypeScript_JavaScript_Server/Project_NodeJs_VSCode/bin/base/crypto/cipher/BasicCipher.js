"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class BasicCipher {
    constructor(algorithm) {
        this.algorithm = algorithm;
    }
    processBytes(inBuf, inBytesOffset, outBuf, outBytesOffset, bytesCount) {
        this.algorithm.processBytes(inBuf, inBytesOffset, outBuf, outBytesOffset, bytesCount);
    }
}
exports.BasicCipher = BasicCipher;
//# sourceMappingURL=BasicCipher.js.map