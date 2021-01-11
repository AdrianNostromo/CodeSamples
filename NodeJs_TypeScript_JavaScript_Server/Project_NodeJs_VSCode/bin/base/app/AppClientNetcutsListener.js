"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const AppNetcuts_1 = require("./AppNetcuts");
const ClientComponentValidator_1 = require("base/common/net/ClientComponentValidator");
const TcpServer_1 = require("base/common/net/TcpServer");
const GeneralTcpConnection_1 = require("base/common/net/tcp/connection/GeneralTcpConnection");
const ComponentStatus_1 = require("base/common/util/ComponentStatus");
class AppClientNetcutsListener extends AppNetcuts_1.AppNetcuts {
    constructor(componentType, dirs, netcutsFactory, clientListenerNetConfig) {
        super(componentType, dirs, netcutsFactory);
        this._clientComponentValidator = null;
        this._clientComponentListener = null;
        this._isListenerStarted = false;
        this._bf_onClientComponentValidated = null;
        this._bf_onClientComponentValidationFailed = null;
        this._clientListenerNetConfig = clientListenerNetConfig;
    }
    create_framework() {
        super.create_framework();
        if (this.isListenerEnabled()) {
            this.addComponent(this._clientComponentValidator = new ClientComponentValidator_1.ClientComponentValidator(this.getApp()));
            this._bf_onClientComponentValidated = this.onClientComponentValidated.bind(this);
            this._bf_onClientComponentValidationFailed = this.onClientComponentValidationFailed.bind(this);
            this._clientComponentListener = new TcpServer_1.TcpServer(this._clientListenerNetConfig.ipPort, this.onUnconfirmedComponentSocket.bind(this), this.onComponentsListenerNetError.bind(this), this.onComponentsListenerLogicError.bind(this), this.onComponentsListenerActive.bind(this));
            this._clientComponentListener.create();
        }
    }
    onComponentsListenerActive(target) {
        let ipPort = target.getIpPort();
        this.getApp().getLog().info('Client components listener active on ip: ' + ipPort.ip + ' port: ' + ipPort.port);
    }
    onComponentsListenerNetError(target, err) {
        this.getApp().getLog().error('TcpServer. Error. Maybe the port is already in use.');
        this.getApp().getErrorHandler().error('TcpServer. Error. Maybe the port is already in use.');
    }
    onComponentsListenerLogicError(target, err) {
        this.getApp().getLog().error('TcpServer. Error. Maybe the port is already in use.');
        this.getApp().getErrorHandler().error('TcpServer. Error. Maybe the port is already in use.');
    }
    onUnconfirmedComponentSocket(target, socket) {
        if (!this._isListenerStarted) {
            if (!socket.destroyed) {
                socket.destroy();
            }
            return;
        }
        let con = new GeneralTcpConnection_1.GeneralTcpConnection(socket);
        this._clientComponentValidator.handleNewUnconfirmedComponent(con, this._bf_onClientComponentValidated, this._bf_onClientComponentValidationFailed);
    }
    onClientComponentValidated(con, remoteComponentType) {
        // Add the component to the child components handler. Wrap the con in custom Netcut classes that process the messages.
        this.getApp().getLog().info('Client component validated. Type: ' + remoteComponentType.name);
        this.pushNetcut(remoteComponentType, con, true);
    }
    onClientComponentValidationFailed(con) {
        this.getApp().getLog().warning('Client component validation failed.');
    }
    componentStatusChanged(componentStatus) {
        super.componentStatusChanged(componentStatus);
        if (componentStatus == ComponentStatus_1.ComponentStatus.STARTING) {
            if (!this._isListenerStarted && this._clientComponentListener) {
                this._isListenerStarted = true;
            }
        }
    }
    isComponentTransientStateFinished(componentStatus) {
        let ret = super.isComponentTransientStateFinished(componentStatus);
        if (componentStatus == ComponentStatus_1.ComponentStatus.STARTING) {
            if (this._clientComponentListener && !this._clientComponentListener.getIsListenerActive()) {
                ret = false;
            }
        }
        return ret;
    }
    isListenerEnabled() {
        if (this._clientListenerNetConfig) {
            return true;
        }
        return false;
    }
    getListenerIpPort() {
        if (!this.isListenerEnabled() || !this._clientComponentListener) {
            return null;
        }
        return this._clientComponentListener.getIpPort();
    }
}
exports.AppClientNetcutsListener = AppClientNetcutsListener;
//# sourceMappingURL=AppClientNetcutsListener.js.map