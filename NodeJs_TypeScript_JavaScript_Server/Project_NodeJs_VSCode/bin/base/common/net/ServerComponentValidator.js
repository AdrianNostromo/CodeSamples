"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
const DataFormatError_1 = require("base/net/exception/DataFormatError");
const ComponentType_1 = require("base/common/ComponentType");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
const NetCodes_SC_UnvalidatedServerComponent_To_UnvalidatedClientComponent_1 = require("base/common/net/netcodes/NetCodes_SC_UnvalidatedServerComponent_To_UnvalidatedClientComponent");
const NetCodes_CS_UnvalidatedClientComponent_To_UnvalidatedServerComponent_1 = require("base/common/net/netcodes/NetCodes_CS_UnvalidatedClientComponent_To_UnvalidatedServerComponent");
class ValidatingConData {
    constructor(remoteComponentType, cbValidated, cbFailed) {
        this.remoteComponentType = remoteComponentType;
        this.cbValidated = cbValidated;
        this.cbFailed = cbFailed;
    }
}
class ServerComponentValidator extends BasicAppComponent_1.BasicAppComponent {
    constructor(app) {
        super(app);
        this._ILoopUserBase_General = true;
        this._unconfirmedComponentCons = [];
        this._receiveIddleConnectionTimeLimitS = 500000; //asd_tst_001; 5
        //void
    }
    create_framework() {
        super.create_framework();
        //void
    }
    appLoop_General(deltaS, deltaMS) {
        for (let i = this._unconfirmedComponentCons.length - 1; i >= 0; i--) {
            let con = this._unconfirmedComponentCons[i];
            if (con) {
                if (con.isData()) {
                    this.processConData(con);
                }
            }
        }
        for (let i = this._unconfirmedComponentCons.length - 1; i >= 0; i--) {
            let con = this._unconfirmedComponentCons[i];
            if (con) {
                con.getUserData("validatingData").receiveIddleTimeS += deltaS;
                if (con.getUserData("validatingData").receiveIddleTimeS >= this._receiveIddleConnectionTimeLimitS) {
                    this.connectionErrorOccured(con);
                }
            }
        }
    }
    handleNewUnconfirmedComponent(con, remoteComponentType, cbValidated, cbFailed) {
        con.setUserData("validatingData", new ValidatingConData(remoteComponentType, cbValidated, cbFailed));
        this._unconfirmedComponentCons.push(con);
        con.setCbClose(this.onConnectionError.bind(this));
        con.setCbError(this.onConnectionError.bind(this));
        this.processConData(con);
    }
    processConData(con) {
        while (true) {
            let msgNetBuffer;
            let msgLen;
            let msgType;
            try {
                msgNetBuffer = con.popNextMsg();
            }
            catch (x) {
                if (x instanceof DataFormatError_1.DataFormatError) {
                    this.connectionErrorOccured(con);
                    return;
                }
                else {
                    this.connectionErrorOccured(con);
                    return;
                }
            }
            if (!msgNetBuffer) {
                return;
            }
            con.getUserData("validatingData").receiveIddleTimeS = 0;
            try {
                msgLen = msgNetBuffer.readInt();
                msgType = msgNetBuffer.readInt();
            }
            catch (err) {
                this.connectionErrorOccured(con);
                return;
            }
            let isMsgHandled;
            try {
                isMsgHandled = this.processMsg(con, msgLen, msgType, msgNetBuffer);
            }
            catch (x) {
                this.getApp().getLog().error('ClientComponentValidator. Unhandled message type: ' + msgType);
                this.connectionErrorOccured(con);
                return;
            }
            if (!isMsgHandled) {
                this.connectionErrorOccured(con);
                return;
            }
        }
    }
    processMsg(con, msgLen, msgType, msgNetBuffer) {
        switch (msgType) {
            case NetCodes_SC_UnvalidatedServerComponent_To_UnvalidatedClientComponent_1.NetCodes_SC_UnvalidatedServerComponent_To_UnvalidatedClientComponent.REQUEST_COMPONENT_CONFIG:
                let remoteComponentTypeID = msgNetBuffer.readInt();
                let remoteComponentType = ComponentType_1.ComponentType.fromID(remoteComponentTypeID);
                if (!remoteComponentType) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                if (remoteComponentType != con.getUserData("validatingData").remoteComponentType) {
                    // The remote component is not of the expected type.
                    this.getApp().getLog().error('ServerComponentValidator. Logic error 2.');
                    return false;
                }
                if (!this.sendMsg_RESPONSE_COMPONENT_CONFIG(con)) {
                    // Handle con error.
                    this.getApp().getLog().error('ServerComponentValidator. Logic error 2.');
                    return false;
                }
                this.validationOccured(con);
                break;
            default:
                return false;
        }
        return true;
    }
    sendMsg_RESPONSE_COMPONENT_CONFIG(con) {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4 + 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_CS_UnvalidatedClientComponent_To_UnvalidatedServerComponent_1.NetCodes_CS_UnvalidatedClientComponent_To_UnvalidatedServerComponent.RESPONSE_COMPONENT_CONFIG);
            netBlock.writeInt(this.getApp().getComponentType().id);
            // END : msg data;
            netBlock.endMsgWrite();
            con.sendBlock(netBlock);
        }
        catch (err) {
            return false;
        }
        return true;
    }
    validationOccured(con) {
        let index = this._unconfirmedComponentCons.indexOf(con);
        if (index < 0) {
            // Already handled by a different fail event.
            return;
        }
        this._unconfirmedComponentCons.splice(index, 1);
        let validatingData = con.getUserData("validatingData");
        this.sanitizeCon(con);
        validatingData.cbValidated(con, validatingData.remoteComponentType);
    }
    onConnectionError(con) {
        this.connectionErrorOccured(con);
    }
    connectionErrorOccured(con) {
        let index = this._unconfirmedComponentCons.indexOf(con);
        if (index < 0) {
            // Already handled by a different fail event.
            return;
        }
        this._unconfirmedComponentCons.splice(index, 1);
        let validatingData = con.getUserData("validatingData");
        this.sanitizeCon(con);
        validatingData.cbFailed(con, validatingData.remoteComponentType);
    }
    sanitizeCon(con) {
        con.setUserData("validatingData", null);
        con.setCbClose(null);
        con.setCbError(null);
    }
}
exports.ServerComponentValidator = ServerComponentValidator;
//# sourceMappingURL=ServerComponentValidator.js.map