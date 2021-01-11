"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const NetClientsHolderBase_1 = require("./NetClientsHolderBase");
const DataFormatError_1 = require("base/net/exception/DataFormatError");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
const LogicError_1 = require("base/exception/LogicError");
const SecurityError_1 = require("base/exception/SecurityError");
const GlobalAccessLogger_1 = require("base/log/GlobalAccessLogger");
class NetCodes {
}
NetCodes.SC_C_PING = 10010;
NetCodes.CS_R_PING_OK = 10011;
class NetClientsHolderList extends NetClientsHolderBase_1.NetClientsHolderBase {
    constructor(cbNewClient, cbClientRemoved_pre, cbProcessMsg) {
        super();
        this._ILoopUserBase_General = true;
        // This callback is sent each tick.
        // The callback must check if ping is in progress.
        this.clientsList = [];
        this.iddleDataReceiveThresholdS = 5;
        this.interPingDelayS = 1;
        // Used for log purposes.
        this.name = "";
        this.cbNewClient = cbNewClient;
        this.cbClientRemoved_pre = cbClientRemoved_pre;
        this.cbProcessMsg = cbProcessMsg;
    }
    getClientsList() {
        return this.clientsList;
    }
    getClientsCount() {
        return this.clientsList.length;
    }
    addClient(client) {
        this.clientsList.push(client);
        client.getCon().setCbClose(this.onConClose.bind(this));
        client.getCon().setCbError(this.onConError.bind(this));
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug('+NetClientsHolder(' + this.name + '). Client port: ' + client.remotePort + ' . Current list size: ' + this.clientsList.length);
        this.onNewClient(client);
    }
    extractClientCon(client) {
        if (client.getCon() == null) {
            return null;
        }
        let con = client.getCon();
        con.setCbClose(null);
        con.setCbError(null);
        client.setCon(null);
        return con;
    }
    onNewClient(client) {
        if (this.getIsDisposed()) {
            return;
        }
        if (this.cbNewClient != null) {
            this.cbNewClient(client);
        }
    }
    onConClose(con) {
        if (this.getIsDisposed()) {
            return;
        }
        let client = con.getUserData("client");
        this.removeClient(client);
    }
    onConError(con) {
        if (this.getIsDisposed()) {
            return;
        }
        let client = con.getUserData("client");
        this.removeClient(client);
    }
    removeClient(client) {
        if (!client) {
            return;
        }
        let index = this.clientsList.indexOf(client);
        if (index < 0) {
            GlobalAccessLogger_1.GlobalAccessLogger.getLog().error('ServerComponentValidator. Error.');
            throw new LogicError_1.LogicError('Error.');
        }
        this.onClientRemoved_pre(client);
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug('-NetClientsHolder(' + this.name + '). Client port: ' + client.remotePort + ' . Current list size: ' + this.clientsList.length);
        if (client.getCon() != null) {
            client.getCon().setCbClose(null);
            client.getCon().setCbError(null);
            client.getCon().forceClose();
            client.setCon(null);
        }
        this.clientsList.splice(index, 1);
    }
    onClientRemoved_pre(client) {
        if (this.getIsDisposed()) {
            return;
        }
        if (this.cbClientRemoved_pre != null) {
            this.cbClientRemoved_pre(client);
        }
    }
    appLoop_General(deltaS, deltaMS) {
        this.appLoop_net(deltaS, deltaMS);
    }
    appLoop_net(deltaS, deltaMS) {
        for (let i = this.clientsList.length - 1; i >= 0; i--) {
            let client = this.clientsList[i];
            if (client != null) {
                if (client.forceDisconnectTimeoutS >= 0) {
                    client.forceDisconnectTimeoutS -= deltaS;
                    if (client.forceDisconnectTimeoutS <= 0) {
                        this.removeClient(client);
                        continue;
                    }
                }
                if (client.currentCommandTimeoutS >= 0) {
                    client.currentCommandTimeoutS -= deltaS;
                    if (client.currentCommandTimeoutS <= 0) {
                        this.removeClient(client);
                        continue;
                    }
                }
                if (client.getCon().isData()) {
                    client.receiveIddleTimeS = 0;
                    this.processPlayerConData(client);
                }
                else {
                    client.receiveIddleTimeS += deltaS;
                    if (client.receiveIddleTimeS > this.iddleDataReceiveThresholdS) {
                        this.removeClient(client);
                        continue;
                    }
                    if (client.isPingAllowed) {
                        if (client.isPingInProgress) {
                            client.activePingTimeoutS -= deltaS;
                            if (client.activePingTimeoutS <= 0) {
                                this.removeClient(client);
                                continue;
                            }
                        }
                        else {
                            if (client.receiveIddleTimeS > this.interPingDelayS) {
                                this.SC_C_PING(client);
                            }
                        }
                    }
                }
            }
        }
    }
    SC_C_PING(client) {
        if (client.isPingInProgress) {
            throw new LogicError_1.LogicError('Error.');
        }
        client.isPingInProgress = true;
        client.activePingTimeoutS = NetClientsHolderList.PING_RESPONSE_TIME_LIMIT_S;
        client.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4)
            .startMsgWrite()
            .writeInt(NetCodes.SC_C_PING)
            .endMsgWrite());
    }
    processPlayerConData(client) {
        while (true) {
            let msgNetBuffer;
            let msgLen;
            let msgType;
            // Make this check 2 times for better removal handling.
            if (client.markForRemoval) {
                this.removeClient(client);
                return;
            }
            try {
                msgNetBuffer = client.getCon().popNextMsg();
            }
            catch (x) {
                if (x instanceof DataFormatError_1.DataFormatError) {
                    this.removeClient(client);
                    return;
                }
                else {
                    this.removeClient(client);
                    return;
                }
            }
            if (msgNetBuffer == null) {
                return;
            }
            try {
                msgLen = msgNetBuffer.readInt();
                msgType = msgNetBuffer.readInt();
            }
            catch (x) {
                this.removeClient(client);
                return;
            }
            let isMsgHandled;
            try {
                isMsgHandled = this.onProcessMsg(client, msgLen, msgType, msgNetBuffer);
            }
            catch (x) {
                GlobalAccessLogger_1.GlobalAccessLogger.getLog().security('Client message error A.' + msgType);
                this.removeClient(client);
                return;
            }
            if (!isMsgHandled) {
                GlobalAccessLogger_1.GlobalAccessLogger.getLog().security('Client message pare error B.' + msgType);
                this.removeClient(client);
                return;
            }
            // Make this check 2 times for better removal handling.
            if (client.markForRemoval) {
                this.removeClient(client);
                return;
            }
        }
    }
    onProcessMsg(client, msgLen, msgType, msgNetBuffer) {
        // Check if the msg is a locally handled one.
        if (this.processMsg_local(client, msgLen, msgType, msgNetBuffer)) {
            return true;
        }
        if (this.cbProcessMsg != null) {
            return this.cbProcessMsg(client, msgLen, msgType, msgNetBuffer);
        }
        return false;
    }
    processMsg_local(client, msgLen, msgType, msgNetBuffer) {
        switch (msgType) {
            case NetCodes.CS_R_PING_OK:
                if (!client.isPingInProgress) {
                    throw new SecurityError_1.SecurityError("NetClientsHolderList.processMsg_local.1.");
                }
                client.isPingInProgress = false;
                break;
            default:
                return false;
        }
        return true;
    }
}
NetClientsHolderList.PING_RESPONSE_TIME_LIMIT_S = 3;
exports.NetClientsHolderList = NetClientsHolderList;
//# sourceMappingURL=NetClientsHolderList.js.map