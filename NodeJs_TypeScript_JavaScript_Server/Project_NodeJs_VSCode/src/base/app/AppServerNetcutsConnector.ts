import { AppClientNetcutsListener } from "./AppClientNetcutsListener";
import { ComponentType } from "base/common/ComponentType";
import { AppDirs } from "base/common/util/AppDirs";
import { BasicNetcutsFactory } from "base/net/BasicNetcutsFactory";
import { ServerConnectorNetConfig } from "base/module/util/ServerConnectorNetConfig";

import { ServerComponentValidator } from "base/common/net/ServerComponentValidator";
import { TcpConnector } from "base/common/net/TcpConnector";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ServerConnectionCallbacksHolder } from "base/common/util/ServerConnectionCallbacksHolder";
import { ClientListenerNetConfig } from "base/module/util/ClientListenerNetConfig";

export class AppServerNetcutsConnector extends AppClientNetcutsListener {

  private serverConnectorNetConfig: ServerConnectorNetConfig;
    
  private _serverComponentValidator: ServerComponentValidator = null;
  private _serverComponentConnector: TcpConnector = null;

  public constructor(componentType: ComponentType, dirs: AppDirs, netcutsFactory: BasicNetcutsFactory, clientListenerNetConfig: ClientListenerNetConfig, serverConnectorNetConfig: ServerConnectorNetConfig) {
    super(componentType, dirs, netcutsFactory, clientListenerNetConfig);
    
    this.serverConnectorNetConfig = serverConnectorNetConfig;
  }

  protected create_framework(): void {
    super.create_framework();
    
    if(this.serverConnectorNetConfig) {
      this.addComponent(this._serverComponentValidator = new ServerComponentValidator(this.getApp()));
    
      this.addComponent(this._serverComponentConnector = new TcpConnector(this.getApp()));
    }
  }
  
  public connectToServerComponent(componentType: ComponentType, ip: string, port: number, cbHolder: ServerConnectionCallbacksHolder): boolean {
    let retCon: GeneralTcpConnection = this._serverComponentConnector.newCon(
      ip, port, 
      componentType, 
      this.onUnvalidatedServerCon.bind(this), this.onServerConFailed.bind(this)
    )
    
    if(!retCon) {
      this.getApp().getLog().error('Server component connection validation failed.' + '. Treated a warning, not critical at this location.');
      
      this.getApp().getErrorHandler().error('Server component connection validation failed.');
      
      return false;
    }
    
    retCon.setUserData<ServerConnectionCallbacksHolder>("connectionCbHolder", cbHolder);
    
    return true;
  }

  private onUnvalidatedServerCon(conTrackID: number, con: GeneralTcpConnection, remoteComponentType: ComponentType): void {
    // Connection established to a server, validate it.
    let cbHolder: ServerConnectionCallbacksHolder = con.getUserData<ServerConnectionCallbacksHolder>("connectionCbHolder");
    if(cbHolder && cbHolder.cbUnvalidatedConnection) {
      cbHolder.cbUnvalidatedConnection(con, remoteComponentType);
    }
    
    this._serverComponentValidator.handleNewUnconfirmedComponent(
      con, remoteComponentType,
      this.onServerConValidated.bind(this), this.onServerConValidationFailed.bind(this)
    )
  }

  private onServerConFailed(con: GeneralTcpConnection, conTrackID: number, remoteComponentType: ComponentType): void {
    // Post a warning and try again.
    
    let cbHolder: ServerConnectionCallbacksHolder = con.getUserData<ServerConnectionCallbacksHolder>("connectionCbHolder");
    if(cbHolder && cbHolder.cbConnectionFail) {
      cbHolder.cbConnectionFail(con, remoteComponentType);
    }
    con.setUserData<ServerConnectionCallbacksHolder>("connectionCbHolder", null);
  }

  private onServerConValidated(con: GeneralTcpConnection, remoteComponentType: ComponentType): void {
    let cbHolder: ServerConnectionCallbacksHolder = con.getUserData<ServerConnectionCallbacksHolder>("connectionCbHolder");
    if(cbHolder && cbHolder.cbValidated) {
      cbHolder.cbValidated(con, remoteComponentType);
    }
    con.setUserData<ServerConnectionCallbacksHolder>("connectionCbHolder", null);
    
    this.pushNetcut(remoteComponentType, con, false);
  }

  private onServerConValidationFailed(con: GeneralTcpConnection, remoteComponentType: ComponentType): void {
    this.getApp().getLog().error('Server component connection validation failed.' + '. Treated a warning, not critical at this location.');
    
    this.getApp().getErrorHandler().error('Server component connection validation failed.');
    
    return ;
  }

}
