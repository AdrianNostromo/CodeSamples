import { GWIClientsValidator } from "./GWIClientsValidator";
import { NetClient } from "./util/NetClient";
import { NetClientsHolder } from "base/common/client/net/holder/NetClientsHolder";
import { IHandler_GWI } from "./IHandler_GWI";
import { DTGWIConfig } from "./util/DTGWIConfig";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { BasicPlayer } from "./util/BasicPlayer";

export abstract class GWIClients extends GWIClientsValidator {

  private validatedNetClientsHolder: NetClientsHolder<NetClient> = null;
  
  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super(handler, config);
    
    //void
  }

  public isAnyPlayerConnected(): boolean {
    if(this.validatedNetClientsHolder.getClientsCount() > 0) {
      return true;
    }

    return false;
  }

  protected create_framework(): void {
    super.create_framework();
    
    this.addComponent(this.validatedNetClientsHolder = new NetClientsHolder<NetClient>(
      this.onNewClient.bind(this),
      this.onClientRemoved_pre.bind(this),
      this.onNewConMsg.bind(this)
    ));
    this.validatedNetClientsHolder.name = "GameWorld_" + this.getServerInstanceId();
  }

  protected onNetClientValidationSuccess(accountId: number, con: GeneralTcpConnection): void {
    if(this.getIsDisposed()) {
      return ;
    }
    
    this.validatedNetClientsHolder.addClient(new NetClient(
      con, 
      accountId
    ));
  }

  protected onNewClient(client: NetClient): void {
    if(this.getIsDisposed()) {
      return ;
    }
    
    this.handler.getLog().debug("Adding client. Server Id: " + this.config.serverInstanceId + ". Client port: " + client.remotePort + ". Remaining clients: " + this.validatedNetClientsHolder.getClientsCount() + ".");
  }

  protected onClientRemoved_pre(client: NetClient): void {
    if(this.getIsDisposed()) {
      return ;
    }
    
    this.handler.getLog().debug("Removing client. Server Id: " + this.config.serverInstanceId + ". Client port: " + client.remotePort + "Remaining clients: " + (this.validatedNetClientsHolder.getClientsCount() - 1) + ".");

    //void
  }

  private onNewConMsg(client: NetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    if(this.getIsDisposed()) {
      return false;
    }
    
    if(client.player != null) {
      return this.processPlayerMsg(client.player, client, msgLen, msgType, msgNetBuffer);
    }else {
      return this.processClientMsg(client, msgLen, msgType, msgNetBuffer);
    }
  }

  protected processPlayerMsg(pl: BasicPlayer, client: NetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    return false;
  }

  protected processClientMsg(client: NetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    return false;
  }

}
