"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Netcut_Component_To_Component_1 = require("./Netcut_Component_To_Component");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
const NetCodes_SC_ServerComponent_To_ClientComponent_1 = require("base/common/net/netcodes/NetCodes_SC_ServerComponent_To_ClientComponent");
const NetCodes_CS_ClientComponent_To_ServerComponent_1 = require("base/common/net/netcodes/NetCodes_CS_ClientComponent_To_ServerComponent");
class Netcut_CS_ClientComponent_To_ServerComponent extends Netcut_Component_To_Component_1.Netcut_Component_To_Component {
    constructor(app, con, componentType) {
        super(app, con, componentType, false);
        //void
    }
    create_framework() {
        super.create_framework();
        //void
    }
    processMsg(msgLen, msgType, msgNetBuffer) {
        if (super.processMsg(msgLen, msgType, msgNetBuffer)) {
            return true;
        }
        switch (msgType) {
            case NetCodes_SC_ServerComponent_To_ClientComponent_1.NetCodes_SC_ServerComponent_To_ClientComponent.REQUEST_PING:
                if (!this.sendMsg_RESPONSE_PING_OK()) {
                    this.getApp().getLog().warning('Ping response received from a server connection. Maybe a logic error on the remote component.');
                    return false;
                }
                break;
            default:
                return false;
        }
        return true;
    }
    sendMsg_RESPONSE_PING_OK() {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_CS_ClientComponent_To_ServerComponent_1.NetCodes_CS_ClientComponent_To_ServerComponent.RESPONSE_PING_OK);
            // END : msg data;
            netBlock.endMsgWrite();
            this.sendNetBlock(netBlock);
        }
        catch (err) {
            return false;
        }
        return true;
    }
}
exports.Netcut_CS_ClientComponent_To_ServerComponent = Netcut_CS_ClientComponent_To_ServerComponent;
//# sourceMappingURL=Netcut_CS_ClientComponent_To_ServerComponent.js.map