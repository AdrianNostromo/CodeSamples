"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ClientsDatastoreBase_1 = require("./ClientsDatastoreBase");
const DbAccountEntry_1 = require("./util/DbAccountEntry");
const LogicError_1 = require("base/exception/LogicError");
class ClientsDatastoreUsers extends ClientsDatastoreBase_1.ClientsDatastoreBase {
    constructor(app) {
        super(app);
        this.usersCollection = null;
        //void
    }
    factoryInitializeNewCollections(db) {
        super.factoryInitializeNewCollections(db);
        db.addCollection('users', {
            indices: ['accountId', 'socialUserId', 'nickname'],
            unique: ['accountId', 'socialUserId']
        });
    }
    onInitDbColleactions() {
        super.onInitDbColleactions();
        this.usersCollection = this.dbInstance.getCollection('users');
        if (!this.usersCollection) {
            throw new LogicError_1.LogicError("AccountsDatastoreCollection.onInitDbColleactions.1.");
        }
    }
    userEntryChanged(accEntry) {
        this.usersCollection.update(accEntry);
    }
    selectUserWithAccountId(sanitized_accountId) {
        let results = this.usersCollection.find({ 'accountId': { '$eq': sanitized_accountId } });
        if (results.length == 0) {
            return null;
        }
        else if (results.length > 1) {
            throw new LogicError_1.LogicError('AccountsDatastore.selectUserWithAccountId.1.');
        }
        return results[0];
    }
    existsUserWithAccountId(sanitized_accountId) {
        if (this.selectUserWithAccountId(sanitized_accountId) != null) {
            return true;
        }
        return false;
    }
    updateAccNicknameWithAccountId(sanitized_accountId, sanitized_nickname) {
        let entry = this.selectUserWithAccountId(sanitized_accountId);
        if (entry && entry.nickname != sanitized_nickname) {
            entry.nickname = sanitized_nickname;
            this.usersCollection.update(entry);
            this.saveDatabase();
        }
        return entry;
    }
    selectUserWithSocialUserId(sanitized_socialUserId) {
        let results = this.usersCollection.find({ 'socialUserId': { '$eq': sanitized_socialUserId } });
        if (results.length == 0) {
            return null;
        }
        else if (results.length > 1) {
            throw new LogicError_1.LogicError('AccountsDatastore.selectSingleUserWithSocialUserId.1.');
        }
        return results[0];
    }
    existsUserWithSocialUserId(sanitized_socialUserId) {
        if (this.selectUserWithSocialUserId(sanitized_socialUserId) != null) {
            return true;
        }
        return false;
    }
    insertAccount(sanitized_nickname) {
        let accountId;
        let isAccountIdFound = false;
        // Generate a account uid. Try 10 times.
        for (let i = 0; i < 50; i++) {
            // 1 000, 1 000 000 000
            let newAccountId = this.rng.intBetween(1000, 1000000000);
            if (!this.existsUserWithAccountId(newAccountId)) {
                accountId = newAccountId;
                isAccountIdFound = true;
                break;
            }
        }
        if (!isAccountIdFound) {
            throw new LogicError_1.LogicError('Error.');
        }
        let socialUserId;
        let isSocialUserIdFound = false;
        // Generate a account uid. Try 10 times.
        for (let i = 0; i < 50; i++) {
            let newSocialUserId = this.rng.intBetween(1000, 1000000000);
            if (!this.existsUserWithSocialUserId(newSocialUserId)) {
                socialUserId = newSocialUserId;
                isSocialUserIdFound = true;
                break;
            }
        }
        if (!isSocialUserIdFound) {
            throw new LogicError_1.LogicError('Error.');
        }
        let newEntry = null;
        try {
            newEntry = this.usersCollection.insert(new DbAccountEntry_1.DbAccountEntry(accountId, socialUserId, (new Date()).getTime(), sanitized_nickname));
        }
        catch (x) {
            throw x;
        }
        if (!newEntry) {
            throw new LogicError_1.LogicError('Error.');
        }
        this.saveDatabase();
        return newEntry;
    }
}
exports.ClientsDatastoreUsers = ClientsDatastoreUsers;
//# sourceMappingURL=ClientsDatastoreUsers.js.map