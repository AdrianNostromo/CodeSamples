"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const DataFormatError_1 = require("base/net/exception/DataFormatError");
class NetcutBasic {
    constructor(app, con, componentType, isServerLocally) {
        this._cbError = null;
        this._receiveIddleTimeS = 0;
        this._sendIddleTimeS = 0;
        this._disconnectIddleThresholdTimeS = 150000; //asd_tst_001; 15
        this._iripMap = new Map();
        this._dataCache = new Map();
        this.app = app;
        this._con = con;
        this._componentType = componentType;
        this._isServerLocally = isServerLocally;
    }
    getApp() {
        return this.app;
    }
    create() {
        this._con.setCbClose(this.onConError.bind(this));
        this._con.setCbError(this.onConError.bind(this));
        this.create_framework();
        this.processConData();
    }
    create_framework() {
        //void
    }
    logicLoop(deltaS, deltaMS) {
        this._receiveIddleTimeS += deltaS;
        if (this._receiveIddleTimeS >= this._disconnectIddleThresholdTimeS) {
            // Remote host is unresponsize or connection lost.
            this.handleConError();
        }
        if (this._con.isData()) {
            this.processConData();
        }
    }
    onConError(con) {
        if (!con.getUserData("isErrorHandled")) {
            con.setUserData("isErrorHandled", true);
            this.handleConError();
        }
    }
    handleConError() {
        if (this._cbError) {
            this._cbError(this);
        }
    }
    setCbError(cbError) {
        this._cbError = cbError;
    }
    processConData() {
        while (true) {
            let msgNetBuffer;
            let msgLen;
            let msgType;
            try {
                msgNetBuffer = this._con.popNextMsg();
            }
            catch (x) {
                if (x instanceof DataFormatError_1.DataFormatError) {
                    this.handleConError();
                    return;
                }
                else {
                    this.handleConError();
                    return;
                }
            }
            if (!msgNetBuffer) {
                return;
            }
            this._receiveIddleTimeS = 0;
            try {
                msgLen = msgNetBuffer.readInt();
                msgType = msgNetBuffer.readInt();
            }
            catch (err) {
                this.handleConError();
                return;
            }
            let isMsgHandled;
            try {
                isMsgHandled = this.processMsg(msgLen, msgType, msgNetBuffer);
            }
            catch (x) {
                this.handleConError();
                return;
            }
            if (!isMsgHandled) {
                this.handleConError();
            }
            else {
                // Check if all the data was read.
                if (msgNetBuffer.getOffset() != msgNetBuffer.getBoundsLimit()) {
                    this.handleConError();
                }
            }
        }
    }
    processMsg(msgLen, msgType, msgNetBuffer) {
        return false;
    }
    getCon() {
        return this._con;
    }
    getComponentType() {
        return this._componentType;
    }
    sendNetBlock(netBlock) {
        this._con.sendBlock(netBlock);
        this._sendIddleTimeS = 0;
    }
    getIsServerLocally() {
        return this._isServerLocally;
    }
}
exports.NetcutBasic = NetcutBasic;
//# sourceMappingURL=NetcutBasic.js.map