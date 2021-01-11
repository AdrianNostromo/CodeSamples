import { AppNetcuts } from "./AppNetcuts";
import { ComponentType } from "base/common/ComponentType";
import { AppDirs } from "base/common/util/AppDirs";
import { BasicNetcutsFactory } from "base/net/BasicNetcutsFactory";
import { IpPort } from "base/net/util/IpPort";

import { ClientComponentValidator } from "base/common/net/ClientComponentValidator";
import { TcpServer } from "base/common/net/TcpServer";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentStatus } from "base/common/util/ComponentStatus";
import * as net from "net";
import { ClientListenerNetConfig } from "base/module/util/ClientListenerNetConfig";

export class AppClientNetcutsListener extends AppNetcuts {

  private _clientListenerNetConfig: ClientListenerNetConfig;
  
  private _clientComponentValidator: ClientComponentValidator = null;
  private _clientComponentListener: TcpServer = null;
  
  private _isListenerStarted: boolean = false;

  private _bf_onClientComponentValidated: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void = null;
  private _bf_onClientComponentValidationFailed: (con: GeneralTcpConnection) => void = null;

  public constructor(componentType: ComponentType, dirs: AppDirs, netcutsFactory: BasicNetcutsFactory, clientListenerNetConfig: ClientListenerNetConfig) {
    super(componentType, dirs, netcutsFactory);
    
    this._clientListenerNetConfig = clientListenerNetConfig;
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    if(this.isListenerEnabled()) {
      this.addComponent(this._clientComponentValidator = new ClientComponentValidator(this.getApp()));
      
      this._bf_onClientComponentValidated = this.onClientComponentValidated.bind(this);
      this._bf_onClientComponentValidationFailed = this.onClientComponentValidationFailed.bind(this);
      
      this._clientComponentListener = new TcpServer(
        this._clientListenerNetConfig.ipPort,

        this.onUnconfirmedComponentSocket.bind(this),
        this.onComponentsListenerNetError.bind(this),
        this.onComponentsListenerLogicError.bind(this),
        this.onComponentsListenerActive.bind(this)
      );
      this._clientComponentListener.create();
    }
  }
  
  private onComponentsListenerActive(target: TcpServer): void {
    let ipPort: IpPort = target.getIpPort();
    
    this.getApp().getLog().info('Client components listener active on ip: ' + ipPort.ip + ' port: ' + ipPort.port);
  }

  private onComponentsListenerNetError(target: TcpServer, err: Error): void {
    this.getApp().getLog().error('TcpServer. Error. Maybe the port is already in use.');

    this.getApp().getErrorHandler().error('TcpServer. Error. Maybe the port is already in use.');
  }

  private onComponentsListenerLogicError(target: TcpServer, err: Error): void {
    this.getApp().getLog().error('TcpServer. Error. Maybe the port is already in use.');

    this.getApp().getErrorHandler().error('TcpServer. Error. Maybe the port is already in use.');
  }

  private onUnconfirmedComponentSocket(target: TcpServer, socket: net.Socket): void {
    if(!this._isListenerStarted) {
      if(!socket.destroyed) {
        socket.destroy();
      }

      return ;
    }

    let con: GeneralTcpConnection = new GeneralTcpConnection(socket);
    
    this._clientComponentValidator.handleNewUnconfirmedComponent(
      con, 
      this._bf_onClientComponentValidated, 
      this._bf_onClientComponentValidationFailed
    )
  }

  private onClientComponentValidated(con: GeneralTcpConnection, remoteComponentType: ComponentType): void {
    // Add the component to the child components handler. Wrap the con in custom Netcut classes that process the messages.
    
    this.getApp().getLog().info('Client component validated. Type: ' + remoteComponentType.name);
    
    this.pushNetcut(remoteComponentType, con, true);
  }
  
  private onClientComponentValidationFailed(con: GeneralTcpConnection): void {
    this.getApp().getLog().warning('Client component validation failed.');
  }
  
  public componentStatusChanged(componentStatus: ComponentStatus): void {
    super.componentStatusChanged(componentStatus);
    
    if(componentStatus == ComponentStatus.STARTING) {
      if(!this._isListenerStarted && this._clientComponentListener) {
        this._isListenerStarted = true;
      }
    }
  }
  
  public isComponentTransientStateFinished(componentStatus: ComponentStatus): boolean {
    let ret: boolean = super.isComponentTransientStateFinished(componentStatus);
    
    if(componentStatus == ComponentStatus.STARTING) {
      if(this._clientComponentListener && !this._clientComponentListener.getIsListenerActive()) {
        ret = false;
      }
    }
    
    return ret;
  }
  
  public isListenerEnabled(): boolean {
    if(this._clientListenerNetConfig) {
      return true;
    }
    
    return false;
  }
  
  public getListenerIpPort(): IpPort {
    if(!this.isListenerEnabled() || !this._clientComponentListener) {
      return null;
    }
    
    return this._clientComponentListener.getIpPort();
  }
  
}
