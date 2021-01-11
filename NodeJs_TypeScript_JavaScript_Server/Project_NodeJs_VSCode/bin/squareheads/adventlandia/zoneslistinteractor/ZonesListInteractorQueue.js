"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ZonesListInteractorQueueBase_1 = require("./ZonesListInteractorQueueBase");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
class NetCodes {
}
NetCodes.SC_C_MATCH_FIND_STATUS_ON = 200020;
NetCodes.CS_R_MATCH_FIND_STATUS_ON_OK = 200021;
NetCodes.SC_C_MATCH_FIND_FINISHED = 200030;
// int serverInstanceId
// string serverIp
// int serverPort
// string scenarioId
NetCodes.CS_R_MATCH_FIND_FINISHED_OK = 200031;
class ZonesListInteractorQueue extends ZonesListInteractorQueueBase_1.ZonesListInteractorQueueBase {
    constructor(app) {
        super(app);
        this._ILoopUserBase_General = true;
        this.queueCheckDelayS = 0;
        //void
    }
    onQueuePlayerAdded(pl) {
        super.onQueuePlayerAdded(pl);
        // Get the player session id.
        this.sendMsg_SC_C_MATCH_FIND_STATUS_ON(pl);
    }
    onProcessQueueClientMsg(pl, msgLen, msgType, msgNetBuffer) {
        if (super.onProcessQueueClientMsg(pl, msgLen, msgType, msgNetBuffer)) {
            return true;
        }
        switch (msgType) {
            case NetCodes.CS_R_MATCH_FIND_STATUS_ON_OK:
                if (pl.getCurrentCommandNetCode() != NetCodes.SC_C_MATCH_FIND_STATUS_ON) {
                    throw new Error('Security warning.');
                }
                pl.clearCurrentCommandNetCode();
                pl.isMatchFindInProgress = true;
                break;
            case NetCodes.CS_R_MATCH_FIND_FINISHED_OK:
                if (pl.getCurrentCommandNetCode() != NetCodes.SC_C_MATCH_FIND_FINISHED) {
                    throw new Error('Security warning.');
                }
                pl.clearCurrentCommandNetCode();
                // Con finished.
                pl.markForRemoval = true;
                break;
            default:
                return false;
        }
        return true;
    }
    sendMsg_SC_C_MATCH_FIND_STATUS_ON(pl) {
        pl.setCurrentCommandNetCode(NetCodes.SC_C_MATCH_FIND_STATUS_ON);
        pl.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4)
            .startMsgWrite()
            .writeInt(NetCodes.SC_C_MATCH_FIND_STATUS_ON)
            .endMsgWrite());
    }
    appLoop_General(deltaS, deltaMS) {
        this.appLoop_queue(deltaS, deltaMS);
    }
    appLoop_queue(deltaS, deltaMS) {
        this.queueCheckDelayS -= deltaS;
        if (this.queueCheckDelayS < 0) {
            this.queueCheckDelayS = ZonesListInteractorQueue.QUEUE_CHECK_DELAY_S;
            let playersList = this.netClientsHolder.getClientsList();
            // Need to enumerate from the start, so the older players have a higherr priority.
            for (let i = 0; i < playersList.length; i++) {
                let pl = playersList[i];
                if (pl != null && pl.isMatchFindInProgress && pl.getCurrentCommandNetCode() < 0) {
                    // Get the first scenario server that is optimally crowded.
                    let scenarioServer = this.getApp().getSquareHeads().getAdventLandia().getZonesListManager().getOrCreateOptimallyJoinableScearioServer();
                    if (scenarioServer == null || !scenarioServer.getIsInitFinished()) {
                        break;
                    }
                    scenarioServer.incrementPossibleNewPlayersCount();
                    let scenarioServerIpPort = scenarioServer.getPlayerServerIpPort();
                    this.sendMsg_SC_C_MATCH_FIND_FINISHED(pl, scenarioServer.getServerInstanceId(), scenarioServerIpPort.ip, scenarioServerIpPort.port, scenarioServer.getScenarioId());
                }
            }
        }
    }
    // int serverInstanceId
    // string serverIp
    // int serverPort
    sendMsg_SC_C_MATCH_FIND_FINISHED(pl, serverInstanceId, serverIp, serverPort, scenarioId) {
        pl.setCurrentCommandNetCode(NetCodes.SC_C_MATCH_FIND_FINISHED);
        pl.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4
            + 4
            + NetMsgsBlock_1.NetMsgsBlock.stringInBufferSize(serverIp)
            + 4)
            .startMsgWrite()
            .writeInt(NetCodes.SC_C_MATCH_FIND_FINISHED)
            .writeInt(serverInstanceId)
            .writeString(serverIp)
            .writeInt(serverPort)
            .writeString(scenarioId)
            .endMsgWrite());
    }
}
ZonesListInteractorQueue.QUEUE_CHECK_DELAY_S = 1.5;
exports.ZonesListInteractorQueue = ZonesListInteractorQueue;
//# sourceMappingURL=ZonesListInteractorQueue.js.map