import { BasicAppComponent } from "base/component/BasicAppComponent";

import { IApp } from "base/app/IApp";

import { DataFormatError } from 'base/net/exception/DataFormatError';
import { ComponentType } from "base/common/ComponentType";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { StreamBuffer } from "base/buffer/StreamBuffer";

import { NetCodes_SC_UnvalidatedServerComponent_To_UnvalidatedClientComponent } from "base/common/net/netcodes/NetCodes_SC_UnvalidatedServerComponent_To_UnvalidatedClientComponent";
import { NetCodes_CS_UnvalidatedClientComponent_To_UnvalidatedServerComponent } from "base/common/net/netcodes/NetCodes_CS_UnvalidatedClientComponent_To_UnvalidatedServerComponent";
import { ILoopUserBase_General } from "base/component/loop/list/ILoopUserBase_General";

class NetConValidationData {

  public cbValidated: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void;
  public cbValidationFailed: (con: GeneralTcpConnection) => void;

  public receiveIddleTimeS: number = 0

  public constructor(
    cbValidated: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void, 
    cbValidationFailed: (con: GeneralTcpConnection) => void
  ) {
    this.cbValidated = cbValidated;
    this.cbValidationFailed = cbValidationFailed;
  }

}

export class ClientComponentValidator extends BasicAppComponent implements ILoopUserBase_General {
  
  public _ILoopUserBase_General: true = true;
  
  private _unconfirmedComponentCons: GeneralTcpConnection[] = [];
  
  private _receiveIddleConnectionTimeLimitS = 5000;//asd_tst_002;// 5
  
  public constructor(app: IApp) {
    super(app);
    
    //void
  }
  
  public appLoop_General(deltaS: number, deltaMS: number): void {
    for(let i: number=this._unconfirmedComponentCons.length-1;i>=0;i--) {
      let con: GeneralTcpConnection = this._unconfirmedComponentCons[i];
      if(con) {
        if(con.isData()) {
          this.processConData(con);
        }
      }
    }
    
    for(let i: number=this._unconfirmedComponentCons.length-1;i>=0;i--) {
      let con: GeneralTcpConnection = this._unconfirmedComponentCons[i];
      if(con) {
        con.getUserData<NetConValidationData>("validationData").receiveIddleTimeS += deltaS;
        
        if(con.getUserData<NetConValidationData>("validationData").receiveIddleTimeS >= this._receiveIddleConnectionTimeLimitS) {
          this.connectionErrorOccured(con);
        }
      }
    }
  }
  
  public handleNewUnconfirmedComponent(
    con: GeneralTcpConnection, 
    cbValidated: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void, 
    cbValidationFailed: (con: GeneralTcpConnection) => void): void 
  {
    let validationData: NetConValidationData = new NetConValidationData(
      cbValidated,
      cbValidationFailed
    );
    con.setUserData<NetConValidationData>("validationData", validationData);
    
    this._unconfirmedComponentCons.push(con);
    
    con.setCbClose(this.onConDataError.bind(this));
    con.setCbError(this.onConDataError.bind(this));
    
    if(!this.sendMsg_REQUEST_COMPONENT_CONFIG(con)) {
      // Handle con error.
      this.connectionErrorOccured(con);
      
      return ;
    }
  }

  private sendMsg_REQUEST_COMPONENT_CONFIG(con: GeneralTcpConnection): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4 + 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_SC_UnvalidatedServerComponent_To_UnvalidatedClientComponent.REQUEST_COMPONENT_CONFIG);
      netBlock.writeInt(this.getApp().getComponentType().id);
      // END : msg data;
      netBlock.endMsgWrite();
      
      con.sendBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }

  private processConData(con: GeneralTcpConnection): void {
    while(true) {
      let msgNetBuffer: StreamBuffer;
      let msgLen: number;
      let msgType: number;
      
      try {
        msgNetBuffer = con.popNextMsg();
      }catch(x) {
        if(x instanceof DataFormatError) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          this.connectionErrorOccured(con);
          
          return ;
        }else {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          this.connectionErrorOccured(con);
          
          return ;
        }
      }
      if(!msgNetBuffer) {
        return ;
      }
      
      con.getUserData<NetConValidationData>("validationData").receiveIddleTimeS = 0;
      
      try {
        msgLen = msgNetBuffer.readInt();
        msgType = msgNetBuffer.readInt();
      }catch(err) {
        this.getApp().getLog().error('ServerComponentValidator. Logic error.');

        this.connectionErrorOccured(con);
        
        return ;
      }
      
      let isMsgHandled: boolean;
      try {
        isMsgHandled = this.processMsg(con, msgLen, msgType, msgNetBuffer);
      }catch(x) {
        this.getApp().getLog().error('ClientComponentValidator. Unhandled message type: ' + msgType);

        this.connectionErrorOccured(con);
        
        return ;
      }
      if(!isMsgHandled) {
        this.getApp().getLog().error('ClientComponentValidator. Unhandled message type: ' + msgType);

        this.connectionErrorOccured(con);
        
        return ;
      }
    }
  }

  protected processMsg(con: GeneralTcpConnection, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    switch (msgType) {
      case NetCodes_CS_UnvalidatedClientComponent_To_UnvalidatedServerComponent.RESPONSE_COMPONENT_CONFIG:
        let remoteComponentTypeID: number;
        
        try {
          remoteComponentTypeID = msgNetBuffer.readInt();
        }catch(err) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        let remoteComponentType: ComponentType = ComponentType.fromID(remoteComponentTypeID);
        if(!remoteComponentType) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        // Component is valid.
        let index: number = this._unconfirmedComponentCons.indexOf(con);
        if (index < 0) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        this._unconfirmedComponentCons.splice(index, 1);
        
        let validationData: NetConValidationData = con.getUserData<NetConValidationData>("validationData");
        
        this.sanitizeCon(con);
        
        validationData.cbValidated(con, remoteComponentType);
        
        break;
      default:
        return false;
    }
    
    return true;
  }

  private onConDataError(con: GeneralTcpConnection): void {
    this.connectionErrorOccured(con);
  }
  
  private connectionErrorOccured(con: GeneralTcpConnection): void {
    // The component validation failed.
    
    // Remove the con from the list. If the con is not in the list, it is already removed and return.
    let index: number = this._unconfirmedComponentCons.indexOf(con);
    if (index < 0) {
      // Already handled by a similar event.
      
      return ;
    }
    this._unconfirmedComponentCons.splice(index, 1);
    
    let validationData: NetConValidationData = con.getUserData<NetConValidationData>("validationData");
    
    this.sanitizeCon(con);
    
    validationData.cbValidationFailed(con);
  }

  private sanitizeCon(con: GeneralTcpConnection): void {
    con.setUserData<NetConValidationData>("validationData", null);
    
    con.setCbClose(null);
    con.setCbError(null);
  }
  
}
