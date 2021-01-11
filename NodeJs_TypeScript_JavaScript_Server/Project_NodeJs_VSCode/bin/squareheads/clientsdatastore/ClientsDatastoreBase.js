"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicDatastoreModule_1 = require("base/common/db/BasicDatastoreModule");
const random_seed = require("random-seed");
class ClientsDatastoreBase extends BasicDatastoreModule_1.BasicDatastoreModule {
    constructor(app) {
        super(app);
        this.rng = null;
        this._app = app;
    }
    getApp() {
        return this._app;
    }
    create_framework() {
        super.create_framework();
        this.rng = random_seed.create((new Date()).getTime().toString(8));
    }
}
exports.ClientsDatastoreBase = ClientsDatastoreBase;
//# sourceMappingURL=ClientsDatastoreBase.js.map