"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ComponentDependenciesHandlerBase_1 = require("./ComponentDependenciesHandlerBase");
const ComponentType_1 = require("./../ComponentType");
const ComponentStatus_1 = require("./../util/ComponentStatus");
const ComponentDependencyEntry_1 = require("./ComponentDependencyEntry");
const ServerConnectionCallbacksHolder_1 = require("./ServerConnectionCallbacksHolder");
class CurrentComponentDependency {
    constructor(componentType, componentStatusLayer, completionStatus) {
        this.statusDurationS = 0;
        this.logicDelayS = 0;
        this.ipPortResponseCurrentWaitTimeS = 0;
        this.isUnvalidatedConnected = false;
        this.isError = false;
        this.isValidated = false;
        this.isDepConFailed = false;
        this.componentType = componentType;
        this.componentStatusLayer = componentStatusLayer;
        this.completionStatus = completionStatus;
    }
}
class ComponentDependenciesHandlerMandatory extends ComponentDependenciesHandlerBase_1.ComponentDependenciesHandlerBase {
    constructor(app) {
        super(app);
        this._ILoopUserBase_General = true;
        this._dependenciesList = [];
        this._isDepsConnected_CONNECTING = false;
        this._isDepsConnected_ONLINE_FOR_COMPONENTS = false;
        this._currentDependency = null;
        this._uLogID_A = null;
        this._uniquePostDepConRetryLogger = new Map();
        this.depConnectionCbHolder = new ServerConnectionCallbacksHolder_1.ServerConnectionCallbacksHolder(this.onUnvalidatedDepConnection.bind(this), this.onDepConnectionFail.bind(this), this.onDepConValidated.bind(this));
        //void
    }
    appLoop_General(deltaS, deltaMS) {
        if (this._currentDependency) {
            this.doActiveDependencyTick(deltaS, deltaMS);
        }
        if (!this._currentDependency) {
            // If any dep is missing, update the component status as needed.
            if (!this.depsTick(deltaS, deltaMS, ComponentStatus_1.ComponentStatus.CONNECTING)) {
                this._isDepsConnected_CONNECTING = false;
                // Set the component status to CONNECTING if it is higher.
                if (this.getApp().getComponentStatus().id > ComponentStatus_1.ComponentStatus.CONNECTING.id) {
                    this.getApp().setComponentStatus(ComponentStatus_1.ComponentStatus.CONNECTING);
                }
            }
            else {
                this._isDepsConnected_CONNECTING = true;
                if (!this.depsTick(deltaS, deltaMS, ComponentStatus_1.ComponentStatus.ONLINE_FOR_COMPONENTS)) {
                    this._isDepsConnected_ONLINE_FOR_COMPONENTS = false;
                    // Set the component status to ONLINE_FOR_COMPONENTS if it is higher.
                    if (this.getApp().getComponentStatus().id > ComponentStatus_1.ComponentStatus.ONLINE_FOR_COMPONENTS.id) {
                        this.getApp().setComponentStatus(ComponentStatus_1.ComponentStatus.ONLINE_FOR_COMPONENTS);
                    }
                }
                else {
                    this._isDepsConnected_ONLINE_FOR_COMPONENTS = true;
                }
            }
        }
    }
    depsTick(deltaS, deltaMS, dependencyComponentStatus) {
        for (let i = 0; i < this._dependenciesList.length; i++) {
            let depComponent = this._dependenciesList[i];
            if (depComponent.componentStatusLayer == dependencyComponentStatus) {
                // Is CONNECTING dependency.
                if (!this.getApp().getServerComponentNetcuts().getDefaultOnlineNetcut(depComponent.componentType)) {
                    // Dependency required.
                    this._currentDependency = new CurrentComponentDependency(depComponent.componentType, depComponent.componentStatusLayer, ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_NEW);
                    return false;
                }
            }
        }
        return true;
    }
    componentStatusChanged(componentStatus) {
        super.componentStatusChanged(componentStatus);
        //void
    }
    isComponentTransientStateFinished(componentStatus) {
        let ret = super.isComponentTransientStateFinished(componentStatus);
        if (componentStatus == ComponentStatus_1.ComponentStatus.CONNECTING) {
            if (!this._isDepsConnected_CONNECTING) {
                ret = false;
            }
        }
        else if (componentStatus == ComponentStatus_1.ComponentStatus.ONLINE_FOR_COMPONENTS) {
            if (!this._isDepsConnected_ONLINE_FOR_COMPONENTS) {
                ret = false;
            }
        }
        return ret;
    }
    doActiveDependencyTick(deltaS, deltaMS) {
        if (!this._currentDependency) {
            return false;
        }
        if (this._currentDependency.isError) {
            // Connection failed, try again.
            this.getApp().getLog().warning('Server dependency connection failed. Remote server component type: ' + this._currentDependency.componentType.name + '. Treated a warning, not critical at this location. Try again.');
            this._currentDependency = null;
            return false;
        }
        this._currentDependency.statusDurationS += deltaS;
        if (this._currentDependency.logicDelayS > 0) {
            this._currentDependency.logicDelayS -= deltaS;
        }
        if (this._currentDependency.logicDelayS > 0) {
            return true;
        }
        let depListenerIpPort;
        switch (this._currentDependency.completionStatus) {
            case ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_NEW:
                this._uLogID_A = this.getApp().getLog().getNextULogId();
                if (this._currentDependency.componentType != ComponentType_1.ComponentType.SERVER_MANAGER) {
                    // Clear the dependency ip port.
                    this.clearDependencyComponentListenerIpPort(this._currentDependency.componentType);
                }
                depListenerIpPort = this.getDependencyComponentListenerIpPort(this._currentDependency.componentType);
                if (!depListenerIpPort) {
                    if (this._currentDependency.componentStatusLayer == ComponentStatus_1.ComponentStatus.CONNECTING || this.getApp().getComponentType() != ComponentType_1.ComponentType.SERVER_MANAGER) {
                        if (this._currentDependency.componentStatusLayer == ComponentStatus_1.ComponentStatus.CONNECTING && this.getApp().getComponentType() == ComponentType_1.ComponentType.SERVER_MANAGER) {
                            return false;
                        }
                        // Make request to the ServerManager for the ip port of the dependency components listener.
                        let serverManagerNetcut = this.getApp().getServerComponentNetcuts().getDefaultOnlineNetcut(ComponentType_1.ComponentType.SERVER_MANAGER);
                        if (serverManagerNetcut == null) {
                            return false;
                        }
                        if (!serverManagerNetcut.requestComponentListenerIpPort(this._currentDependency.componentType)) {
                            return false;
                        }
                        this._currentDependency.ipPortResponseCurrentWaitTimeS = 0;
                        this._currentDependency.completionStatus = ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_LISTENER_IP_PORT;
                        this._currentDependency.statusDurationS = 0;
                    }
                    else if (this._currentDependency.componentStatusLayer == ComponentStatus_1.ComponentStatus.ONLINE_FOR_COMPONENTS) {
                        // this._currentDependency.componentStatusLayer == ComponentStatus.ONLINE_FOR_COMPONENTS && is server Manager;
                        // For pos online dependencies, wait until they have their component listener active.
                        if (this.getApp().getComponentType() != ComponentType_1.ComponentType.SERVER_MANAGER) {
                            return false;
                        }
                        // Try to get the ipPort from the netcutsHandler and push it locally.
                        let targetComponentNetcut = this.getApp().getClientComponentNetcuts().getDefaultOnlineNetcut(this._currentDependency.componentType);
                        if (!targetComponentNetcut) {
                            this.sendLogPostDependencyNotOnline(this._currentDependency.componentType);
                            this._currentDependency.logicDelayS = 0.5;
                        }
                        else {
                            let ipPort = targetComponentNetcut.getRemoteComponentListenerIpPort();
                            if (!ipPort) {
                                return false;
                            }
                            if (!this.getApp().getComponentDependenciesHandler().addDependencyComponentListenerIpPort(this._currentDependency.componentType, ipPort)) {
                                return false;
                            }
                            this._currentDependency.completionStatus = ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_LISTENER_IP_PORT;
                            this._currentDependency.statusDurationS = 0;
                        }
                    }
                }
                else {
                    this._currentDependency.completionStatus = ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_LISTENER_IP_PORT;
                    this._currentDependency.statusDurationS = 0;
                }
                break;
            case ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_LISTENER_IP_PORT:
                depListenerIpPort = this.getDependencyComponentListenerIpPort(this._currentDependency.componentType);
                if (!depListenerIpPort) {
                    this._currentDependency.ipPortResponseCurrentWaitTimeS += deltaS;
                    if (this._currentDependency.ipPortResponseCurrentWaitTimeS >= 5) {
                        if (this._currentDependency.componentStatusLayer == ComponentStatus_1.ComponentStatus.CONNECTING) {
                            // Ip port should have been received from the server Manager.
                            return false;
                        }
                        else {
                            // Dependency failed, try again.
                            this.sendLogPostDependencyNotOnline(this._currentDependency.componentType);
                            this._currentDependency = null;
                            return false;
                        }
                    }
                }
                else {
                    // Connect to the dependency component and wait until it is validated.
                    let ret = this.getApp().connectToServerComponent(this._currentDependency.componentType, depListenerIpPort.ip, depListenerIpPort.port, this.depConnectionCbHolder);
                    if (!ret) {
                        return false;
                    }
                    this._currentDependency.completionStatus = ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_CONNECTION;
                    this._currentDependency.statusDurationS = 0;
                }
                break;
            case ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_CONNECTION:
                // Error if not connected after 10 seconds. Show a log every 10 seconds while waiting.
                if (this.getApp().getServerComponentNetcuts().getDefaultOnlineNetcut(this._currentDependency.componentType)) {
                    this._currentDependency = null;
                }
                else {
                    if (this._currentDependency.statusDurationS > 5 && !this._currentDependency.isError && !this._currentDependency.isUnvalidatedConnected && !this._currentDependency.isValidated) {
                        // Logic error.
                        this.getApp().getLog().error('Dependency validation timeout threshold occured. Component type: ' + this._currentDependency.componentType.name);
                        this.getApp().getErrorHandler().error('Dependency failed.');
                        throw new Error("Error");
                    }
                    else if (this._currentDependency.statusDurationS > 200) {
                        this.getApp().getLog().error('Dependency connected but not online. Component type: ' + this._currentDependency.componentType.name);
                        this.getApp().getErrorHandler().error('Dependency failed.');
                        throw new Error("Error");
                    }
                    else {
                        this.getApp().getLog().info('Waiting for dependency with type: ' + this._currentDependency.componentType.name, this._uLogID_A, 5);
                    }
                }
                break;
            default:
                this.getApp().getLog().error('Dependency connected but not online. Component type: ' + this._currentDependency.componentType.name);
                this.getApp().getErrorHandler().error('Dependency failed.');
                throw new Error("Error");
        }
        return true;
    }
    onUnvalidatedDepConnection(con, remoteComponentType) {
        // Check the component type;
        if (!this._currentDependency || this._currentDependency.componentType != remoteComponentType) {
            this.getApp().getLog().error('Dependency failed. Component type: ' + remoteComponentType.name);
            this.getApp().getErrorHandler().error('Dependency failed.');
            return;
        }
        this._currentDependency.isUnvalidatedConnected = true;
        this.getApp().getLog().info('Unvalidated dependency connected with type: ' + this._currentDependency.componentType.name);
    }
    onDepConnectionFail(con, remoteComponentType) {
        // Check the component type;
        if (!this._currentDependency || this._currentDependency.componentType != remoteComponentType) {
            this.getApp().getLog().error('Dependency failed. Component type: ' + remoteComponentType.name);
            this.getApp().getErrorHandler().error('Dependency failed.');
            return;
        }
        this._currentDependency.isError = true;
        this.getApp().getLog().info('Unvalidated dependency connection failed with type: ' + this._currentDependency.componentType.name);
    }
    onDepConValidated(con, remoteComponentType) {
        // Check the component type;
        if (!this._currentDependency || this._currentDependency.componentType != remoteComponentType) {
            this.getApp().getLog().error('Dependency failed. Component type: ' + remoteComponentType.name);
            this.getApp().getErrorHandler().error('Dependency failed.');
            return;
        }
        this._currentDependency.isValidated = true;
        this.getApp().getLog().info('Dependency connection validated with type: ' + this._currentDependency.componentType.name);
    }
    sendLogPostDependencyNotOnline(dependencyComponentType) {
        if (!this._uniquePostDepConRetryLogger.get(dependencyComponentType.id)) {
            this._uniquePostDepConRetryLogger.set(dependencyComponentType.id, true);
            this.getApp().getLog().info('Post dependency no online. Dependency type: ' + this._currentDependency.componentType.name);
        }
    }
    addDependency(componentType, componentStatusLayer) {
        if (this.isComponentDependency(componentType)) {
            return false;
        }
        this._dependenciesList.push(new ComponentDependencyEntry_1.ComponentDependencyEntry(componentType, componentStatusLayer));
        return true;
    }
    isComponentDependency(componentType) {
        for (let i = 0; i < this._dependenciesList.length; i++) {
            let depComponent = this._dependenciesList[i];
            if (depComponent && depComponent.componentType == componentType) {
                return true;
            }
        }
        return false;
    }
}
ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_NEW = 1;
ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_LISTENER_IP_PORT = 2;
ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_CONNECTION = 3;
exports.ComponentDependenciesHandlerMandatory = ComponentDependenciesHandlerMandatory;
//# sourceMappingURL=ComponentDependenciesHandlerMandatory.js.map