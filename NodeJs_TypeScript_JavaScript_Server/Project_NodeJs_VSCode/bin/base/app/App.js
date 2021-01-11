"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const AppServerNetcutsConnector_1 = require("./AppServerNetcutsConnector");
const ComponentType_1 = require("base/common/ComponentType");
const AppConfig_1 = require("base/common/config/AppConfig");
const ComponentStatus_1 = require("base/common/util/ComponentStatus");
class App extends AppServerNetcutsConnector_1.AppServerNetcutsConnector {
    constructor(componentType, dirs, netcutsFactory, clientListenerNetConfig, serverConnectorNetConfig) {
        super(componentType, dirs, netcutsFactory, clientListenerNetConfig, serverConnectorNetConfig);
        //void
    }
    create_framework() {
        super.create_framework();
        this.getLog().console('Component type: ' + this._componentType.name);
    }
    startApp() {
        if (this.getComponentStatus() != ComponentStatus_1.ComponentStatus.OFFLINE) {
            this.getApp().getLog().error('Exit. Initialize error 1.');
            this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
            return;
        }
        this.setComponentStatus(ComponentStatus_1.ComponentStatus.STARTING);
    }
    componentStatusChanged(componentStatus) {
        super.componentStatusChanged(componentStatus);
        if (componentStatus == ComponentStatus_1.ComponentStatus.STARTING) {
            if (this._componentType != ComponentType_1.ComponentType.SERVER_MANAGER) {
                if (!this.getApp().getComponentDependenciesHandler().addDependencyComponentListenerIpPort(ComponentType_1.ComponentType.SERVER_MANAGER, AppConfig_1.AppConfig.DEFAULT_SERVER_MANAGER_IP_PORT)) {
                    this.getApp().getLog().error('Exit. Initialize error 1.');
                    this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
                    return;
                }
                if (!this.getApp().getComponentDependenciesHandler().addDependency(ComponentType_1.ComponentType.SERVER_MANAGER, ComponentStatus_1.ComponentStatus.CONNECTING)) {
                    this.getApp().getLog().error('Exit. Initialize error 1.');
                    this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
                    return;
                }
            }
            if (!this.getApp().getComponentDependenciesHandler().addDependency(ComponentType_1.ComponentType.LOG_MANAGER, ComponentStatus_1.ComponentStatus.ONLINE_FOR_COMPONENTS)) {
                this.getApp().getLog().error('Exit. Initialize error 1.');
                this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
                return;
            }
        }
    }
}
exports.App = App;
//# sourceMappingURL=App.js.map