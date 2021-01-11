"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ClientsDatastoreUsers_1 = require("./ClientsDatastoreUsers");
const DbConnectedDeviceEntry_1 = require("./util/DbConnectedDeviceEntry");
const LogicError_1 = require("base/exception/LogicError");
const CryptoUtil_1 = require("base/crypto/util/CryptoUtil");
class ClientsDatastoreConnectedDevices extends ClientsDatastoreUsers_1.ClientsDatastoreUsers {
    constructor(app) {
        super(app);
        this.connectedDevicesCollection = null;
        //void
    }
    factoryInitializeNewCollections(db) {
        super.factoryInitializeNewCollections(db);
        db.addCollection('connected_devices', {
            indices: ['deivceId'],
            unique: ['deivceId']
        });
    }
    onInitDbColleactions() {
        super.onInitDbColleactions();
        this.connectedDevicesCollection = this.dbInstance.getCollection('connected_devices');
        if (!this.connectedDevicesCollection) {
            throw new LogicError_1.LogicError("AccountsDatastoreCollection.onInitDbColleactions.2.");
        }
    }
    insertConnectedDevice(accountId) {
        let accEntry = this.selectUserWithAccountId(accountId);
        if (accEntry == null) {
            throw new LogicError_1.LogicError('Error.');
        }
        // Generate the random entry variables.
        let deviceId;
        let isDeviceIdFound = false;
        // Generate a account uid. Try 10 times.
        for (let i = 0; i < 50; i++) {
            // 1 000, 1 000 000 000
            let newDeviceId = this.rng.intBetween(1000, 1000000000);
            if (!this.existsConnectedDeviceWithDeviceId(newDeviceId)) {
                deviceId = newDeviceId;
                isDeviceIdFound = true;
                break;
            }
        }
        if (!isDeviceIdFound) {
            throw new LogicError_1.LogicError('Error.');
        }
        let deviceKey = CryptoUtil_1.CryptoUtil.randomHexString(ClientsDatastoreConnectedDevices.DEVICE_KEY_STRING_LENGTH);
        // Push the device entry.
        let newEntry;
        try {
            newEntry = this.connectedDevicesCollection.insert(new DbConnectedDeviceEntry_1.DbConnectedDeviceEntry(accEntry.accountId, deviceId, deviceKey));
        }
        catch (x) {
            throw x;
        }
        // Also update the client account connectedDevicesList.
        accEntry.connectedDevicesList.push(newEntry);
        this.userEntryChanged(accEntry);
        this.saveDatabase();
        return newEntry;
    }
    selectConnectedDeviceWithDeviceId(deviceId) {
        let results = this.connectedDevicesCollection.find({ 'deivceId': { '$eq': deviceId } });
        if (results.length == 0) {
            return null;
        }
        else if (results.length > 1) {
            throw new LogicError_1.LogicError('AccountsDatastore.selectConnectedDeviceWithDeviceId.1.');
        }
        return results[0];
    }
    existsConnectedDeviceWithDeviceId(deviceId) {
        if (this.selectConnectedDeviceWithDeviceId(deviceId) != null) {
            return true;
        }
        return false;
    }
}
ClientsDatastoreConnectedDevices.DEVICE_KEY_STRING_LENGTH = 32;
exports.ClientsDatastoreConnectedDevices = ClientsDatastoreConnectedDevices;
//# sourceMappingURL=ClientsDatastoreConnectedDevices.js.map