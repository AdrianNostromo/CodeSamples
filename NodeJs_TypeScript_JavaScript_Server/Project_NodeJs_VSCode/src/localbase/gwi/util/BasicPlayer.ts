import { NetClient } from "./NetClient";

export class BasicPlayer {

  public playerId: number;
  
  public accountId: number;

  private client: NetClient = null;

  public isRemoved: boolean = false;
  
  public constructor(playerId: number, accountId: number) {
    this.playerId = playerId;

    this.accountId = accountId;
  }

  public getClient(): NetClient {
    return this.client;
  }
  
  public setClient(client: NetClient): void {
    if(this.client != null) {
      this.client.player = null;
      
      this.client = null;
    }

    if(client != null) {
      this.client = client;

      this.client.player = this;
    }
  }
  
}
