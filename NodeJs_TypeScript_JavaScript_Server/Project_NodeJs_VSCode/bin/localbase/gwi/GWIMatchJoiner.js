"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWIPlayers_1 = require("./GWIPlayers");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
class NetCodes {
}
NetCodes.SC_C_SEND_CONNECTION_PURPOSE = 300010;
NetCodes.CS_R_SEND_CONNECTION_PURPOSE_PLAYER = 300011;
NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_ACTIVE = 300020;
// playerId: number;
NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_SERVER_FULL = 300030;
NetCodes.CS_R_SET_PLAY_STATUS_PLAYER_SERVER_FULL_OK = 300031;
NetCodes.SC_C_CONTROLL_TRANSFERRED_RECONNECT_OCCURED = 300040;
NetCodes.CS_R_CONTROLL_TRANSFERRED_RECONNECT_OCCURED_OK = 300041;
class NetCodes_Player {
}
NetCodes_Player.CS_R_SET_PLAY_STATUS_PLAYER_ACTIVE_OK = 300021;
class GWIMatchJoiner extends GWIPlayers_1.GWIPlayers {
    constructor(handler, config) {
        super(handler, config);
        //void
    }
    onNewClient(client) {
        super.onNewClient(client);
        this.sendMsg_SC_C_SEND_CONNECTION_PURPOSE(client);
    }
    sendMsg_SC_C_SEND_CONNECTION_PURPOSE(client) {
        client.setCurrentCommandNetCode(NetCodes.SC_C_SEND_CONNECTION_PURPOSE);
        client.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4)
            .startMsgWrite()
            .writeInt(NetCodes.SC_C_SEND_CONNECTION_PURPOSE)
            .endMsgWrite());
    }
    processClientMsg(client, msgLen, msgType, msgNetBuffer) {
        if (super.processClientMsg(client, msgLen, msgType, msgNetBuffer)) {
            return true;
        }
        switch (msgType) {
            case NetCodes.CS_R_SEND_CONNECTION_PURPOSE_PLAYER:
                this.processMsg_CS_R_SEND_CONNECTION_PURPOSE_PLAYER(client, msgLen, msgType, msgNetBuffer);
                break;
            case NetCodes.CS_R_SET_PLAY_STATUS_PLAYER_SERVER_FULL_OK:
                this.processMsg_CS_R_SET_PLAY_STATUS_PLAYER_SERVER_FULL_OK(client, msgLen, msgType, msgNetBuffer);
                break;
            case NetCodes.CS_R_CONTROLL_TRANSFERRED_RECONNECT_OCCURED_OK:
                this.processMsg_CS_R_CONTROLL_TRANSFERRED_RECONNECT_OCCURED_OK(client, msgLen, msgType, msgNetBuffer);
                break;
            default:
                return false;
        }
        return true;
    }
    processMsg_CS_R_SEND_CONNECTION_PURPOSE_PLAYER(client, msgLen, msgType, msgNetBuffer) {
        if (client.getCurrentCommandNetCode() != NetCodes.SC_C_SEND_CONNECTION_PURPOSE) {
            throw new Error('Security warning.');
        }
        client.clearCurrentCommandNetCode();
        // Check if the client account is already a player and replace the netClient if found. Also disconnect the existing netClient with a msg.
        let pl = this.getPlayerWithAccountId(client.accountId);
        if (pl != null) {
            if (pl.getClient() != null) {
                let oldClient = pl.getClient();
                pl.setClient(null);
                // Deactivate auto ping for the old client.
                oldClient.isPingAllowed = false;
                // Send a msg to the old client.
                this.sendMsg_SC_C_CONTROLL_TRANSFERRED_RECONNECT_OCCURED(oldClient);
                // Start a client removal timer.
                oldClient.forceDisconnectTimeoutS = GWIMatchJoiner.DISCONNECT_RESPONSE_WAITING_TIMEOUT_S;
            }
            // Save the client to the player.
            pl.setClient(client);
            // Activate auto ping.
            pl.getClient().isPingAllowed = true;
            this.sendMsg_SC_C_SET_PLAY_STATUS_PLAYER_ACTIVE(client, pl.playerId);
        }
        else {
            if (this.getPlayersCount() > this.config.maxPlayersCount) {
                this.sendMsg_SC_C_SET_PLAY_STATUS_PLAYER_SERVER_FULL(client);
                // Start a client removal timer.
                client.forceDisconnectTimeoutS = GWIMatchJoiner.DISCONNECT_RESPONSE_WAITING_TIMEOUT_S;
            }
            else {
                // Create the player.
                pl = this.createNewPlayer(client.accountId);
                // Save the client to the player.
                pl.setClient(client);
                // Activate auto ping.
                pl.getClient().isPingAllowed = true;
                this.sendMsg_SC_C_SET_PLAY_STATUS_PLAYER_ACTIVE(client, pl.playerId);
            }
        }
    }
    sendMsg_SC_C_SET_PLAY_STATUS_PLAYER_ACTIVE(client, playerId) {
        client.setCurrentCommandNetCode(NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_ACTIVE);
        client.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4
            + 4)
            .startMsgWrite()
            .writeInt(NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_ACTIVE)
            .writeInt(playerId)
            .endMsgWrite());
    }
    sendMsg_SC_C_SET_PLAY_STATUS_PLAYER_SERVER_FULL(client) {
        client.setCurrentCommandNetCode(NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_SERVER_FULL);
        client.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4)
            .startMsgWrite()
            .writeInt(NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_SERVER_FULL)
            .endMsgWrite());
    }
    processMsg_CS_R_SET_PLAY_STATUS_PLAYER_SERVER_FULL_OK(client, msgLen, msgType, msgNetBuffer) {
        if (client.getCurrentCommandNetCode() != NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_SERVER_FULL) {
            throw new Error('Security warning.');
        }
        client.clearCurrentCommandNetCode();
        client.markForRemoval = true;
    }
    sendMsg_SC_C_CONTROLL_TRANSFERRED_RECONNECT_OCCURED(client) {
        client.setCurrentCommandNetCode(NetCodes.SC_C_CONTROLL_TRANSFERRED_RECONNECT_OCCURED);
        client.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4)
            .startMsgWrite()
            .writeInt(NetCodes.SC_C_CONTROLL_TRANSFERRED_RECONNECT_OCCURED)
            .endMsgWrite());
    }
    processMsg_CS_R_CONTROLL_TRANSFERRED_RECONNECT_OCCURED_OK(client, msgLen, msgType, msgNetBuffer) {
        if (client.getCurrentCommandNetCode() != NetCodes.SC_C_CONTROLL_TRANSFERRED_RECONNECT_OCCURED) {
            throw new Error('Security warning.');
        }
        client.clearCurrentCommandNetCode();
        client.markForRemoval = true;
    }
    processPlayerMsg(pl, client, msgLen, msgType, msgNetBuffer) {
        if (super.processPlayerMsg(pl, client, msgLen, msgType, msgNetBuffer)) {
            return true;
        }
        switch (msgType) {
            case NetCodes_Player.CS_R_SET_PLAY_STATUS_PLAYER_ACTIVE_OK:
                this.processMsg_Player_CS_R_SET_PLAY_STATUS_PLAYER_ACTIVE_OK(pl, client, msgLen, msgType, msgNetBuffer);
                break;
            default:
                return false;
        }
        return true;
    }
    processMsg_Player_CS_R_SET_PLAY_STATUS_PLAYER_ACTIVE_OK(pl, client, msgLen, msgType, msgNetBuffer) {
        if (client.getCurrentCommandNetCode() != NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_ACTIVE) {
            throw new Error('Security warning.');
        }
        client.clearCurrentCommandNetCode();
        //void
    }
}
GWIMatchJoiner.DISCONNECT_RESPONSE_WAITING_TIMEOUT_S = 10;
exports.GWIMatchJoiner = GWIMatchJoiner;
//# sourceMappingURL=GWIMatchJoiner.js.map