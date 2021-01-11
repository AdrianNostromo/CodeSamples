"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
const GeneralTcpConnection_1 = require("base/common/net/tcp/connection/GeneralTcpConnection");
const net = require("net");
class ConnectingData {
    constructor(remoteIP, remotePort, remoteComponentType, cbConnected, cbFailed, con, conTrackID) {
        this.remoteIP = remoteIP;
        this.remotePort = remotePort;
        this.remoteComponentType = remoteComponentType;
        this.cbConnected = cbConnected;
        this.cbFailed = cbFailed;
        this.con = con;
        this.conTrackID = conTrackID;
    }
}
class TcpConnector extends BasicAppComponent_1.BasicAppComponent {
    constructor(app) {
        super(app);
        this._waitingConsList = [];
        this._conTrackIDCounter = 0;
        //void
    }
    create_framework() {
        super.create_framework();
        //void
    }
    newCon(remoteIP, remotePort, remoteComponentType, cbConnected, cbFailed) {
        let newCon = new GeneralTcpConnection_1.GeneralTcpConnection(new net.Socket());
        newCon.setCbConnect(this._onConnectionEstablished.bind(this));
        newCon.setCbClose(this._onConnectionFailed.bind(this));
        newCon.setCbError(this._onConnectionFailed.bind(this));
        let connectingData = new ConnectingData(remoteIP, remotePort, remoteComponentType, cbConnected, cbFailed, newCon, ++this._conTrackIDCounter);
        newCon.setUserData("connectingDataconnectingData", connectingData);
        newCon.getSocket().connect(remotePort, remoteIP);
        this._waitingConsList.push(newCon);
        return newCon;
    }
    _onConnectionEstablished(con) {
        let index = this._waitingConsList.indexOf(con);
        if (index < 0) {
            this.getApp().getLog().error('TcpConnector. Critical error.');
            this.getApp().getErrorHandler().error('TcpConnector. Critical error.');
            return;
        }
        this._waitingConsList.splice(index, 1);
        let connectingData = con.getUserData("connectingDataconnectingData");
        con.setUserData("connectingDataconnectingData", null);
        // Sanitize con.
        con.setCbConnect(null);
        con.setCbClose(null);
        con.setCbError(null);
        connectingData.cbConnected(connectingData.conTrackID, connectingData.con, connectingData.remoteComponentType);
    }
    _onConnectionFailed(con) {
        let index = this._waitingConsList.indexOf(con);
        if (index < 0) {
            // Already handled by a different fail event.
            return;
        }
        this._waitingConsList.splice(index, 1);
        let connectingData = con.getUserData("connectingDataconnectingData");
        con.setUserData("connectingDataconnectingData", null);
        // Sanitize con.
        con.setCbConnect(null);
        con.setCbClose(null);
        con.setCbError(null);
        connectingData.cbFailed(con, connectingData.conTrackID, connectingData.remoteComponentType);
    }
}
exports.TcpConnector = TcpConnector;
//# sourceMappingURL=TcpConnector.js.map