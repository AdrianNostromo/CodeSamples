"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
const ServerStatus_1 = require("base/common/server/ServerStatus");
const ComponentStatus_1 = require("base/common/util/ComponentStatus");
const Logger_1 = require("base/log/Logger");
const ErrorHandler_1 = require("base/error/ErrorHandler");
const ComponentDependenciesHandler_1 = require("base/common/util/ComponentDependenciesHandler");
const LoopDistribution_1 = require("base/component/loop/LoopDistribution");
const ConsoleHandler_1 = require("base/console/ConsoleHandler");
class AppBase extends BasicAppComponent_1.BasicAppComponent {
    constructor(componentType, dirs) {
        super(null);
        this._ILoopUserBase_General = true;
        this._componentStatus = ComponentStatus_1.ComponentStatus.OFFLINE;
        this._serverStatus = ServerStatus_1.ServerStatus.OFFLINE;
        this.log = null;
        this.errorHandler = null;
        this.consoleHandler = null;
        this.componentDependenciesHandler = null;
        this._statusChangeCheckDelayS = 0;
        this.loopDistribution = new LoopDistribution_1.LoopDistribution();
        this.dirs = dirs;
        this._componentType = componentType;
        this.setRoot(this);
    }
    getDirs() {
        return this.dirs;
    }
    getLog() {
        return this.log;
    }
    getConsoleHandler() {
        return this.consoleHandler;
    }
    getComponentDependenciesHandler() {
        return this.componentDependenciesHandler;
    }
    getErrorHandler() {
        return this.errorHandler;
    }
    create_framework() {
        super.create_framework();
        this.loopDistribution.setRootComponent(this);
        this.addComponent(this.log = new Logger_1.Logger(this.getApp()));
        this.addComponent(this.errorHandler = new ErrorHandler_1.ErrorHandler(this.getApp()));
        this.addComponent(this.consoleHandler = new ConsoleHandler_1.ConsoleHandler(this.getApp()));
        this.addComponent(this.componentDependenciesHandler = new ComponentDependenciesHandler_1.ComponentDependenciesHandler(this.getApp()));
    }
    appLoop_General(deltaS, deltaMS) {
        if (this._componentStatus.isTransientState) {
            this._statusChangeCheckDelayS -= deltaS;
            if (this._statusChangeCheckDelayS <= 0) {
                this._statusChangeCheckDelayS = 0.2;
                if (this.isComponentTransientStateFinished(this._componentStatus)) {
                    if (this._componentStatus == ComponentStatus_1.ComponentStatus.STARTING) {
                        this.setComponentStatus(ComponentStatus_1.ComponentStatus.CONNECTING);
                    }
                    else if (this._componentStatus == ComponentStatus_1.ComponentStatus.CONNECTING) {
                        this.setComponentStatus(ComponentStatus_1.ComponentStatus.ONLINE_FOR_COMPONENTS);
                    }
                    else if (this._componentStatus == ComponentStatus_1.ComponentStatus.ONLINE_FOR_COMPONENTS) {
                        this.setComponentStatus(ComponentStatus_1.ComponentStatus.ONLINE_FOR_WORLD);
                    }
                    else if (this._componentStatus == ComponentStatus_1.ComponentStatus.STOPING) {
                        this.setComponentStatus(ComponentStatus_1.ComponentStatus.OFFLINE);
                    }
                    else {
                        this.getApp().getLog().error('ServerComponentValidator. Error.');
                        this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
                        return;
                    }
                }
            }
        }
    }
    getComponentType() {
        return this._componentType;
    }
    getComponentStatus() {
        return this._componentStatus;
    }
    isStatus_OFFLINE() {
        if (this._componentStatus == ComponentStatus_1.ComponentStatus.OFFLINE) {
            return true;
        }
        return false;
    }
    isStatus_STOPING() {
        if (this._componentStatus == ComponentStatus_1.ComponentStatus.STOPING) {
            return true;
        }
        return false;
    }
    isStatus_STARTING() {
        if (this._componentStatus == ComponentStatus_1.ComponentStatus.STARTING) {
            return true;
        }
        return false;
    }
    isStatus_CONNECTING() {
        if (this._componentStatus == ComponentStatus_1.ComponentStatus.CONNECTING) {
            return true;
        }
        return false;
    }
    isStatus_ONLINE_FOR_COMPONENTS() {
        if (this._componentStatus == ComponentStatus_1.ComponentStatus.ONLINE_FOR_COMPONENTS) {
            return true;
        }
        return false;
    }
    isStatus_ONLINE_FOR_WORLD() {
        if (this._componentStatus == ComponentStatus_1.ComponentStatus.ONLINE_FOR_WORLD) {
            return true;
        }
        return false;
    }
    isServerStatus_OFFLINE() {
        if (this._serverStatus == ServerStatus_1.ServerStatus.OFFLINE) {
            return true;
        }
        return false;
    }
    isServerStatus_ONLINE() {
        if (this._serverStatus == ServerStatus_1.ServerStatus.ONLINE) {
            return true;
        }
        return false;
    }
    setComponentStatus(componentStatus) {
        if (this._componentStatus == componentStatus) {
            this.getApp().getLog().error('App. Error.');
            this.getApp().getErrorHandler().error('App. Error.');
            return;
        }
        this.getLog().console('Status changed from: ' + this._componentStatus.name + ' to: ' + componentStatus.name);
        this._componentStatus = componentStatus;
        this.componentStatusChanged(this._componentStatus);
        if (this._componentStatus.isTransientState) {
            this._statusChangeCheckDelayS = 0.2;
        }
    }
    getServerStatus() {
        return this._serverStatus;
    }
    setServerStatus(serverStatus) {
        this._serverStatus = serverStatus;
        this.getApp().getLog().info('Server status changed: ' + this._serverStatus.name);
    }
}
exports.AppBase = AppBase;
//# sourceMappingURL=AppBase.js.map