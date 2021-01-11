"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LocalAppComponent_1 = require("./../component/LocalAppComponent");
const AppConfig_1 = require("base/common/config/AppConfig");
const ComponentStatus_1 = require("base/common/util/ComponentStatus");
const ComponentType_1 = require("base/common/ComponentType");
const ServerStatus_1 = require("base/common/server/ServerStatus");
class ServerComponentConfig {
    constructor(componentType, count, isAutoCreated) {
        this.componentType = componentType;
        this.count = count;
        this.isAutoCreated = isAutoCreated;
    }
}
class CurrentlyCreatingComponent {
    constructor(componentType, curentOnlineComponentsCount) {
        this.currentWaitDurationS = 0;
        this.componentType = componentType;
        this.curentOnlineComponentsCount = curentOnlineComponentsCount;
    }
}
class ServerStructureHandler extends LocalAppComponent_1.LocalAppComponent {
    constructor(app) {
        super(app);
        this._ILoopUserBase_General = true;
        this._currentlyCreatingComponent = null;
        this._isInitialNodeControllersConnected = false;
        //void
    }
    create_framework() {
        super.create_framework();
        this._uLogID_A = this.getApp().getLog().getNextULogId();
        this._uLogID_B = this.getApp().getLog().getNextULogId();
    }
    appLoop_General(deltaS, deltaMS) {
        this.appLoop_local(deltaS, deltaMS);
    }
    appLoop_local(deltaS, deltaMS) {
        if (!this.getApp().getComponentStatus().isOnline) {
            return;
        }
        if (!this._isInitialNodeControllersConnected) {
            this.getApp().getLog().info('Waiting for the initial ' + AppConfig_1.AppConfig.NODES_COUNT + ' NODE_CONTROLLER components.', this._uLogID_A, 5);
            let compsCount = this.getApp().getClientComponentNetcuts().getOnlineComponentsCountFromType(ComponentType_1.ComponentType.NODE_CONTROLLER);
            if (compsCount > AppConfig_1.AppConfig.NODES_COUNT) {
                this.getApp().getLog().error('ServerComponentValidator. Error.');
                this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
                return;
            }
            else if (compsCount < AppConfig_1.AppConfig.NODES_COUNT) {
                //void
            }
            else {
                this.getApp().getLog().info('Initial ' + AppConfig_1.AppConfig.NODES_COUNT + ' NODE_CONTROLLER components connected.');
                this._isInitialNodeControllersConnected = true;
            }
        }
        if (this._currentlyCreatingComponent) {
            let onlineCompsCount = this.getApp().getClientComponentNetcuts().getOnlineComponentsCountFromType(this._currentlyCreatingComponent.componentType);
            if (onlineCompsCount > 1) {
                // Currently only one component of each type can exits.
                this.getApp().getLog().error('Component creation through a NODE_CONTROLLER failed. Remote component type: ' + this._currentlyCreatingComponent.componentType.name);
                this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
                return;
            }
            if (onlineCompsCount > this._currentlyCreatingComponent.curentOnlineComponentsCount) {
                this.getApp().getLog().info('Dependency connected with type: ' + this._currentlyCreatingComponent.componentType.name);
                this._currentlyCreatingComponent = null;
            }
            else {
                // Wait for 10 seconds for the component or treat as critical error.
                this._currentlyCreatingComponent.currentWaitDurationS += deltaS;
                if (this._currentlyCreatingComponent.currentWaitDurationS >= 20000) { //asd_tst_001;// 20
                    this.getApp().getLog().error('Component creation through a NODE_CONTROLLER failed. Remote component type: ' + this._currentlyCreatingComponent.componentType.name);
                    this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
                    return;
                }
            }
        }
        let isAllComponentsOnlineForWorld = true;
        // Check if any components need creating.
        for (let i = 0; i < ServerStructureHandler.targetServerComponentsStructure.length; i++) {
            let compConfig = ServerStructureHandler.targetServerComponentsStructure[i];
            if (compConfig.isAutoCreated) {
                let onlineCompsCount = this.getApp().getClientComponentNetcuts().getOnlineComponentsCountFromType(compConfig.componentType);
                if (onlineCompsCount > compConfig.count) {
                    this.getApp().getLog().error('ServerComponentValidator. Error.');
                    this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
                    return;
                }
                if (onlineCompsCount < compConfig.count) {
                    // More components required. Create it.
                    isAllComponentsOnlineForWorld = false;
                    if (!this._currentlyCreatingComponent) {
                        if (this._isInitialNodeControllersConnected) {
                            let nodeControllerNetcut = this.getApp().getClientComponentNetcuts().getDefaultOnlineNetcut(ComponentType_1.ComponentType.NODE_CONTROLLER);
                            if (nodeControllerNetcut) {
                                this._currentlyCreatingComponent = new CurrentlyCreatingComponent(compConfig.componentType, onlineCompsCount);
                                nodeControllerNetcut.requestCreateComponent(compConfig.componentType);
                                this.getApp().getLog().info('Component create request sent. Component type: ' + compConfig.componentType.name);
                            }
                            else {
                                this.getApp().getLog().info('Waiting for a NODE_CONTROLLER to create a component with type: ' + compConfig.componentType.name, this._uLogID_B, 5);
                            }
                        }
                    }
                    break;
                }
            }
            let onlineForWorldCompsCount = this.getApp().getClientComponentNetcuts().getComponentsCountWithStatus(compConfig.componentType, ComponentStatus_1.ComponentStatus.ONLINE_FOR_WORLD);
            if (onlineForWorldCompsCount != compConfig.count) {
                isAllComponentsOnlineForWorld = false;
            }
        }
        if (!isAllComponentsOnlineForWorld && this.getApp().getServerStatus() != ServerStatus_1.ServerStatus.OFFLINE) {
            // Server should be offline.
            this.getApp().setServerStatus(ServerStatus_1.ServerStatus.OFFLINE);
        }
        else if (isAllComponentsOnlineForWorld && this.getApp().getServerStatus() != ServerStatus_1.ServerStatus.ONLINE) {
            // Server should be online.
            this.getApp().setServerStatus(ServerStatus_1.ServerStatus.ONLINE);
        }
    }
}
ServerStructureHandler.targetServerComponentsStructure = [
    new ServerComponentConfig(ComponentType_1.ComponentType.LOG_MANAGER, 1, true),
    //new ServerComponentConfig(ComponentType.STAR_MANAGER, 1, false)//asd_tst_001;
    new ServerComponentConfig(ComponentType_1.ComponentType.SQUARE_HEADS_MANAGER, 1, false) //asd_tst_001;
];
exports.ServerStructureHandler = ServerStructureHandler;
//# sourceMappingURL=ServerStructureHandler.js.map