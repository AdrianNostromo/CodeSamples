"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LimitedCommandNetClient_1 = require("base/common/client/net/holder/util/LimitedCommandNetClient");
class UnvalidatedClient extends LimitedCommandNetClient_1.LimitedCommandNetClient {
    constructor(con) {
        super(con);
        this.posValidationAccountId = -1;
        this.sessionTestString = null;
        //void
    }
}
exports.UnvalidatedClient = UnvalidatedClient;
//# sourceMappingURL=UnvalidatedClient.js.map