"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWIEntities_1 = require("./GWIEntities");
const GlobalAccessLogger_1 = require("base/log/GlobalAccessLogger");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
class NetCodes {
}
NetCodes.SC_C_SINK_GAME_WORLD_STATE = 400010;
// snapshotDeltaIndex: number;
// updateBufferLength: number;
// updateBuffer: Buffer;
NetCodes.CS_R_SINK_GAME_WORLD_STATE_OK = 400011;
class GWISyncNetHandling extends GWIEntities_1.GWIEntities {
    constructor(handler, config) {
        super(handler, config);
        //void
    }
    processPlayerMsg(pl, client, msgLen, msgType, msgNetBuffer) {
        if (super.processPlayerMsg(pl, client, msgLen, msgType, msgNetBuffer)) {
            return true;
        }
        switch (msgType) {
            case NetCodes.CS_R_SINK_GAME_WORLD_STATE_OK:
                this.processMsg_CS_R_SINK_GAME_WORLD_STATE_OK(pl, client, msgLen, msgType, msgNetBuffer);
                break;
            default:
                return false;
        }
        return true;
    }
    processMsg_CS_R_SINK_GAME_WORLD_STATE_OK(pl, client, msgLen, msgType, msgNetBuffer) {
        if (client.activeSyncCommands <= 0) {
            throw new Error('Security warning.');
        }
        client.activeSyncCommands--;
        //void
    }
    sendMsg_SC_C_SINK_GAME_WORLD_STATE(client, snapshotDeltaIndex, updateBuffer) {
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug("Client sync msg. snapshotDeltaIndex: " + snapshotDeltaIndex + ", updateBuffer.length: " + updateBuffer.length + ", client.remotePort" + client.remotePort);
        client.activeSyncCommands++;
        client.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4
            + 4
            + 4
            + updateBuffer.length)
            .startMsgWrite()
            .writeInt(NetCodes.SC_C_SINK_GAME_WORLD_STATE)
            .writeInt(snapshotDeltaIndex)
            .writeInt(updateBuffer.length)
            .writeBuffer(updateBuffer)
            .endMsgWrite());
    }
}
exports.GWISyncNetHandling = GWISyncNetHandling;
//# sourceMappingURL=GWISyncNetHandling.js.map