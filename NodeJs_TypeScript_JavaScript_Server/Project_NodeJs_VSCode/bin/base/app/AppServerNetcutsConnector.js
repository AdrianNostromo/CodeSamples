"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const AppClientNetcutsListener_1 = require("./AppClientNetcutsListener");
const ServerComponentValidator_1 = require("base/common/net/ServerComponentValidator");
const TcpConnector_1 = require("base/common/net/TcpConnector");
class AppServerNetcutsConnector extends AppClientNetcutsListener_1.AppClientNetcutsListener {
    constructor(componentType, dirs, netcutsFactory, clientListenerNetConfig, serverConnectorNetConfig) {
        super(componentType, dirs, netcutsFactory, clientListenerNetConfig);
        this._serverComponentValidator = null;
        this._serverComponentConnector = null;
        this.serverConnectorNetConfig = serverConnectorNetConfig;
    }
    create_framework() {
        super.create_framework();
        if (this.serverConnectorNetConfig) {
            this.addComponent(this._serverComponentValidator = new ServerComponentValidator_1.ServerComponentValidator(this.getApp()));
            this.addComponent(this._serverComponentConnector = new TcpConnector_1.TcpConnector(this.getApp()));
        }
    }
    connectToServerComponent(componentType, ip, port, cbHolder) {
        let retCon = this._serverComponentConnector.newCon(ip, port, componentType, this.onUnvalidatedServerCon.bind(this), this.onServerConFailed.bind(this));
        if (!retCon) {
            this.getApp().getLog().error('Server component connection validation failed.' + '. Treated a warning, not critical at this location.');
            this.getApp().getErrorHandler().error('Server component connection validation failed.');
            return false;
        }
        retCon.setUserData("connectionCbHolder", cbHolder);
        return true;
    }
    onUnvalidatedServerCon(conTrackID, con, remoteComponentType) {
        // Connection established to a server, validate it.
        let cbHolder = con.getUserData("connectionCbHolder");
        if (cbHolder && cbHolder.cbUnvalidatedConnection) {
            cbHolder.cbUnvalidatedConnection(con, remoteComponentType);
        }
        this._serverComponentValidator.handleNewUnconfirmedComponent(con, remoteComponentType, this.onServerConValidated.bind(this), this.onServerConValidationFailed.bind(this));
    }
    onServerConFailed(con, conTrackID, remoteComponentType) {
        // Post a warning and try again.
        let cbHolder = con.getUserData("connectionCbHolder");
        if (cbHolder && cbHolder.cbConnectionFail) {
            cbHolder.cbConnectionFail(con, remoteComponentType);
        }
        con.setUserData("connectionCbHolder", null);
    }
    onServerConValidated(con, remoteComponentType) {
        let cbHolder = con.getUserData("connectionCbHolder");
        if (cbHolder && cbHolder.cbValidated) {
            cbHolder.cbValidated(con, remoteComponentType);
        }
        con.setUserData("connectionCbHolder", null);
        this.pushNetcut(remoteComponentType, con, false);
    }
    onServerConValidationFailed(con, remoteComponentType) {
        this.getApp().getLog().error('Server component connection validation failed.' + '. Treated a warning, not critical at this location.');
        this.getApp().getErrorHandler().error('Server component connection validation failed.');
        return;
    }
}
exports.AppServerNetcutsConnector = AppServerNetcutsConnector;
//# sourceMappingURL=AppServerNetcutsConnector.js.map