"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicCipher_1 = require("./BasicCipher");
const Base64_1 = require("base/encoding/base64/Base64");
class EncryptCipher extends BasicCipher_1.BasicCipher {
    constructor(algorithm) {
        super(algorithm);
        //void
    }
    encryptIntToBuffer(data) {
        let bData = Buffer.allocUnsafe(4);
        bData.writeInt32BE(data, 0);
        this.processBytes(bData.buffer, bData.byteOffset, bData.buffer, bData.byteOffset, bData.byteLength);
        return bData;
    }
    encryptStringToBase64(data) {
        let bData = Buffer.from(data, "utf8");
        this.processBytes(bData.buffer, bData.byteOffset, bData.buffer, bData.byteOffset, bData.byteLength);
        let ret = Base64_1.Base64.encode(bData);
        return ret;
    }
}
exports.EncryptCipher = EncryptCipher;
//# sourceMappingURL=EncryptCipher.js.map