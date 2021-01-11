"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ServerConnectorNetConfig_1 = require("base/module/util/ServerConnectorNetConfig");
const NodeController_1 = require("./../NodeController");
const LocalNetcutsFactory_1 = require("./../netcut/LocalNetcutsFactory");
const App_1 = require("base/app/App");
class AppLocal extends App_1.App {
    constructor(componentType, dirs) {
        super(componentType, dirs, new LocalNetcutsFactory_1.LocalNetcutsFactory(), null, new ServerConnectorNetConfig_1.ServerConnectorNetConfig());
        this.nodeController = null;
        this.initApp(this);
    }
    getApp() {
        return this;
    }
    getNodeController() {
        return this.nodeController;
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.nodeController = new NodeController_1.NodeController(this.getApp()));
    }
}
exports.AppLocal = AppLocal;
//# sourceMappingURL=AppLocal.js.map