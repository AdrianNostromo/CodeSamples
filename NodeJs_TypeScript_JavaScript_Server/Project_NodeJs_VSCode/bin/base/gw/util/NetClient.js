"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LimitedCommandNetClient_1 = require("base/common/client/net/holder/util/LimitedCommandNetClient");
class NetClient extends LimitedCommandNetClient_1.LimitedCommandNetClient {
    constructor(con, accountId) {
        super(con);
        this.player = null;
        this.lastSyncDeltaIndex = 0;
        this.activeSyncCommands = 0;
        this.accountId = accountId;
    }
}
exports.NetClient = NetClient;
//# sourceMappingURL=NetClient.js.map