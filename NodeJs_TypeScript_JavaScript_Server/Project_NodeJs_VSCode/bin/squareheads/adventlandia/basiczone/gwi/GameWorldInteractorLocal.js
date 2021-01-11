"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWILPlayerActions_1 = require("./GWILPlayerActions");
const Player_1 = require("./util/Player");
class GameWorldInteractorLocal extends GWILPlayerActions_1.GWILPlayerActions {
    constructor(handler, config) {
        super(handler, config);
        //void
    }
    createPlayerInstance(playerId, accountId) {
        let pl = new Player_1.Player(playerId, accountId);
        return pl;
    }
    onClientRemoved_pre(client) {
        let pl = client.player;
        super.onClientRemoved_pre(client);
        if (pl != null) {
            this.removePlayer(pl);
        }
    }
}
exports.GameWorldInteractorLocal = GameWorldInteractorLocal;
//# sourceMappingURL=GameWorldInteractorLocal.js.map