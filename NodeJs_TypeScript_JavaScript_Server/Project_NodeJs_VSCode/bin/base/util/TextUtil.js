"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class TextUtil {
    static countUtf8Bytes(value) {
        let b = 0;
        let i = 0;
        let c;
        for (; c = value.charCodeAt(i++); b += c >> 11 ? 3 : c >> 7 ? 2 : 1)
            ;
        return b;
    }
}
exports.TextUtil = TextUtil;
//# sourceMappingURL=TextUtil.js.map