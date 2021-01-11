"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class Base64 {
    static encode(srcBuf) {
        let base64Value = srcBuf.toString("base64");
        return base64Value;
    }
    static decode(srcBase64) {
        let bytesValue = Buffer.from(srcBase64, "base64");
        return bytesValue;
    }
}
exports.Base64 = Base64;
//# sourceMappingURL=Base64.js.map