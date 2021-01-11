"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const NetClientsHolderList_1 = require("./NetClientsHolderList");
class NetClientsHolder extends NetClientsHolderList_1.NetClientsHolderList {
    constructor(cbNewClient, cbClientRemoved_pre, cbProcessMsg) {
        super(cbNewClient, cbClientRemoved_pre, cbProcessMsg);
        //void
    }
}
exports.NetClientsHolder = NetClientsHolder;
//# sourceMappingURL=NetClientsHolder.js.map