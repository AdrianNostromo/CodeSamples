"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LocalCustomModule_1 = require("./module/LocalCustomModule");
const ServerStructureHandler_1 = require("servermanager/serverstructurehandler/ServerStructureHandler");
class ServerManager extends LocalCustomModule_1.LocalCustomModule {
    constructor(app) {
        super(app, [
        //void
        ]);
        this._serverStructureHandler = null;
        this._serverStructureHandler = null;
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this._serverStructureHandler = new ServerStructureHandler_1.ServerStructureHandler(this.getApp()));
    }
}
exports.ServerManager = ServerManager;
//# sourceMappingURL=ServerManager.js.map