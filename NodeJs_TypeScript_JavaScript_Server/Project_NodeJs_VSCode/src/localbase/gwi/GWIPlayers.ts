import { GWIClients } from "./GWIClients";
import { BasicPlayer } from "./util/BasicPlayer";
import { IHandler_GWI } from "./IHandler_GWI";
import { NetClient } from "./util/NetClient";
import { LogicError } from "base/exception/LogicError";
import { DTGWIConfig } from "./util/DTGWIConfig";

export abstract class GWIPlayers extends GWIClients {

  private playerIdIncrementor: number = 0;

  private playersList: BasicPlayer[] = null;
  private playersCount: number = 0;
  
  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super(handler, config);
    
    //void
  }

  protected create_framework(): void {
    super.create_framework();

    this.playersList = [];
  }
  
  protected getPlayersList(): BasicPlayer[] {
    return this.playersList;
  }

  public getPlayersCount(): number {
    return this.playersCount;
  }

  protected onClientRemoved_pre(client: NetClient): void {
    super.onClientRemoved_pre(client);

    let pl: BasicPlayer = client.player;
    if(pl != null) {
      pl.setClient(null);
    }
  }

  protected getPlayerWithAccountId(accountId: number): BasicPlayer {
    for(let i: number=0;i<this.playersList.length;i++) {
      let pl: BasicPlayer = this.playersList[i];

      if(pl != null && pl.accountId == accountId) {
        return pl;
      }
    }

    return null;
  }

  protected createNewPlayer(accountId: number): BasicPlayer {
    let pl: BasicPlayer = this.createPlayerInstance(
      ++this.playerIdIncrementor,
      accountId
    );
    this.playersList.push(pl);
    this.playersCount++;

    this.onPlayerCreated(pl);

    return pl;
  }

  protected abstract createPlayerInstance(playerId: number, accountId: number): BasicPlayer;

  protected onPlayerCreated(pl: BasicPlayer) {
    this.handler.getLog().debug("Adding player. Server Id: " + this.config.serverInstanceId + ". Remaining players: " + this.playersCount + ".");
  }

  protected onPlayerRemove_pre(pl: BasicPlayer) {
    this.handler.getLog().debug("Removing player. Server Id: " + this.config.serverInstanceId + ". Remaining players: " + (this.playersCount - 1) + ".");

    //void
  }

  protected removePlayer(pl: BasicPlayer): void {
    if(pl.isRemoved) {
      throw new LogicError("GWIPlayers.removePlayer.1.");
    }
    
    let index: number = this.playersList.indexOf(pl);
    if(index < 0) {
      throw new LogicError("GWIPlayers.removePlayer.1.");
    }

    this.onPlayerRemove_pre(pl);

    let client: NetClient = pl.getClient();
    if(client != null) {
      pl.setClient(null);

      client.markForRemoval = true;
    }

    pl.isRemoved = true;
    this.playersList.splice(index, 1);

    this.playersCount--;
  }
  
  public dispose(): void {
    if(this.playersList != null) {
      for(let i: number=this.playersList.length-1;i>=0;i--) {
        let pl: BasicPlayer = this.playersList[i];

        if(pl != null) {
          this.removePlayer(pl);
        }
      }

      this.playersList = null;
    }
    
    super.dispose();
  }

}
