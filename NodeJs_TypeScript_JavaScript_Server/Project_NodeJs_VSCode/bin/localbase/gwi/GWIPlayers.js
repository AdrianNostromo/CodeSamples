"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWIClients_1 = require("./GWIClients");
const LogicError_1 = require("base/exception/LogicError");
class GWIPlayers extends GWIClients_1.GWIClients {
    constructor(handler, config) {
        super(handler, config);
        this.playerIdIncrementor = 0;
        this.playersList = null;
        this.playersCount = 0;
        //void
    }
    create_framework() {
        super.create_framework();
        this.playersList = [];
    }
    getPlayersList() {
        return this.playersList;
    }
    getPlayersCount() {
        return this.playersCount;
    }
    onClientRemoved_pre(client) {
        super.onClientRemoved_pre(client);
        let pl = client.player;
        if (pl != null) {
            pl.setClient(null);
        }
    }
    getPlayerWithAccountId(accountId) {
        for (let i = 0; i < this.playersList.length; i++) {
            let pl = this.playersList[i];
            if (pl != null && pl.accountId == accountId) {
                return pl;
            }
        }
        return null;
    }
    createNewPlayer(accountId) {
        let pl = this.createPlayerInstance(++this.playerIdIncrementor, accountId);
        this.playersList.push(pl);
        this.playersCount++;
        this.onPlayerCreated(pl);
        return pl;
    }
    onPlayerCreated(pl) {
        this.handler.getLog().debug("Adding player. Server Id: " + this.config.serverInstanceId + ". Remaining players: " + this.playersCount + ".");
    }
    onPlayerRemove_pre(pl) {
        this.handler.getLog().debug("Removing player. Server Id: " + this.config.serverInstanceId + ". Remaining players: " + (this.playersCount - 1) + ".");
        //void
    }
    removePlayer(pl) {
        if (pl.isRemoved) {
            throw new LogicError_1.LogicError("GWIPlayers.removePlayer.1.");
        }
        let index = this.playersList.indexOf(pl);
        if (index < 0) {
            throw new LogicError_1.LogicError("GWIPlayers.removePlayer.1.");
        }
        this.onPlayerRemove_pre(pl);
        let client = pl.getClient();
        if (client != null) {
            pl.setClient(null);
            client.markForRemoval = true;
        }
        pl.isRemoved = true;
        this.playersList.splice(index, 1);
        this.playersCount--;
    }
    dispose() {
        if (this.playersList != null) {
            for (let i = this.playersList.length - 1; i >= 0; i--) {
                let pl = this.playersList[i];
                if (pl != null) {
                    this.removePlayer(pl);
                }
            }
            this.playersList = null;
        }
        super.dispose();
    }
}
exports.GWIPlayers = GWIPlayers;
//# sourceMappingURL=GWIPlayers.js.map