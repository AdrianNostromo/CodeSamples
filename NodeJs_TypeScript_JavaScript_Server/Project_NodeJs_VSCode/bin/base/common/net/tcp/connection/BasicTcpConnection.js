"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const StreamBuffer_1 = require("base/buffer/StreamBuffer");
class BasicTcpConnection {
    constructor(socket) {
        this.userDataMap = new Map();
        this._receivedNetBuffersList = [];
        this._cbConnect = null;
        this._cbClose = null;
        this._cbError = null;
        this._socket = socket;
        // Bug workaround.
        // Simulate this._socket.setEncoding(null);
        if (this._socket._readableState) {
            delete this._socket._readableState.decoder;
            delete this._socket._readableState.encoding;
        }
        this._socket.on('connect', this.onSocketConnect.bind(this));
        this._socket.on('data', this.onSocketData.bind(this));
        this._socket.on('error', this.onSocketError.bind(this));
        this._socket.on('close', this.onSocketClose.bind(this));
    }
    onSocketConnect() {
        if (this._cbConnect) {
            this._cbConnect(this);
        }
    }
    onSocketData(data) {
        this.cacheInputBitesToBufferList(data);
    }
    cacheInputBitesToBufferList(data) {
        let newBuff = new StreamBuffer_1.StreamBuffer(data);
        newBuff.setBoundsLimit(0);
        this._receivedNetBuffersList.push(newBuff);
    }
    onSocketError(err) {
        if (this._cbError) {
            this._cbError(this);
        }
    }
    onSocketClose(had_error) {
        if (this._cbClose) {
            this._cbClose(this);
        }
    }
    isData() {
        if (this._receivedNetBuffersList.length > 0) {
            return true;
        }
        return false;
    }
    getUserData(key) {
        return this.userDataMap.get(key);
    }
    setUserData(key, value) {
        this.userDataMap.set(key, value);
    }
    setCbConnect(cbConnect) {
        this._cbConnect = cbConnect;
    }
    setCbClose(cbClose) {
        this._cbClose = cbClose;
    }
    setCbError(cbError) {
        this._cbError = cbError;
    }
    getSocket() {
        return this._socket;
    }
    forceClose() {
        if (!this._socket.destroyed) {
            this._socket.destroy();
        }
    }
}
exports.BasicTcpConnection = BasicTcpConnection;
//# sourceMappingURL=BasicTcpConnection.js.map