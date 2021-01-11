"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWIClientsValidator_1 = require("./GWIClientsValidator");
const NetClient_1 = require("./util/NetClient");
const NetClientsHolder_1 = require("base/common/client/net/holder/NetClientsHolder");
class GWIClients extends GWIClientsValidator_1.GWIClientsValidator {
    constructor(handler, config) {
        super(handler, config);
        this.validatedNetClientsHolder = null;
        //void
    }
    isAnyPlayerConnected() {
        if (this.validatedNetClientsHolder.getClientsCount() > 0) {
            return true;
        }
        return false;
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.validatedNetClientsHolder = new NetClientsHolder_1.NetClientsHolder(this.onNewClient.bind(this), this.onClientRemoved_pre.bind(this), this.onNewConMsg.bind(this)));
        this.validatedNetClientsHolder.name = "GameWorld_" + this.getServerInstanceId();
    }
    onNetClientValidationSuccess(accountId, con) {
        if (this.getIsDisposed()) {
            return;
        }
        this.validatedNetClientsHolder.addClient(new NetClient_1.NetClient(con, accountId));
    }
    onNewClient(client) {
        if (this.getIsDisposed()) {
            return;
        }
        this.handler.getLog().debug("Adding client. Server Id: " + this.config.serverInstanceId + ". Client port: " + client.remotePort + ". Remaining clients: " + this.validatedNetClientsHolder.getClientsCount() + ".");
    }
    onClientRemoved_pre(client) {
        if (this.getIsDisposed()) {
            return;
        }
        this.handler.getLog().debug("Removing client. Server Id: " + this.config.serverInstanceId + ". Client port: " + client.remotePort + "Remaining clients: " + (this.validatedNetClientsHolder.getClientsCount() - 1) + ".");
        //void
    }
    onNewConMsg(client, msgLen, msgType, msgNetBuffer) {
        if (this.getIsDisposed()) {
            return false;
        }
        if (client.player != null) {
            return this.processPlayerMsg(client.player, client, msgLen, msgType, msgNetBuffer);
        }
        else {
            return this.processClientMsg(client, msgLen, msgType, msgNetBuffer);
        }
    }
    processPlayerMsg(pl, client, msgLen, msgType, msgNetBuffer) {
        return false;
    }
    processClientMsg(client, msgLen, msgType, msgNetBuffer) {
        return false;
    }
}
exports.GWIClients = GWIClients;
//# sourceMappingURL=GWIClients.js.map