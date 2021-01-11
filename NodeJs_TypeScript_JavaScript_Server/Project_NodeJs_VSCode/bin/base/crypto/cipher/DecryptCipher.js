"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicCipher_1 = require("./BasicCipher");
const Base64_1 = require("base/encoding/base64/Base64");
const LogicError_1 = require("base/exception/LogicError");
class DecryptCipher extends BasicCipher_1.BasicCipher {
    constructor(algorithm) {
        super(algorithm);
        //void
    }
    decryptBase64ToString(eData) {
        let binaryEDataString = Base64_1.Base64.decode(eData);
        this.processBytes(binaryEDataString.buffer, binaryEDataString.byteOffset, binaryEDataString.buffer, binaryEDataString.byteOffset, binaryEDataString.byteLength);
        let ret = binaryEDataString.toString("utf8");
        return ret;
    }
    decryptBufferToInt(eData) {
        if (eData == null || eData.length != 4) {
            throw new LogicError_1.LogicError('Bounds error.');
        }
        let bData = Buffer.allocUnsafe(4);
        this.processBytes(eData.buffer, eData.byteOffset, bData.buffer, bData.byteOffset, 4);
        let data = bData.readUInt32BE(0);
        return data;
    }
}
exports.DecryptCipher = DecryptCipher;
//# sourceMappingURL=DecryptCipher.js.map