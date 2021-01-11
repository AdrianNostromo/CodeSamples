"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ClientListenerNetConfig_1 = require("base/module/util/ClientListenerNetConfig");
const ServerConnectorNetConfig_1 = require("base/module/util/ServerConnectorNetConfig");
const AppConfig_1 = require("base/common/config/AppConfig");
const IpPort_1 = require("base/net/util/IpPort");
const ServerManager_1 = require("./../ServerManager");
const LocalNetcutsFactory_1 = require("./../netcut/LocalNetcutsFactory");
const App_1 = require("base/app/App");
class AppLocal extends App_1.App {
    constructor(componentType, dirs) {
        super(componentType, dirs, new LocalNetcutsFactory_1.LocalNetcutsFactory(), new ClientListenerNetConfig_1.ClientListenerNetConfig(new IpPort_1.IpPort('127.0.0.1', AppConfig_1.AppConfig.DEFAULT_SERVER_MANAGER_IP_PORT.port)), new ServerConnectorNetConfig_1.ServerConnectorNetConfig());
        this.serverManager = null;
        this.initApp(this);
    }
    getApp() {
        return this;
    }
    getServerManager() {
        return this.serverManager;
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.serverManager = new ServerManager_1.ServerManager(this.getApp()));
    }
}
exports.AppLocal = AppLocal;
//# sourceMappingURL=AppLocal.js.map