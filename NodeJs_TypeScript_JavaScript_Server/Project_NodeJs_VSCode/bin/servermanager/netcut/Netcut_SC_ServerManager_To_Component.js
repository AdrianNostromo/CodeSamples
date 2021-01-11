"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Netcut_SC_ServerComponent_To_ClientComponent_1 = require("base/common/net/netcut/Netcut_SC_ServerComponent_To_ClientComponent");
const ComponentType_1 = require("base/common/ComponentType");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
const IpPort_1 = require("base/net/util/IpPort");
const ServerStatus_1 = require("base/common/server/ServerStatus");
const NetCodes_SC_ServerManager_To_ClientComponent_1 = require("servermanager/netcode/NetCodes_SC_ServerManager_To_ClientComponent");
const NetCodes_CS_ClientComponent_To_ServerManager_1 = require("servermanager/netcode/NetCodes_CS_ClientComponent_To_ServerManager");
class Netcut_SC_ServerManager_To_Component extends Netcut_SC_ServerComponent_To_ClientComponent_1.Netcut_SC_ServerComponent_To_ClientComponent {
    constructor(app, con, componentType) {
        super(app, con, componentType);
        this._remoteComponentListenerIpPort = null;
        this._sd_serverStatus = ServerStatus_1.ServerStatus.OFFLINE;
        //void
    }
    create_framework() {
        super.create_framework();
        //void
    }
    logicLoop(deltaS, deltaMS) {
        super.logicLoop(deltaS, deltaMS);
        if (this._sd_serverStatus != this.getApp().getServerStatus()) {
            this._sd_serverStatus = this.getApp().getServerStatus();
            if (!this.sendMsg_REQUEST_SET_SERVER_STATUS(this._sd_serverStatus)) {
                this.handleConError();
                return;
            }
        }
    }
    processMsg(msgLen, msgType, msgNetBuffer) {
        if (super.processMsg(msgLen, msgType, msgNetBuffer)) {
            return true;
        }
        switch (msgType) {
            case NetCodes_CS_ClientComponent_To_ServerManager_1.NetCodes_CS_ClientComponent_To_ServerManager.REQUEST_SYNC_COMPONENT_LISTENER_IP_PORT:
                if (this._remoteComponentListenerIpPort) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                let ip;
                let port;
                try {
                    ip = msgNetBuffer.readString();
                    port = msgNetBuffer.readInt();
                }
                catch (err) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                // Save the component listener information.
                this._remoteComponentListenerIpPort = new IpPort_1.IpPort(ip, port);
                if (!this.sendMsg_RESPONSE_SYNC_COMPONENT_LISTENER_IP_PORT_OK()) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                break;
            case NetCodes_CS_ClientComponent_To_ServerManager_1.NetCodes_CS_ClientComponent_To_ServerManager.REQUEST_COMPONENT_LISTENER_IP_PORT_FROM_TYPE:
                // Read the request data.
                let targetComponentTypeID;
                try {
                    targetComponentTypeID = msgNetBuffer.readInt();
                }
                catch (err) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                let targetComponentType = ComponentType_1.ComponentType.fromID(targetComponentTypeID);
                if (!targetComponentType) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                // Pool the response data.
                let targetComponentNetcut = this.getApp().getClientComponentNetcuts().getDefaultOnlineNetcut(targetComponentType);
                if (!targetComponentNetcut) {
                    // The component has not connected yet.
                    this.sendMsg_RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_COMPONENT_NOT_CONNECTED(targetComponentType);
                }
                else {
                    let componentListenerIpPort = targetComponentNetcut.getRemoteComponentListenerIpPort();
                    // Return the response data.
                    this.sendMsg_RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_OK(targetComponentType, componentListenerIpPort);
                }
                break;
            case NetCodes_CS_ClientComponent_To_ServerManager_1.NetCodes_CS_ClientComponent_To_ServerManager.RESPONSE_SET_SERVER_STATUS_OK:
                //void
                break;
            default:
                return false;
        }
        return true;
    }
    sendMsg_RESPONSE_SYNC_COMPONENT_LISTENER_IP_PORT_OK() {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_SC_ServerManager_To_ClientComponent_1.NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_SYNC_COMPONENT_LISTENER_IP_PORT_OK);
            // END : msg data;
            netBlock.endMsgWrite();
            this.sendNetBlock(netBlock);
        }
        catch (err) {
            return false;
        }
        return true;
    }
    sendMsg_RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_OK(targetComponentType, ipPort) {
        if (!ipPort) {
            return false;
        }
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4 + 4 + (2 + Buffer.byteLength(ipPort.ip, 'utf8')) + 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_SC_ServerManager_To_ClientComponent_1.NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_OK);
            netBlock.writeInt(targetComponentType.id);
            netBlock.writeString(ipPort.ip);
            netBlock.writeInt(ipPort.port);
            // END : msg data;
            netBlock.endMsgWrite();
            this.sendNetBlock(netBlock);
        }
        catch (err) {
            return false;
        }
        return true;
    }
    sendMsg_RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_COMPONENT_NOT_CONNECTED(targetComponentType) {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4 + 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_SC_ServerManager_To_ClientComponent_1.NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_COMPONENT_NOT_CONNECTED);
            netBlock.writeInt(targetComponentType.id);
            // END : msg data;
            netBlock.endMsgWrite();
            this.sendNetBlock(netBlock);
        }
        catch (err) {
            return false;
        }
        return true;
    }
    sendMsg_REQUEST_SET_SERVER_STATUS(serverStatus) {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4 + 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_SC_ServerManager_To_ClientComponent_1.NetCodes_SC_ServerManager_To_ClientComponent.REQUEST_SET_SERVER_STATUS);
            netBlock.writeInt(serverStatus.id);
            // END : msg data;
            netBlock.endMsgWrite();
            this.sendNetBlock(netBlock);
        }
        catch (err) {
            return false;
        }
        return true;
    }
    getRemoteComponentListenerIpPort() {
        return this._remoteComponentListenerIpPort;
    }
}
exports.Netcut_SC_ServerManager_To_Component = Netcut_SC_ServerManager_To_Component;
//# sourceMappingURL=Netcut_SC_ServerManager_To_Component.js.map