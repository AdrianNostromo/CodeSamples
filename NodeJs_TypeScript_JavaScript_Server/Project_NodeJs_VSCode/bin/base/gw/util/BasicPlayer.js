"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class BasicPlayer {
    constructor(playerId, accountId) {
        this.client = null;
        this.isRemoved = false;
        this.playerId = playerId;
        this.accountId = accountId;
    }
    getClient() {
        return this.client;
    }
    setClient(client) {
        if (this.client != null) {
            this.client.player = null;
            this.client = null;
        }
        if (client != null) {
            this.client = client;
            this.client.player = this;
        }
    }
}
exports.BasicPlayer = BasicPlayer;
//# sourceMappingURL=BasicPlayer.js.map