"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LimitedCommandNetClient_1 = require("base/common/client/net/holder/util/LimitedCommandNetClient");
class QueuePlayer extends LimitedCommandNetClient_1.LimitedCommandNetClient {
    //public isMatchFound: boolean = false;
    constructor(con, accountId) {
        super(con);
        this.isMatchFindInProgress = false;
        this.accountId = accountId;
    }
}
exports.QueuePlayer = QueuePlayer;
//# sourceMappingURL=QueuePlayer.js.map