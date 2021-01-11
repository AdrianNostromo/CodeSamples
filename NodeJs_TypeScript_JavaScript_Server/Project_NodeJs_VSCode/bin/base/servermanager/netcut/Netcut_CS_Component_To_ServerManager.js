"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Netcut_CS_ClientComponent_To_ServerComponent_1 = require("base/common/net/netcut/Netcut_CS_ClientComponent_To_ServerComponent");
const ComponentType_1 = require("base/common/ComponentType");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
const ComponentStatus_1 = require("base/common/util/ComponentStatus");
const ServerStatus_1 = require("base/common/server/ServerStatus");
const IpPort_1 = require("base/net/util/IpPort");
const NetCodes_SC_ServerManager_To_ClientComponent_1 = require("servermanager/netcode/NetCodes_SC_ServerManager_To_ClientComponent");
const NetCodes_CS_ClientComponent_To_ServerManager_1 = require("servermanager/netcode/NetCodes_CS_ClientComponent_To_ServerManager");
class Netcut_CS_Component_To_ServerManager extends Netcut_CS_ClientComponent_To_ServerComponent_1.Netcut_CS_ClientComponent_To_ServerComponent {
    constructor(app, con, componentType) {
        super(app, con, componentType);
        //void
    }
    create_framework() {
        super.create_framework();
        //void
    }
    processMsg(msgLen, msgType, msgNetBuffer) {
        if (super.processMsg(msgLen, msgType, msgNetBuffer)) {
            return true;
        }
        let targetComponentTypeID;
        let targetComponentType;
        switch (msgType) {
            case NetCodes_SC_ServerManager_To_ClientComponent_1.NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_SYNC_COMPONENT_LISTENER_IP_PORT_OK:
                //void
                break;
            case NetCodes_SC_ServerManager_To_ClientComponent_1.NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_OK:
                // Read the data.
                let ip;
                let port;
                try {
                    targetComponentTypeID = msgNetBuffer.readInt();
                    ip = msgNetBuffer.readString();
                    port = msgNetBuffer.readInt();
                }
                catch (err) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                targetComponentType = ComponentType_1.ComponentType.fromID(targetComponentTypeID);
                if (!targetComponentType) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                // Save the component listener information.
                let ipPort = new IpPort_1.IpPort(ip, port);
                // Push the data to the app.getComponentDependenciesHandler().
                if (!this.getApp().getComponentDependenciesHandler().addDependencyComponentListenerIpPort(targetComponentType, ipPort)) {
                    // Not error because debug at a certain step on the server Manager may cause this request to be sent multiple times before the response is received.
                    //this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    //return false;
                }
                break;
            case NetCodes_SC_ServerManager_To_ClientComponent_1.NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_COMPONENT_NOT_CONNECTED:
                // Read the data.
                try {
                    targetComponentTypeID = msgNetBuffer.readInt();
                }
                catch (err) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                targetComponentType = ComponentType_1.ComponentType.fromID(targetComponentTypeID);
                if (!targetComponentType) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                //void
                break;
            case NetCodes_SC_ServerManager_To_ClientComponent_1.NetCodes_SC_ServerManager_To_ClientComponent.REQUEST_SET_SERVER_STATUS:
                // Read the data.
                let serverStatusId;
                try {
                    serverStatusId = msgNetBuffer.readInt();
                }
                catch (err) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                let serverStatus = ServerStatus_1.ServerStatus.fromID(serverStatusId);
                if (!serverStatus) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                // Save the value.
                this.getApp().setServerStatus(serverStatus);
                // Send response.
                if (!this.sendMsg_RESPONSE_SET_SERVER_STATUS_OK()) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                break;
            default:
                return false;
        }
        return true;
    }
    syncComponentStatus(componentStatus) {
        if (componentStatus == ComponentStatus_1.ComponentStatus.ONLINE_FOR_COMPONENTS && this.getApp().isListenerEnabled()) {
            let ipPort = this.getApp().getListenerIpPort();
            if (!ipPort) {
                this.handleConError();
                return;
            }
            if (!this.sendMsg_REQUEST_SYNC_COMPONENT_LISTENER_IP_PORT(ipPort)) {
                this.handleConError();
                return;
            }
        }
        super.syncComponentStatus(componentStatus);
    }
    sendMsg_REQUEST_SYNC_COMPONENT_LISTENER_IP_PORT(ipPort) {
        if (!ipPort) {
            return false;
        }
        let ipStringLen = Buffer.byteLength(ipPort.ip, 'utf8');
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4 + (2 + ipStringLen) + 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_CS_ClientComponent_To_ServerManager_1.NetCodes_CS_ClientComponent_To_ServerManager.REQUEST_SYNC_COMPONENT_LISTENER_IP_PORT);
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
    sendMsg_REQUEST_COMPONENT_LISTENER_IP_PORT_FROM_TYPE(componentType) {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4 + 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_CS_ClientComponent_To_ServerManager_1.NetCodes_CS_ClientComponent_To_ServerManager.REQUEST_COMPONENT_LISTENER_IP_PORT_FROM_TYPE);
            netBlock.writeInt(componentType.id);
            // END : msg data;
            netBlock.endMsgWrite();
            this.sendNetBlock(netBlock);
        }
        catch (err) {
            return false;
        }
        return true;
    }
    sendMsg_RESPONSE_SET_SERVER_STATUS_OK() {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_CS_ClientComponent_To_ServerManager_1.NetCodes_CS_ClientComponent_To_ServerManager.RESPONSE_SET_SERVER_STATUS_OK);
            // END : msg data;
            netBlock.endMsgWrite();
            this.sendNetBlock(netBlock);
        }
        catch (err) {
            return false;
        }
        return true;
    }
    requestComponentListenerIpPort(componentType) {
        if (!this.sendMsg_REQUEST_COMPONENT_LISTENER_IP_PORT_FROM_TYPE(componentType)) {
            this.handleConError();
            return false;
        }
        return true;
    }
}
exports.Netcut_CS_Component_To_ServerManager = Netcut_CS_Component_To_ServerManager;
//# sourceMappingURL=Netcut_CS_Component_To_ServerManager.js.map