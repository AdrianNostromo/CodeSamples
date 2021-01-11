"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ZonesListInteractorNetValidator_1 = require("./ZonesListInteractorNetValidator");
const QueuePlayer_1 = require("./util/QueuePlayer");
const NetClientsHolder_1 = require("base/common/client/net/holder/NetClientsHolder");
class ZonesListInteractorQueueBase extends ZonesListInteractorNetValidator_1.ZonesListInteractorNetValidator {
    constructor(app) {
        super(app);
        this.netClientsHolder = null;
        //void
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.netClientsHolder = new NetClientsHolder_1.NetClientsHolder(null, //this.onNewClient.bind(this),
        null, //this.onClientRemoved_pre.bind(this),
        this.onProcessQueueClientMsg.bind(this)));
        this.netClientsHolder.iddleDataReceiveThresholdS = ZonesListInteractorQueueBase.IDDLE_CONNECTION_THRESHOLD_S;
        this.netClientsHolder.interPingDelayS = ZonesListInteractorQueueBase.INTER_PING_DELAY_S;
    }
    onNetClientValidationSuccess(accountId, con) {
        // Enable auto crypto.
        con.enabledAutoCryptoRead();
        con.enabledAutoCryptoWrite();
        let pl = new QueuePlayer_1.QueuePlayer(con, accountId);
        this.netClientsHolder.addClient(pl);
        pl.isPingAllowed = true;
        this.onQueuePlayerAdded(pl);
    }
    onQueuePlayerAdded(pl) {
        //void
    }
    onProcessQueueClientMsg(pl, msgLen, msgType, msgNetBuffer) {
        return false;
    }
}
ZonesListInteractorQueueBase.IDDLE_CONNECTION_THRESHOLD_S = 5;
ZonesListInteractorQueueBase.INTER_PING_DELAY_S = 1;
exports.ZonesListInteractorQueueBase = ZonesListInteractorQueueBase;
//# sourceMappingURL=ZonesListInteractorQueueBase.js.map