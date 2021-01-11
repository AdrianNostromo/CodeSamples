"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const SquareHeadsManagerBase_1 = require("./SquareHeadsManagerBase");
const ClientsDatastore_1 = require("./clientsdatastore/ClientsDatastore");
const ClientAccountsManager_1 = require("./clientaccountsmanager/ClientAccountsManager");
const ClientDevicesManager_1 = require("./clientdevicesmanager/ClientDevicesManager");
const ClientSessionsManager_1 = require("./clientsessionsmanager/ClientSessionsManager");
const ClientAccountsInteractor_1 = require("./clientaccountsinteractor/ClientAccountsInteractor");
class SquareHeadsManagerUsers extends SquareHeadsManagerBase_1.SquareHeadsManagerBase {
    constructor(app) {
        super(app);
        this.clientsDatastore = null;
        this.clientAccountsManager = null;
        this.clientDevicesManager = null;
        this.clientSessionsManager = null;
        this.clientAccountsInteractor = null;
        //void
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.clientsDatastore = new ClientsDatastore_1.ClientsDatastore(this.getApp()));
        this.addComponent(this.clientAccountsManager = new ClientAccountsManager_1.ClientAccountsManager(this.getApp()));
        this.addComponent(this.clientDevicesManager = new ClientDevicesManager_1.ClientDevicesManager(this.getApp()));
        this.addComponent(this.clientSessionsManager = new ClientSessionsManager_1.ClientSessionsManager(this.getApp()));
        this.addComponent(this.clientAccountsInteractor = new ClientAccountsInteractor_1.ClientAccountsInteractor(this.getApp()));
    }
    getClientsDatastore() {
        return this.clientsDatastore;
    }
    getClientAccountsManager() {
        return this.clientAccountsManager;
    }
    getClientDevicesManager() {
        return this.clientDevicesManager;
    }
    getClientSessionsManager() {
        return this.clientSessionsManager;
    }
    getClientAccountsInteractor() {
        return this.clientAccountsInteractor;
    }
}
exports.SquareHeadsManagerUsers = SquareHeadsManagerUsers;
//# sourceMappingURL=SquareHeadsManagerUsers.js.map