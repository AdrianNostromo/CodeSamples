import { GWIBase } from "./GWIBase";
import { TcpServer } from "base/common/net/TcpServer";
import { IHandler_GWI } from "./IHandler_GWI";
import { DTGWIConfig } from "./util/DTGWIConfig";
import { IpPort } from "base/net/util/IpPort";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { GlobalAccessLogger } from "base/log/GlobalAccessLogger";
import * as net from "net";

export abstract class GWINetListener extends GWIBase {

  private netClientsListener: TcpServer = null;
  
  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super(handler, config);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    this.netClientsListener = new TcpServer(
      new IpPort(
        '127.0.0.1',
        0
      ),
      this.onNewUnvalidatedSocket.bind(this),
      this.onListenerNetError.bind(this),
      this.onListenerLogicError.bind(this),
      null
    );
    this.netClientsListener.create();
  }

  private onNewUnvalidatedSocket(target: TcpServer, socket: net.Socket): void {
    let con: GeneralTcpConnection = new GeneralTcpConnection(socket);

    GlobalAccessLogger.getLog().debug('New listener socket(GameWorld_' + this.getServerInstanceId() + '). Client port: ' + socket.remotePort);
    
    this.onNewUnvalidatedCon(con);
  }

  protected abstract onNewUnvalidatedCon(con: GeneralTcpConnection): void;

  private onListenerNetError(error: Error): void {
    this.netError("Error.GWINetListener.1.");
  }

  private onListenerLogicError(error: Error): void {
    this.logicError("Error.onListenerLogicError.1.");
  }

  protected checkIsInitFinished(): boolean {
    if(!super.checkIsInitFinished()) {
      return false;
    }

    if(this.netClientsListener == null || !this.netClientsListener.getIsListenerActive()) {
      return false;
    }

    return true;
  }

  public getPlayerServerIpPort(): IpPort {
    if(this.netClientsListener != null) {
      let ret: IpPort = this.netClientsListener.getIpPort();
      
      return ret;
    }
    
    return null;
  }

}
