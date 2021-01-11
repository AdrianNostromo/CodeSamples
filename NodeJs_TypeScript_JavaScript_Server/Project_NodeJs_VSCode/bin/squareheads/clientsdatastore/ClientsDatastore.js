"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ClientsDatastoreSessions_1 = require("./ClientsDatastoreSessions");
class ClientsDatastore extends ClientsDatastoreSessions_1.ClientsDatastoreSessions {
    constructor(app) {
        super(app);
        //void
    }
    create_framework() {
        // NOTE : database creation code.
        //this.factoryInitializeNewDatabase();
        super.create_framework();
    }
    getDbPath() {
        //super.getDbPath();
        let dbFilePath = this.getApp().getDirs().dataDir + 'users/db_clients.json';
        return dbFilePath;
    }
}
exports.ClientsDatastore = ClientsDatastore;
//# sourceMappingURL=ClientsDatastore.js.map