"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWIPlayerActions_1 = require("./GWIPlayerActions");
const SecurityWarning_1 = require("base/exception/SecurityWarning");
class NetCodes {
}
NetCodes.CS_C_HandlePlayerActions = 500010;
// actionsCount: number;
// actionsBuffer: Buffer;
NetCodes.SC_R_HandlePlayerActions_Ok = 500011;
class GWIPlayerActionsNetHandling extends GWIPlayerActions_1.GWIPlayerActions {
    constructor(handler, config) {
        super(handler, config);
        //void
    }
    processPlayerMsg(pl, client, msgLen, msgType, msgNetBuffer) {
        if (super.processPlayerMsg(pl, client, msgLen, msgType, msgNetBuffer)) {
            return true;
        }
        switch (msgType) {
            case NetCodes.CS_C_HandlePlayerActions:
                this.processMsg_CS_C_HandlePlayerActions(pl, client, msgLen, msgType, msgNetBuffer);
                break;
            default:
                return false;
        }
        return true;
    }
    processMsg_CS_C_HandlePlayerActions(pl, client, msgLen, msgType, msgNetBuffer) {
        let actionsCount = msgNetBuffer.readInt();
        for (let i = 0; i < actionsCount; i++) {
            let action = this.processMsgPart_readPlayerActionEntry(msgNetBuffer);
            if (action == null) {
                throw new SecurityWarning_1.SecurityWarning("Logged security warning.");
            }
            // Save the action to a list.
            this.pushPlayerAction(pl, action);
        }
    }
}
exports.GWIPlayerActionsNetHandling = GWIPlayerActionsNetHandling;
//# sourceMappingURL=GWIPlayerActionsNetHandling.js.map