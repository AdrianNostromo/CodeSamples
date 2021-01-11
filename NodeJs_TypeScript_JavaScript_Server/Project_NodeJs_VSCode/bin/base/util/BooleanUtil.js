"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class BooleanUtil {
    static is(bitMap, bitIndex) {
        return (bitMap & (1 << bitIndex)) != 0;
    }
    static set(bitMap, bitIndex) {
        return bitMap | (1 << bitIndex);
    }
    static clear(bitMap, bitIndex) {
        return bitMap & (~(1 << bitIndex));
    }
    static writeBitToBitMap(destBitMap, destByteIndex, destBitIndex, srcValue, srcBitIndex) {
        // Extract the byte.
        let targetByte = destBitMap.readInt8(destByteIndex);
        // Modify the byte.
        if (BooleanUtil.is(srcValue, srcBitIndex)) {
            targetByte = BooleanUtil.set(targetByte, destBitIndex);
        }
        else {
            targetByte = BooleanUtil.clear(targetByte, destBitIndex);
        }
        // Write back the byte.
        destBitMap.writeUInt8(targetByte, destByteIndex);
    }
}
exports.BooleanUtil = BooleanUtil;
//# sourceMappingURL=BooleanUtil.js.map