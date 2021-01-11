"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const NetcutBasic_1 = require("./NetcutBasic");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
const ComponentStatus_1 = require("base/common/util/ComponentStatus");
const NetCodes_Component_To_Component_1 = require("base/common/net/netcodes/NetCodes_Component_To_Component");
const CallbackRequest_1 = require("./util/CallbackRequest");
class Netcut_Component_To_Component extends NetcutBasic_1.NetcutBasic {
    constructor(app, con, componentType, isServerLocally) {
        super(app, con, componentType, isServerLocally);
        this._remoteComponentStatus = null;
        this._sd_componentStatus = null;
        this._cbRemoteComponentStatusChanged = null;
        this._activeCallbackedRequestsList = [];
        this._activeCallbackOriginIdCounter = 0;
        //void
    }
    create_framework() {
        super.create_framework();
        //void
    }
    logicLoop(deltaS, deltaMS) {
        super.logicLoop(deltaS, deltaMS);
        if (this._sd_componentStatus != this.getApp().getComponentStatus()) {
            this._sd_componentStatus = this.getApp().getComponentStatus();
            let s = 'Sending component status. Remote component type: ' + this._componentType.name + '. Local component status: ' + this._sd_componentStatus.name + ' Remote component status: ';
            if (this._remoteComponentStatus) {
                s += this._remoteComponentStatus.name;
            }
            else {
                s += 'null';
            }
            //this.getApp().getLog().info(s);
            this.syncComponentStatus(this._sd_componentStatus);
        }
    }
    // Extended.
    syncComponentStatus(componentStatus) {
        if (!this.sendMsg_REQUEST_COMPONENT_STATUS_CHANGED(this._sd_componentStatus)) {
            this.handleConError();
            return;
        }
    }
    processMsg(msgLen, msgType, msgNetBuffer) {
        if (super.processMsg(msgLen, msgType, msgNetBuffer)) {
            return true;
        }
        switch (msgType) {
            case NetCodes_Component_To_Component_1.NetCodes_Component_To_Component.REQUEST_COMPONENT_STATUS_CHANGED:
                let remoteComponentStatusID;
                try {
                    remoteComponentStatusID = msgNetBuffer.readInt();
                }
                catch (err) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                let remoteComponentStatusType = ComponentStatus_1.ComponentStatus.fromID(remoteComponentStatusID);
                if (!remoteComponentStatusType) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                this._remoteComponentStatus = remoteComponentStatusType;
                let s = 'Remote component status received. Remote component type: ' + this._componentType.name + '. Remote component status: ' + this._remoteComponentStatus.name + ' Local actual component status: ' + this.getApp().getComponentStatus().name + ' Local component synked status: ';
                if (this._sd_componentStatus) {
                    s += this._sd_componentStatus.name;
                }
                else {
                    s += 'null';
                }
                //this.getApp().getLog().info(s);
                if (this._cbRemoteComponentStatusChanged) {
                    this._cbRemoteComponentStatusChanged(this);
                }
                if (!this.sendMsg_RESPONSE_COMPONENT_STATUS_CHANGED_OK()) {
                    this.getApp().getLog().warning('ServerComponentValidator. Logic error.');
                    return false;
                }
                break;
            case NetCodes_Component_To_Component_1.NetCodes_Component_To_Component.RESPONSE_COMPONENT_STATUS_CHANGED_OK:
                //void
                break;
            default:
                return false;
        }
        return true;
    }
    sendMsg_REQUEST_COMPONENT_STATUS_CHANGED(componentStatus) {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4 + 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_Component_To_Component_1.NetCodes_Component_To_Component.REQUEST_COMPONENT_STATUS_CHANGED);
            netBlock.writeInt(componentStatus.id);
            // END : msg data;
            netBlock.endMsgWrite();
            this.sendNetBlock(netBlock);
        }
        catch (err) {
            return false;
        }
        return true;
    }
    sendMsg_RESPONSE_COMPONENT_STATUS_CHANGED_OK() {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_Component_To_Component_1.NetCodes_Component_To_Component.RESPONSE_COMPONENT_STATUS_CHANGED_OK);
            // END : msg data;
            netBlock.endMsgWrite();
            this.sendNetBlock(netBlock);
        }
        catch (err) {
            return false;
        }
        return true;
    }
    getRemoteComponentStatus() {
        return this._remoteComponentStatus;
    }
    setCbRemoteComponentStatusChanged(cbRemoteComponentStatusChanged) {
        this._cbRemoteComponentStatusChanged = cbRemoteComponentStatusChanged;
    }
    pushNewActiveCallbackRequest() {
        this._activeCallbackOriginIdCounter++;
        let newRequest = new CallbackRequest_1.CallbackRequest(this._activeCallbackOriginIdCounter);
        this._activeCallbackedRequestsList.push(newRequest);
        return newRequest;
    }
    popActiveCallbackRequest(originID) {
        for (let i = 0; i < this._activeCallbackedRequestsList.length; i++) {
            let activeRequestEntry = this._activeCallbackedRequestsList[i];
            if (activeRequestEntry.originID == originID) {
                this._activeCallbackedRequestsList.splice(i, 1);
                return activeRequestEntry;
            }
        }
        return null;
    }
}
exports.Netcut_Component_To_Component = Netcut_Component_To_Component;
//# sourceMappingURL=Netcut_Component_To_Component.js.map