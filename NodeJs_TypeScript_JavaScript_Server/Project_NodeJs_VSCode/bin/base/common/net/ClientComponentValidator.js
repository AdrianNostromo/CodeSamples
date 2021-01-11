"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
const DataFormatError_1 = require("base/net/exception/DataFormatError");
const ComponentType_1 = require("base/common/ComponentType");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
const NetCodes_SC_UnvalidatedServerComponent_To_UnvalidatedClientComponent_1 = require("base/common/net/netcodes/NetCodes_SC_UnvalidatedServerComponent_To_UnvalidatedClientComponent");
const NetCodes_CS_UnvalidatedClientComponent_To_UnvalidatedServerComponent_1 = require("base/common/net/netcodes/NetCodes_CS_UnvalidatedClientComponent_To_UnvalidatedServerComponent");
class NetConValidationData {
    constructor(cbValidated, cbValidationFailed) {
        this.receiveIddleTimeS = 0;
        this.cbValidated = cbValidated;
        this.cbValidationFailed = cbValidationFailed;
    }
}
class ClientComponentValidator extends BasicAppComponent_1.BasicAppComponent {
    constructor(app) {
        super(app);
        this._ILoopUserBase_General = true;
        this._unconfirmedComponentCons = [];
        this._receiveIddleConnectionTimeLimitS = 5000; //asd_tst_002;// 5
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
                con.getUserData("validationData").receiveIddleTimeS += deltaS;
                if (con.getUserData("validationData").receiveIddleTimeS >= this._receiveIddleConnectionTimeLimitS) {
                    this.connectionErrorOccured(con);
                }
            }
        }
    }
    handleNewUnconfirmedComponent(con, cbValidated, cbValidationFailed) {
        let validationData = new NetConValidationData(cbValidated, cbValidationFailed);
        con.setUserData("validationData", validationData);
        this._unconfirmedComponentCons.push(con);
        con.setCbClose(this.onConDataError.bind(this));
        con.setCbError(this.onConDataError.bind(this));
        if (!this.sendMsg_REQUEST_COMPONENT_CONFIG(con)) {
            // Handle con error.
            this.connectionErrorOccured(con);
            return;
        }
    }
    sendMsg_REQUEST_COMPONENT_CONFIG(con) {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4 + 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_SC_UnvalidatedServerComponent_To_UnvalidatedClientComponent_1.NetCodes_SC_UnvalidatedServerComponent_To_UnvalidatedClientComponent.REQUEST_COMPONENT_CONFIG);
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
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    this.connectionErrorOccured(con);
                    return;
                }
                else {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    this.connectionErrorOccured(con);
                    return;
                }
            }
            if (!msgNetBuffer) {
                return;
            }
            con.getUserData("validationData").receiveIddleTimeS = 0;
            try {
                msgLen = msgNetBuffer.readInt();
                msgType = msgNetBuffer.readInt();
            }
            catch (err) {
                this.getApp().getLog().error('ServerComponentValidator. Logic error.');
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
                this.getApp().getLog().error('ClientComponentValidator. Unhandled message type: ' + msgType);
                this.connectionErrorOccured(con);
                return;
            }
        }
    }
    processMsg(con, msgLen, msgType, msgNetBuffer) {
        switch (msgType) {
            case NetCodes_CS_UnvalidatedClientComponent_To_UnvalidatedServerComponent_1.NetCodes_CS_UnvalidatedClientComponent_To_UnvalidatedServerComponent.RESPONSE_COMPONENT_CONFIG:
                let remoteComponentTypeID;
                try {
                    remoteComponentTypeID = msgNetBuffer.readInt();
                }
                catch (err) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                let remoteComponentType = ComponentType_1.ComponentType.fromID(remoteComponentTypeID);
                if (!remoteComponentType) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                // Component is valid.
                let index = this._unconfirmedComponentCons.indexOf(con);
                if (index < 0) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                this._unconfirmedComponentCons.splice(index, 1);
                let validationData = con.getUserData("validationData");
                this.sanitizeCon(con);
                validationData.cbValidated(con, remoteComponentType);
                break;
            default:
                return false;
        }
        return true;
    }
    onConDataError(con) {
        this.connectionErrorOccured(con);
    }
    connectionErrorOccured(con) {
        // The component validation failed.
        // Remove the con from the list. If the con is not in the list, it is already removed and return.
        let index = this._unconfirmedComponentCons.indexOf(con);
        if (index < 0) {
            // Already handled by a similar event.
            return;
        }
        this._unconfirmedComponentCons.splice(index, 1);
        let validationData = con.getUserData("validationData");
        this.sanitizeCon(con);
        validationData.cbValidationFailed(con);
    }
    sanitizeCon(con) {
        con.setUserData("validationData", null);
        con.setCbClose(null);
        con.setCbError(null);
    }
}
exports.ClientComponentValidator = ClientComponentValidator;
//# sourceMappingURL=ClientComponentValidator.js.map