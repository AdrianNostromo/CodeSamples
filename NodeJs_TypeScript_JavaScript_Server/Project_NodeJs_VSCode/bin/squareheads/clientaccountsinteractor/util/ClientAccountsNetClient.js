"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LimitedCommandNetClient_1 = require("base/common/client/net/holder/util/LimitedCommandNetClient");
class ClientAccountsNetClient extends LimitedCommandNetClient_1.LimitedCommandNetClient {
    constructor(con) {
        super(con);
        this.deviceTestString = null;
        //void
    }
}
exports.ClientAccountsNetClient = ClientAccountsNetClient;
//# sourceMappingURL=ClientAccountsNetClient.js.map