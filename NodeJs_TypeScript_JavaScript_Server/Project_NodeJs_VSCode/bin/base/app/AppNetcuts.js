"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const AppBase_1 = require("./AppBase");
const BasicNetcutsFactory_1 = require("base/net/BasicNetcutsFactory");
const ComponentNetcutsHandler_1 = require("base/common/net/ComponentNetcutsHandler");
class AppNetcuts extends AppBase_1.AppBase {
    constructor(componentType, dirs, netcutsFactory) {
        super(componentType, dirs);
        this.clientComponentNetcuts = null;
        this.serverComponentNetcuts = null;
        this._netcutsFactory = netcutsFactory;
    }
    getClientComponentNetcuts() {
        return this.clientComponentNetcuts;
    }
    getServerComponentNetcuts() {
        return this.serverComponentNetcuts;
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.clientComponentNetcuts = new ComponentNetcutsHandler_1.ComponentNetcutsHandler(this.getApp()));
        this.getClientComponentNetcuts().setCbNetcutError(this.onComponentNetcutError.bind(this));
        this.addComponent(this.serverComponentNetcuts = new ComponentNetcutsHandler_1.ComponentNetcutsHandler(this.getApp()));
        this.getServerComponentNetcuts().setCbNetcutError(this.onComponentNetcutError.bind(this));
    }
    pushNetcut(remoteComponentType, con, isServerLocally) {
        let newNetcutConfig = null;
        if (isServerLocally) {
            newNetcutConfig = this._netcutsFactory.getNetcutConfig(BasicNetcutsFactory_1.BasicNetcutsFactory.CONNECTION_TYPE_SERVER, remoteComponentType);
            if (newNetcutConfig == null && this._netcutsFactory.getNetcutConfig(BasicNetcutsFactory_1.BasicNetcutsFactory.CONNECTION_TYPE_SERVER, null)) {
                newNetcutConfig = this._netcutsFactory.getNetcutConfig(BasicNetcutsFactory_1.BasicNetcutsFactory.CONNECTION_TYPE_SERVER, null);
            }
        }
        else {
            newNetcutConfig = this._netcutsFactory.getNetcutConfig(BasicNetcutsFactory_1.BasicNetcutsFactory.CONNECTION_TYPE_CLIENT, remoteComponentType);
            if (newNetcutConfig == null && this._netcutsFactory.getNetcutConfig(BasicNetcutsFactory_1.BasicNetcutsFactory.CONNECTION_TYPE_CLIENT, null)) {
                newNetcutConfig = this._netcutsFactory.getNetcutConfig(BasicNetcutsFactory_1.BasicNetcutsFactory.CONNECTION_TYPE_CLIENT, null);
            }
        }
        if (newNetcutConfig == null) {
            this.getApp().getLog().error('Netcut error.');
            this.getApp().getErrorHandler().error('Netcut error.');
            return;
        }
        let newNetcut = newNetcutConfig.newInstance(this.getApp(), con, remoteComponentType);
        if (!newNetcut) {
            this.getApp().getLog().error('Netcut error.');
            this.getApp().getErrorHandler().error('Netcut error.');
            return;
        }
        newNetcut.create();
        if (!isServerLocally) {
            this.getServerComponentNetcuts().addNetcut(newNetcut);
        }
        else {
            this.getClientComponentNetcuts().addNetcut(newNetcut);
        }
        newNetcut.processConData();
    }
    onComponentNetcutError(netcut) {
        this.getApp().getLog().error('Netcut error.');
        this.getApp().getErrorHandler().error('Netcut error.');
    }
}
exports.AppNetcuts = AppNetcuts;
//# sourceMappingURL=AppNetcuts.js.map