"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const net = require("net");
const IpPort_1 = require("base/net/util/IpPort");
class TcpServer {
    constructor(initIpPort, cbNewSocket, cbNetError, cbLogicError, cbListenerActive) {
        this.postInitIpPort = null;
        this.server = null;
        this.isListenerActive = false;
        this.isListenerError = false;
        this.cbNewSocket = null;
        this.cbNetError = null;
        this.cbLogicError = null;
        this.cbListenerActive = null;
        this.initIpPort = initIpPort;
        this.cbNewSocket = cbNewSocket;
        this.cbNetError = cbNetError;
        this.cbLogicError = cbLogicError;
        this.cbListenerActive = cbListenerActive;
    }
    create() {
        this.server = net.createServer(this.onNewSocket.bind(this));
        this.server.on('error', this.onListenerError.bind(this));
        //511 is: Backlog is the maximum length of the queue of pending connections. The actual length will be determined by the OS through sysctl settings such as tcp_max_syn_backlog and somaxconn on Linux. The default value of this parameter is 511 (not 512).
        this.server.listen(this.initIpPort.port, this.initIpPort.ip, 511, this.onServerActive.bind(this));
    }
    onNewSocket(socket) {
        if (this.cbNewSocket != null) {
            this.cbNewSocket(this, socket);
        }
    }
    onListenerError(err) {
        this.isListenerError = true;
        this.isListenerActive = false;
        if (this.cbNetError != null) {
            this.cbNetError(this, err);
        }
    }
    onServerActive() {
        this.isListenerActive = true;
        let data = this.server.address();
        this.postInitIpPort = new IpPort_1.IpPort(data.address, data.port);
        if (this.cbListenerActive != null) {
            this.cbListenerActive(this);
        }
    }
    getIsListenerActive() {
        return this.isListenerActive;
    }
    getIsListenerError() {
        return this.isListenerError;
    }
    getIpPort() {
        if (!this.server) {
            throw new Error('Error.');
        }
        return this.postInitIpPort;
    }
    dispose() {
        this.cbNetError = null;
        this.cbLogicError = null;
        this.cbNewSocket = null;
        if (this.server != null) {
            this.server.close();
            this.server = null;
        }
    }
}
exports.TcpServer = TcpServer;
//# sourceMappingURL=TcpServer.js.map