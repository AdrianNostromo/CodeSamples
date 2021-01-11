"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ClientSessionsManagerBase_1 = require("./ClientSessionsManagerBase");
const ClientSessionInfo_1 = require("./util/ClientSessionInfo");
class ClientSessionsManager extends ClientSessionsManagerBase_1.ClientSessionsManagerBase {
    constructor(app) {
        super(app);
        //void
    }
    getOrCreateClientSessionWithAccId(accountId) {
        // Get the account entry.
        let accEntry = this.getApp().getSquareHeads().getClientsDatastore().selectUserWithAccountId(accountId);
        if (accEntry.activeSession == null) {
            // If no session, create one.
            this.getApp().getSquareHeads().getClientsDatastore().insertSession(accountId);
        }
        let clientSessionEntry = accEntry.activeSession;
        let ret = new ClientSessionInfo_1.ClientSessionInfo(clientSessionEntry.sessionId, clientSessionEntry.sessionKey, clientSessionEntry.accountId);
        return ret;
    }
}
exports.ClientSessionsManager = ClientSessionsManager;
//# sourceMappingURL=ClientSessionsManager.js.map