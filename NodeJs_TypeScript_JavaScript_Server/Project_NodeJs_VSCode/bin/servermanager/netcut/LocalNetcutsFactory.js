"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicNetcutsFactory_1 = require("base/net/BasicNetcutsFactory");
const ComponentType_1 = require("base/common/ComponentType");
const NetcutFactoryConfig_1 = require("base/net/util/NetcutFactoryConfig");
const Netcut_SC_ServerManager_To_NodeController_1 = require("./Netcut_SC_ServerManager_To_NodeController");
const Netcut_SC_ServerManager_To_Component_1 = require("./Netcut_SC_ServerManager_To_Component");
const Netcut_CS_Component_To_LogManager_1 = require("base/logmanager/netcut/Netcut_CS_Component_To_LogManager");
class LocalNetcutsFactory extends BasicNetcutsFactory_1.BasicNetcutsFactory {
    constructor() {
        super();
        this.initEntries([
            // As server.
            new NetcutFactoryConfig_1.NetcutFactoryConfig(BasicNetcutsFactory_1.BasicNetcutsFactory.CONNECTION_TYPE_SERVER, ComponentType_1.ComponentType.NODE_CONTROLLER, function (app, con, componentType) {
                return new Netcut_SC_ServerManager_To_NodeController_1.Netcut_SC_ServerManager_To_NodeController(app, con, componentType);
            }.bind(this)),
            new NetcutFactoryConfig_1.NetcutFactoryConfig(BasicNetcutsFactory_1.BasicNetcutsFactory.CONNECTION_TYPE_SERVER, 
            // Default entry if null.
            null, function (app, con, componentType) {
                return new Netcut_SC_ServerManager_To_Component_1.Netcut_SC_ServerManager_To_Component(app, con, componentType);
            }.bind(this)),
            // As client.
            new NetcutFactoryConfig_1.NetcutFactoryConfig(BasicNetcutsFactory_1.BasicNetcutsFactory.CONNECTION_TYPE_CLIENT, ComponentType_1.ComponentType.LOG_MANAGER, function (app, con, componentType) {
                return new Netcut_CS_Component_To_LogManager_1.Netcut_CS_Component_To_LogManager(app, con, componentType);
            }.bind(this))
        ]);
    }
}
exports.LocalNetcutsFactory = LocalNetcutsFactory;
//# sourceMappingURL=LocalNetcutsFactory.js.map