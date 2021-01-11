"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ClientsDatastoreConnectedDevices_1 = require("./ClientsDatastoreConnectedDevices");
const DbClientSessionEntry_1 = require("./util/DbClientSessionEntry");
const LogicError_1 = require("base/exception/LogicError");
const CryptoUtil_1 = require("base/crypto/util/CryptoUtil");
class ClientsDatastoreSessions extends ClientsDatastoreConnectedDevices_1.ClientsDatastoreConnectedDevices {
    constructor(app) {
        super(app);
        this.sessionsCollection = null;
        //void
    }
    factoryInitializeNewCollections(db) {
        super.factoryInitializeNewCollections(db);
        db.addCollection('sessions', {
            indices: ['sessionId'],
            unique: ['sessionId']
        });
    }
    onInitDbColleactions() {
        super.onInitDbColleactions();
        this.sessionsCollection = this.dbInstance.getCollection('sessions');
        if (!this.sessionsCollection) {
            throw new LogicError_1.LogicError("AccountsDatastoreCollection.onInitDbColleactions.3.");
        }
    }
    insertSession(accountId) {
        let accEntry = this.selectUserWithAccountId(accountId);
        if (accEntry == null) {
            throw new LogicError_1.LogicError('Error.');
        }
        if (accEntry.activeSession != null) {
            throw new LogicError_1.LogicError('Error.');
        }
        // Generate the random entry variables.
        let sessionId;
        let isSessionIdFound = false;
        // Generate a account uid. Try 10 times.
        for (let i = 0; i < 50; i++) {
            // 1 000, 1 000 000 000
            let newId = this.rng.intBetween(1000, 1000000000);
            if (!this.existsSessionWithSessionId(newId)) {
                sessionId = newId;
                isSessionIdFound = true;
                break;
            }
        }
        if (!isSessionIdFound) {
            throw new LogicError_1.LogicError('Error.');
        }
        let sessionKey = CryptoUtil_1.CryptoUtil.randomHexString(ClientsDatastoreSessions.SESSION_KEY_STRING_LENGTH);
        // Push the session entry.
        let newEntry;
        try {
            newEntry = this.sessionsCollection.insert(new DbClientSessionEntry_1.DbClientSessionEntry(accEntry.accountId, sessionId, sessionKey));
        }
        catch (x) {
            throw x;
        }
        // Also update the client account entry.
        accEntry.activeSession = newEntry;
        this.userEntryChanged(accEntry);
        this.saveDatabase();
        return newEntry;
    }
    selectSessionWithSessionId(sessionId) {
        let results = this.sessionsCollection.find({ 'sessionId': { '$eq': sessionId } });
        if (results.length == 0) {
            return null;
        }
        else if (results.length > 1) {
            throw new LogicError_1.LogicError('AccountsDatastore.selectSessionWithSessionId.1.');
        }
        return results[0];
    }
    existsSessionWithSessionId(sessionId) {
        if (this.selectSessionWithSessionId(sessionId) != null) {
            return true;
        }
        return false;
    }
}
ClientsDatastoreSessions.SESSION_KEY_STRING_LENGTH = 32;
exports.ClientsDatastoreSessions = ClientsDatastoreSessions;
//# sourceMappingURL=ClientsDatastoreSessions.js.map