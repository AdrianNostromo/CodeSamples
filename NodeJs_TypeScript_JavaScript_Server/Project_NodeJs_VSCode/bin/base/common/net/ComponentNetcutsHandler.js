"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
const ComponentType_1 = require("base/common/ComponentType");
const ComponentStatus_1 = require("base/common/util/ComponentStatus");
class ComponentNetcutsHandler extends BasicAppComponent_1.BasicAppComponent {
    constructor(app) {
        super(app);
        this._ILoopUserBase_General = true;
        this._netcutsList = [];
        this._defaultNetcuts = new Map();
        this._bf_onNetcutError = null;
        this._bf_onNetcutRemoteComponentStatusChanged = null;
        this._cbNetcutError = null;
        //void
    }
    create_framework() {
        super.create_framework();
        this._bf_onNetcutError = this.onNetcutError.bind(this);
        this._bf_onNetcutRemoteComponentStatusChanged = this.onNetcutRemoteComponentStatusChanged.bind(this);
    }
    addNetcut(netcut) {
        netcut.setCbError(this._bf_onNetcutError);
        netcut.setCbRemoteComponentStatusChanged(this._bf_onNetcutRemoteComponentStatusChanged);
        this._netcutsList.push(netcut);
        this.updateIfNeededDefaultOnlineComponentForComponentType(netcut.getComponentType());
    }
    updateIfNeededDefaultOnlineComponentForComponentType(remoteComponentType) {
        let defaultNetcut = this._defaultNetcuts.get(remoteComponentType.id);
        if (defaultNetcut && this._netcutsList.indexOf(defaultNetcut) >= 0 && defaultNetcut.getRemoteComponentStatus() && defaultNetcut.getRemoteComponentStatus().isOnline) {
            return;
        }
        this._defaultNetcuts.delete(remoteComponentType.id);
        for (let i = 0; i < this._netcutsList.length; i++) {
            let netcut = this._netcutsList[i];
            if (netcut && netcut.getComponentType() == remoteComponentType && netcut.getRemoteComponentStatus() && netcut.getRemoteComponentStatus().isOnline) {
                this._defaultNetcuts.set(netcut.getComponentType().id, netcut);
                break;
            }
        }
    }
    onNetcutError(netcut) {
        let compType = netcut.getComponentType();
        if (netcut.getComponentType() == ComponentType_1.ComponentType.SERVER_MANAGER && !netcut.getIsServerLocally()) {
            // Critical error.
            this.getApp().getLog().error('Critical error. Server Manager connection lost.');
            this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
            return null;
        }
        else {
            // Show a warning message and try to recover.
            if (!netcut.getIsServerLocally()) {
                this.getApp().getLog().warning('Remote server component connection lost. Try to recover. Remote component type: ' + netcut.getComponentType().name);
            }
            else {
                this.getApp().getLog().warning('Remote client component connection lost. Try to recover. Remote component type: ' + netcut.getComponentType().name);
            }
        }
        // Remove netcut.
        let index = this._netcutsList.indexOf(netcut);
        if (index < 0) {
            // Already processed, ignore.
            return;
        }
        this._netcutsList.splice(index, 1);
        this.updateIfNeededDefaultOnlineComponentForComponentType(compType);
    }
    onNetcutRemoteComponentStatusChanged(netcut) {
        this.updateIfNeededDefaultOnlineComponentForComponentType(netcut.getComponentType());
    }
    getDefaultOnlineNetcut(remoteComponentType) {
        if (!remoteComponentType) {
            this.getApp().getLog().error('ServerComponentValidator. Error.');
            this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
            return null;
        }
        return this._defaultNetcuts.get(remoteComponentType.id);
    }
    getOnlineComponentsCountFromType(remoteComponentType) {
        if (!remoteComponentType) {
            this.getApp().getLog().error('ServerComponentValidator. Error.');
            this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
            return null;
        }
        let ct = 0;
        for (let i = 0; i < this._netcutsList.length; i++) {
            let netcut = this._netcutsList[i];
            if (netcut && netcut.getComponentType() == remoteComponentType && netcut.getRemoteComponentStatus() && netcut.getRemoteComponentStatus().isOnline) {
                ct++;
            }
        }
        return ct;
    }
    getComponentsCountWithStatus(remoteComponentType, remoteComponentStatus) {
        if (!remoteComponentType || !remoteComponentStatus) {
            this.getApp().getLog().error('ServerComponentValidator. Error.');
            this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
            return null;
        }
        let ct = 0;
        for (let i = 0; i < this._netcutsList.length; i++) {
            let netcut = this._netcutsList[i];
            if (netcut && netcut.getComponentType() == remoteComponentType && netcut.getRemoteComponentStatus() && netcut.getRemoteComponentStatus() == remoteComponentStatus) {
                ct++;
            }
        }
        return ct;
    }
    appLoop_General(deltaS, deltaMS) {
        for (let i = this._netcutsList.length - 1; i >= 0; i--) {
            let netcut = this._netcutsList[i];
            if (netcut) {
                netcut.logicLoop(deltaS, deltaMS);
            }
        }
    }
    isAllComponentsOnlineForWorld() {
        for (let i = 0; i < this._netcutsList.length; i++) {
            let netcut = this._netcutsList[i];
            if (netcut && (!netcut.getRemoteComponentStatus() || netcut.getRemoteComponentStatus() != ComponentStatus_1.ComponentStatus.ONLINE_FOR_WORLD)) {
                return false;
            }
        }
        return true;
    }
    setCbNetcutError(cbNetcutError) {
        this._cbNetcutError = cbNetcutError;
    }
}
exports.ComponentNetcutsHandler = ComponentNetcutsHandler;
//# sourceMappingURL=ComponentNetcutsHandler.js.map