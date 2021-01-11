"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
class NetUtil {
    constructor() {
        //void
    }
    static extractParamValue(paramName, url) {
        let startIndex;
        let endIndex;
        startIndex = url.indexOf(paramName + '=');
        if (startIndex < 0) {
            return null;
        }
        startIndex += paramName.length + 1;
        endIndex = url.indexOf('&', startIndex);
        if (endIndex < 0) {
            endIndex = url.length;
        }
        let s = url.substring(startIndex, endIndex);
        return s;
    }
    static sendSimpleNetMsg(con, netMsgCode) {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(netMsgCode);
            // END : msg data;
            netBlock.endMsgWrite();
            con.sendBlock(netBlock);
        }
        catch (x) {
            throw x;
        }
    }
}
exports.NetUtil = NetUtil;
//# sourceMappingURL=NetUtil.js.map