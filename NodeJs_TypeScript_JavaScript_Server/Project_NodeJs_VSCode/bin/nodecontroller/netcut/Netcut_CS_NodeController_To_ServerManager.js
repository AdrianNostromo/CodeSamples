"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ComponentType_1 = require("base/common/ComponentType");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
const NetCodes_SC_ServerManager_To_NodeController_1 = require("servermanager/netcode/NetCodes_SC_ServerManager_To_NodeController");
const NetCodes_CS_NodeController_To_ServerManager_1 = require("servermanager/netcode/NetCodes_CS_NodeController_To_ServerManager");
const Netcut_CS_Component_To_ServerManager_1 = require("base/servermanager/netcut/Netcut_CS_Component_To_ServerManager");
class Netcut_CS_NodeController_To_ServerManager extends Netcut_CS_Component_To_ServerManager_1.Netcut_CS_Component_To_ServerManager {
    constructor(app, con, componentType) {
        super(app, con, componentType);
        this._app = app;
    }
    getApp() {
        return this._app;
    }
    create_framework() {
        super.create_framework();
        //void
    }
    processMsg(msgLen, msgType, msgNetBuffer) {
        if (super.processMsg(msgLen, msgType, msgNetBuffer)) {
            return true;
        }
        switch (msgType) {
            case NetCodes_SC_ServerManager_To_NodeController_1.NetCodes_SC_ServerManager_To_NodeController.REQUEST_CREATE_COMPONENT:
                // Read the request data.
                let componentTypeID;
                try {
                    componentTypeID = msgNetBuffer.readInt();
                }
                catch (err) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                let componentType = ComponentType_1.ComponentType.fromID(componentTypeID);
                if (!componentType) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                let ret = this.getApp().getNodeController().componentsProcessHandler.createNewComponent(componentType);
                if (!ret) {
                    this.sendMsg_RESPONSE_CREATE_COMPONENT_ERROR(componentType);
                }
                else {
                    this.sendMsg_RESPONSE_CREATE_COMPONENT_OK(componentType);
                }
                break;
            default:
                return false;
        }
        return true;
    }
    sendMsg_RESPONSE_CREATE_COMPONENT_OK(componentType) {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4 + 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_CS_NodeController_To_ServerManager_1.NetCodes_CS_NodeController_To_ServerManager.RESPONSE_CREATE_COMPONENT_OK);
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
    sendMsg_RESPONSE_CREATE_COMPONENT_ERROR(componentType) {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4 + 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_CS_NodeController_To_ServerManager_1.NetCodes_CS_NodeController_To_ServerManager.RESPONSE_CREATE_COMPONENT_ERROR);
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
}
exports.Netcut_CS_NodeController_To_ServerManager = Netcut_CS_NodeController_To_ServerManager;
//# sourceMappingURL=Netcut_CS_NodeController_To_ServerManager.js.map