"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWIBase_1 = require("./GWIBase");
const TcpServer_1 = require("base/common/net/TcpServer");
const IpPort_1 = require("base/net/util/IpPort");
const GeneralTcpConnection_1 = require("base/common/net/tcp/connection/GeneralTcpConnection");
const GlobalAccessLogger_1 = require("base/log/GlobalAccessLogger");
class GWINetListener extends GWIBase_1.GWIBase {
    constructor(handler, config) {
        super(handler, config);
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
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug('New listener socket(GameWorld_' + this.getServerInstanceId() + '). Client port: ' + socket.remotePort);
        this.onNewUnvalidatedCon(con);
    }
    onListenerNetError(error) {
        this.netError("Error.GWINetListener.1.");
    }
    onListenerLogicError(error) {
        this.logicError("Error.onListenerLogicError.1.");
    }
    checkIsInitFinished() {
        if (!super.checkIsInitFinished()) {
            return false;
        }
        if (this.netClientsListener == null || !this.netClientsListener.getIsListenerActive()) {
            return false;
        }
        return true;
    }
    getPlayerServerIpPort() {
        if (this.netClientsListener != null) {
            let ret = this.netClientsListener.getIpPort();
            return ret;
        }
        return null;
    }
}
exports.GWINetListener = GWINetListener;
//# sourceMappingURL=GWINetListener.js.map