"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Netcut_SC_ServerManager_To_Component_1 = require("./Netcut_SC_ServerManager_To_Component");
const ComponentType_1 = require("base/common/ComponentType");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
const NetCodes_SC_ServerManager_To_NodeController_1 = require("servermanager/netcode/NetCodes_SC_ServerManager_To_NodeController");
const NetCodes_CS_NodeController_To_ServerManager_1 = require("servermanager/netcode/NetCodes_CS_NodeController_To_ServerManager");
class Netcut_SC_ServerManager_To_NodeController extends Netcut_SC_ServerManager_To_Component_1.Netcut_SC_ServerManager_To_Component {
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
        let componentType;
        let componentTypeID;
        switch (msgType) {
            case NetCodes_CS_NodeController_To_ServerManager_1.NetCodes_CS_NodeController_To_ServerManager.RESPONSE_CREATE_COMPONENT_OK:
                // Read the request data.
                try {
                    componentTypeID = msgNetBuffer.readInt();
                }
                catch (err) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                componentType = ComponentType_1.ComponentType.fromID(componentTypeID);
                if (!componentType) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                //void
                break;
            case NetCodes_CS_NodeController_To_ServerManager_1.NetCodes_CS_NodeController_To_ServerManager.RESPONSE_CREATE_COMPONENT_ERROR:
                // Read the request data.
                try {
                    componentTypeID = msgNetBuffer.readInt();
                }
                catch (err) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                componentType = ComponentType_1.ComponentType.fromID(componentTypeID);
                if (!componentType) {
                    this.getApp().getLog().error('ServerComponentValidator. Logic error.');
                    return false;
                }
                this.getApp().getLog().error('Component creation error occured.');
                this.getApp().getErrorHandler().error('Component creation error occured.');
                break;
            default:
                return false;
        }
        return true;
    }
    sendMsg_REQUEST_CREATE_COMPONENT(componentType) {
        let netBlock = new NetMsgsBlock_1.NetMsgsBlock(1, 4 + 4);
        try {
            netBlock.startMsgWrite();
            // START : msg data;
            netBlock.writeInt(NetCodes_SC_ServerManager_To_NodeController_1.NetCodes_SC_ServerManager_To_NodeController.REQUEST_CREATE_COMPONENT);
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
    requestCreateComponent(componentType) {
        this.sendMsg_REQUEST_CREATE_COMPONENT(componentType);
    }
}
exports.Netcut_SC_ServerManager_To_NodeController = Netcut_SC_ServerManager_To_NodeController;
//# sourceMappingURL=Netcut_SC_ServerManager_To_NodeController.js.map