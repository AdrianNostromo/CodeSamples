import { NetcutBasic } from "./NetcutBasic";
import { IApp } from "base/app/IApp";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "base/common/ComponentType";

import { StreamBuffer } from "base/buffer/StreamBuffer";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";

import { ComponentStatus } from "base/common/util/ComponentStatus";
import { NetCodes_Component_To_Component } from "base/common/net/netcodes/NetCodes_Component_To_Component";
import { CallbackRequest } from "./util/CallbackRequest";

export class Netcut_Component_To_Component extends NetcutBasic {

  private _remoteComponentStatus: ComponentStatus = null;
  
  private _sd_componentStatus: ComponentStatus = null;
  
  private _cbRemoteComponentStatusChanged: (target: Netcut_Component_To_Component) => void = null;
  
  private _activeCallbackedRequestsList: CallbackRequest[] = [];
  private _activeCallbackOriginIdCounter: number = 0;
  
  public constructor(app: IApp, con: GeneralTcpConnection, componentType: ComponentType, isServerLocally: boolean) {
    super(app, con, componentType, isServerLocally);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    //void
  }
  
  public logicLoop(deltaS: number, deltaMS: number): void {
    super.logicLoop(deltaS, deltaMS);
    
    if(this._sd_componentStatus != this.getApp().getComponentStatus()) {
      this._sd_componentStatus = this.getApp().getComponentStatus();
      
      let s: string = 'Sending component status. Remote component type: ' + this._componentType.name + '. Local component status: ' + this._sd_componentStatus.name + ' Remote component status: ';
      if(this._remoteComponentStatus) {
        s += this._remoteComponentStatus.name;
      }else {
        s += 'null';
      }
      //this.getApp().getLog().info(s);
      
      this.syncComponentStatus(this._sd_componentStatus);
    }
  }
  
  // Extended.
  protected syncComponentStatus(componentStatus: ComponentStatus): void {
    if(!this.sendMsg_REQUEST_COMPONENT_STATUS_CHANGED(this._sd_componentStatus)) {
      this.handleConError();
      
      return ;
    }
  }
  
  protected processMsg(msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    if(super.processMsg(msgLen, msgType, msgNetBuffer)) {
      return true;
    }
    
    switch (msgType) {
      case NetCodes_Component_To_Component.REQUEST_COMPONENT_STATUS_CHANGED:
        let remoteComponentStatusID: number;
        
        try {
          remoteComponentStatusID = msgNetBuffer.readInt();
        }catch(err) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        let remoteComponentStatusType: ComponentStatus = ComponentStatus.fromID(remoteComponentStatusID);
        if(!remoteComponentStatusType) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        this._remoteComponentStatus = remoteComponentStatusType;
        
        let s: string = 'Remote component status received. Remote component type: ' + this._componentType.name + '. Remote component status: ' + this._remoteComponentStatus.name + ' Local actual component status: ' + this.getApp().getComponentStatus().name + ' Local component synked status: ';
        if(this._sd_componentStatus) {
          s += this._sd_componentStatus.name;
        }else {
          s += 'null';
        }
        //this.getApp().getLog().info(s);
        
        if(this._cbRemoteComponentStatusChanged) {
          this._cbRemoteComponentStatusChanged(this);
        }
        
        if(!this.sendMsg_RESPONSE_COMPONENT_STATUS_CHANGED_OK()) {
          this.getApp().getLog().warning('ServerComponentValidator. Logic error.');
          
          return false;
        }
        
        break;
      case NetCodes_Component_To_Component.RESPONSE_COMPONENT_STATUS_CHANGED_OK:
        //void
        
        break;
      default:
        return false;
    }
    
    return true;
  }
  
  private sendMsg_REQUEST_COMPONENT_STATUS_CHANGED(componentStatus: ComponentStatus): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4 + 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_Component_To_Component.REQUEST_COMPONENT_STATUS_CHANGED);
      netBlock.writeInt(componentStatus.id);
      // END : msg data;
      netBlock.endMsgWrite();
      
      this.sendNetBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }
  
  private sendMsg_RESPONSE_COMPONENT_STATUS_CHANGED_OK(): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_Component_To_Component.RESPONSE_COMPONENT_STATUS_CHANGED_OK);
      // END : msg data;
      netBlock.endMsgWrite();
      
      this.sendNetBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }
  
  public getRemoteComponentStatus(): ComponentStatus {
    return this._remoteComponentStatus;
  }

  public setCbRemoteComponentStatusChanged(cbRemoteComponentStatusChanged: (target: Netcut_Component_To_Component) => void): void {
    this._cbRemoteComponentStatusChanged = cbRemoteComponentStatusChanged;
  }
  
  protected pushNewActiveCallbackRequest(): CallbackRequest {
    this._activeCallbackOriginIdCounter++;
    let newRequest: CallbackRequest = new CallbackRequest(
      this._activeCallbackOriginIdCounter
    );
    this._activeCallbackedRequestsList.push(newRequest);
    
    return newRequest;
  }
  
  protected popActiveCallbackRequest(originID: number): CallbackRequest {
    for(let i: number=0;i<this._activeCallbackedRequestsList.length;i++) {
      let activeRequestEntry: CallbackRequest = this._activeCallbackedRequestsList[i];
      if(activeRequestEntry.originID == originID) {
        this._activeCallbackedRequestsList.splice(i, 1);
        
        return activeRequestEntry;
      }
    }
    
    return null;
  }
  
}
