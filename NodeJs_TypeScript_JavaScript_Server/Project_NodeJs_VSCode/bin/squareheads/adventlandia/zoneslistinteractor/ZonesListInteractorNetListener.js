"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ZonesListInteractorBase_1 = require("./ZonesListInteractorBase");
const TcpServer_1 = require("base/common/net/TcpServer");
const IpPort_1 = require("base/net/util/IpPort");
const GeneralTcpConnection_1 = require("base/common/net/tcp/connection/GeneralTcpConnection");
class ZonesListInteractorNetListener extends ZonesListInteractorBase_1.ZonesListInteractorBase {
    constructor(app) {
        super(app);
        this.netClientsListener = null;
        //void
    }
    create_framework() {
        super.create_framework();
        this.netClientsListener = new TcpServer_1.TcpServer(new IpPort_1.IpPort('127.0.0.1', 0), this.onNewUnvalidatedSocket.bind(this), this.onListenerNetError.bind(this), this.onListenerLogicError.bind(this), null);
        this.netClientsListener.create();
    }
    onNewUnvalidatedSocket(target, socket) {
        let con = new GeneralTcpConnection_1.GeneralTcpConnection(socket);
        this.onNewUnvalidatedCon(con);
    }
    onListenerNetError(target, error) {
        this.getApp().getLog().error('TcpServer. Error. Maybe the port is already in use.');
        this.getApp().getErrorHandler().error('TcpServer. Error. Maybe the port is already in use.');
    }
    onListenerLogicError(target, error) {
        this.getApp().getLog().error('TcpServer. Error. Maybe the port is already in use.');
        this.getApp().getErrorHandler().error('TcpServer. Error. Maybe the port is already in use.');
    }
    getClientsListenerIpPort() {
        if (this.netClientsListener != null) {
            let ret = this.netClientsListener.getIpPort();
            return ret;
        }
        return null;
    }
}
exports.ZonesListInteractorNetListener = ZonesListInteractorNetListener;
//# sourceMappingURL=ZonesListInteractorNetListener.js.map