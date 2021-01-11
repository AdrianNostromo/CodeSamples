import { GWIPlayers } from "./GWIPlayers";
import { IHandler_GWI } from "./IHandler_GWI";
import { DTGWIConfig } from "./util/DTGWIConfig";
import { NetClient } from "./util/NetClient";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { BasicPlayer } from "./util/BasicPlayer";

class NetCodes {
  
  public static readonly SC_C_SEND_CONNECTION_PURPOSE: number = 300010;
  public static readonly CS_R_SEND_CONNECTION_PURPOSE_PLAYER: number = 300011;
  
  public static readonly SC_C_SET_PLAY_STATUS_PLAYER_ACTIVE: number = 300020;
  // playerId: number;

  public static readonly SC_C_SET_PLAY_STATUS_PLAYER_SERVER_FULL: number = 300030;
  public static readonly CS_R_SET_PLAY_STATUS_PLAYER_SERVER_FULL_OK: number = 300031;

  public static readonly SC_C_CONTROLL_TRANSFERRED_RECONNECT_OCCURED: number = 300040;
  public static readonly CS_R_CONTROLL_TRANSFERRED_RECONNECT_OCCURED_OK: number = 300041;
  
}

class NetCodes_Player {
  
  public static readonly CS_R_SET_PLAY_STATUS_PLAYER_ACTIVE_OK: number = 300021;

}

export abstract class GWIMatchJoiner extends GWIPlayers {

  public static readonly DISCONNECT_RESPONSE_WAITING_TIMEOUT_S: number = 10;

  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super(handler, config);
    
    //void
  }
  
  protected onNewClient(client: NetClient): void {
    super.onNewClient(client);

    this.sendMsg_SC_C_SEND_CONNECTION_PURPOSE(client);
  }
  
  private sendMsg_SC_C_SEND_CONNECTION_PURPOSE(client: NetClient): void {
    client.setCurrentCommandNetCode(NetCodes.SC_C_SEND_CONNECTION_PURPOSE);
    client.getCon().sendBlock(new NetMsgsBlock(1, 
      4)
      .startMsgWrite()
      .writeInt(NetCodes.SC_C_SEND_CONNECTION_PURPOSE)
      .endMsgWrite()
    );
  }

  protected processClientMsg(client: NetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    if(super.processClientMsg(client, msgLen, msgType, msgNetBuffer)) {
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

  private processMsg_CS_R_SEND_CONNECTION_PURPOSE_PLAYER(client: NetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): void {
    if(client.getCurrentCommandNetCode() != NetCodes.SC_C_SEND_CONNECTION_PURPOSE) {
      throw new Error('Security warning.');
    }
    client.clearCurrentCommandNetCode();

    // Check if the client account is already a player and replace the netClient if found. Also disconnect the existing netClient with a msg.
    let pl: BasicPlayer = this.getPlayerWithAccountId(client.accountId);
    if(pl != null) {
      if(pl.getClient() != null) {
        let oldClient: NetClient = pl.getClient();
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
    }else {
      if(this.getPlayersCount() > this.config.maxPlayersCount) {
        this.sendMsg_SC_C_SET_PLAY_STATUS_PLAYER_SERVER_FULL(client);

        // Start a client removal timer.
        client.forceDisconnectTimeoutS = GWIMatchJoiner.DISCONNECT_RESPONSE_WAITING_TIMEOUT_S;
      }else {
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

  private sendMsg_SC_C_SET_PLAY_STATUS_PLAYER_ACTIVE(client: NetClient, playerId: number): void {
    client.setCurrentCommandNetCode(NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_ACTIVE);
    client.getCon().sendBlock(new NetMsgsBlock(1, 
      4
      +4)
      .startMsgWrite()
      .writeInt(NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_ACTIVE)
      .writeInt(playerId)
      .endMsgWrite()
    );
  }

  private sendMsg_SC_C_SET_PLAY_STATUS_PLAYER_SERVER_FULL(client: NetClient): void {
    client.setCurrentCommandNetCode(NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_SERVER_FULL);
    client.getCon().sendBlock(new NetMsgsBlock(1, 
      4)
      .startMsgWrite()
      .writeInt(NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_SERVER_FULL)
      .endMsgWrite()
    );
  }

  private processMsg_CS_R_SET_PLAY_STATUS_PLAYER_SERVER_FULL_OK(client: NetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): void {
    if(client.getCurrentCommandNetCode() != NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_SERVER_FULL) {
      throw new Error('Security warning.');
    }
    client.clearCurrentCommandNetCode();

    client.markForRemoval = true;
  }

  private sendMsg_SC_C_CONTROLL_TRANSFERRED_RECONNECT_OCCURED(client: NetClient): void {
    client.setCurrentCommandNetCode(NetCodes.SC_C_CONTROLL_TRANSFERRED_RECONNECT_OCCURED);
    client.getCon().sendBlock(new NetMsgsBlock(1, 
      4)
      .startMsgWrite()
      .writeInt(NetCodes.SC_C_CONTROLL_TRANSFERRED_RECONNECT_OCCURED)
      .endMsgWrite()
    );
  }

  private processMsg_CS_R_CONTROLL_TRANSFERRED_RECONNECT_OCCURED_OK(client: NetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): void {
    if(client.getCurrentCommandNetCode() != NetCodes.SC_C_CONTROLL_TRANSFERRED_RECONNECT_OCCURED) {
      throw new Error('Security warning.');
    }
    client.clearCurrentCommandNetCode();

    client.markForRemoval = true;
  }

  protected processPlayerMsg(pl: BasicPlayer, client: NetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    if(super.processPlayerMsg(pl, client, msgLen, msgType, msgNetBuffer)) {
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

  private processMsg_Player_CS_R_SET_PLAY_STATUS_PLAYER_ACTIVE_OK(pl: BasicPlayer, client: NetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): void {
    if(client.getCurrentCommandNetCode() != NetCodes.SC_C_SET_PLAY_STATUS_PLAYER_ACTIVE) {
      throw new Error('Security warning.');
    }
    client.clearCurrentCommandNetCode();

    //void
  }

}
