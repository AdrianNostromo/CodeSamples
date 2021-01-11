"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ServerConnectorNetConfig_1 = require("base/module/util/ServerConnectorNetConfig");
const SquareHeadsManager_1 = require("./../SquareHeadsManager");
const LocalNetcutsFactory_1 = require("./../netcut/LocalNetcutsFactory");
const App_1 = require("base/app/App");
class AppLocal extends App_1.App {
    constructor(componentType, dirs) {
        super(componentType, dirs, new LocalNetcutsFactory_1.LocalNetcutsFactory(), null, new ServerConnectorNetConfig_1.ServerConnectorNetConfig());
        this.squareHeads = null;
        this.initApp(this);
    }
    getApp() {
        return this;
    }
    getSquareHeads() {
        return this.squareHeads;
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.squareHeads = new SquareHeadsManager_1.SquareHeadsManager(this.getApp()));
    }
}
exports.AppLocal = AppLocal;
//# sourceMappingURL=AppLocal.js.map