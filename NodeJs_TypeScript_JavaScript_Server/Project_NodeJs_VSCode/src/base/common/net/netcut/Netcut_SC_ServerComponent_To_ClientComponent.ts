import { Netcut_Component_To_Component } from "./Netcut_Component_To_Component";

import { IApp } from "base/app/IApp";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "base/common/ComponentType";

import { StreamBuffer } from "base/buffer/StreamBuffer";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";

import { NetCodes_SC_ServerComponent_To_ClientComponent } from "base/common/net/netcodes/NetCodes_SC_ServerComponent_To_ClientComponent";
import { NetCodes_CS_ClientComponent_To_ServerComponent } from "base/common/net/netcodes/NetCodes_CS_ClientComponent_To_ServerComponent";

export class Netcut_SC_ServerComponent_To_ClientComponent extends Netcut_Component_To_Component {

  private static readonly _pingSendIddleDelayS: number = 5;

  public constructor(app: IApp, con: GeneralTcpConnection, componentType: ComponentType) {
    super(app, con, componentType, true);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    //void
  }
  
  public logicLoop(deltaS: number, deltaMS: number): void {
    super.logicLoop(deltaS, deltaMS);
    
    // Handle ping sending.
    
    this._sendIddleTimeS += deltaS;
    if(this._sendIddleTimeS >= Netcut_SC_ServerComponent_To_ClientComponent._pingSendIddleDelayS) {
      this._sendIddleTimeS = 0;

      // Send a ping.
      if(!this.sendMsg_REQUEST_PING()) {
        this.handleConError();
      }
    }
  }
  
  protected processMsg(msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    if(super.processMsg(msgLen, msgType, msgNetBuffer)) {
      return true;
    }
    
    switch (msgType) {
      case NetCodes_CS_ClientComponent_To_ServerComponent.RESPONSE_PING_OK:
        //void
        
        break;
      default:
        return false;
    }
    
    return true;
  }
  
  private sendMsg_REQUEST_PING(): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_SC_ServerComponent_To_ClientComponent.REQUEST_PING);
      // END : msg data;
      netBlock.endMsgWrite();
      
      this.sendNetBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }
  
}
