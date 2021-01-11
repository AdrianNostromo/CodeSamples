"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ClientDevicesManagerBase_1 = require("./ClientDevicesManagerBase");
const ConnectedClientDeviceInfo_1 = require("./util/ConnectedClientDeviceInfo");
class ClientDevicesManager extends ClientDevicesManagerBase_1.ClientDevicesManagerBase {
    constructor(app) {
        super(app);
        //void
    }
    connectNewDeviceToAccount(accountId) {
        let connectedDeviceEntry = this.getApp().getSquareHeads().getClientsDatastore().insertConnectedDevice(accountId);
        let ret = new ConnectedClientDeviceInfo_1.ConnectedClientDeviceInfo(connectedDeviceEntry.deivceId, connectedDeviceEntry.deivceKey, connectedDeviceEntry.accountId);
        return ret;
    }
    getConnectedDeviceInfoFromDeviceId(connectedDeviceId) {
        let connectedDeviceEntry = this.getApp().getSquareHeads().getClientsDatastore().selectConnectedDeviceWithDeviceId(connectedDeviceId);
        if (connectedDeviceEntry == null) {
            return null;
        }
        let ret = new ConnectedClientDeviceInfo_1.ConnectedClientDeviceInfo(connectedDeviceEntry.deivceId, connectedDeviceEntry.deivceKey, connectedDeviceEntry.accountId);
        return ret;
    }
}
exports.ClientDevicesManager = ClientDevicesManager;
//# sourceMappingURL=ClientDevicesManager.js.map