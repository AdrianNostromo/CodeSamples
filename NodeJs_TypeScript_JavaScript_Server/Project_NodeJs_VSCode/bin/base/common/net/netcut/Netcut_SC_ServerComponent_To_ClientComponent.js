"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Netcut_Component_To_Component_1 = require("./Netcut_Component_To_Component");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
const NetCodes_SC_ServerComponent_To_ClientComponent_1 = require("base/common/net/netcodes/NetCodes_SC_ServerComponent_To_ClientComponent");
const NetCodes_CS_ClientComponent_To_ServerComponent_1 = require("base/common/net/netcodes/NetCodes_CS_ClientComponent_To_ServerComponent");
class Netcut_SC_ServerComponent_To_ClientComponent extends Netcut_Component_To_Component_1.Netcut_Component_To_Component {
    constructor(app, con, componentType) {
        super(app, con, componentType, true);
        //void
    }
    create_framework() {
        super.create_framework();
        //void
    }
    logicLoop(deltaS, deltaMS) {
        super.logicLoop(deltaS, deltaMS);
        // Handle ping sending.
        this._sendIddleTimeS += deltaS;
        if (this._sendIddleTimeS >= Netcut_SC_ServerComponent_To_ClientComponent._pingSendIddleDelayS) {
            this._sendIddleTimeS = 0;
            // Send a ping.
            if (!this.sendMsg_REQUEST_PING()) {
                this.handleConError();
            }
        }
    }
    processMsg(msgLen, msgType, msgNetBuffer) {
        if (super.processMsg(msgLen, msgType, msgNetBuffer)) {
            return true;
        }
        switch (msgType) {
            case NetCodes_CS_ClientComponent_To_ServerComponent_1.NetCodes_CS_ClientComponent_To_ServerComponent.RESPONSE_PING_OK:
                //void
                break;
            default:
                return false;
        }
        return true;
    }
    sendMsg_REQUEST_PING() {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_SC_ServerComponent_To_ClientComponent_1.NetCodes_SC_ServerComponent_To_ClientComponent.REQUEST_PING);
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
Netcut_SC_ServerComponent_To_ClientComponent._pingSendIddleDelayS = 5;
exports.Netcut_SC_ServerComponent_To_ClientComponent = Netcut_SC_ServerComponent_To_ClientComponent;
//# sourceMappingURL=Netcut_SC_ServerComponent_To_ClientComponent.js.map