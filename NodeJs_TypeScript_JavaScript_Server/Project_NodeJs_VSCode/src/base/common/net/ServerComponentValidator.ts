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

class ValidatingConData {

  public remoteComponentType: ComponentType;

  public cbValidated: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void;
  public cbFailed: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void;

  public receiveIddleTimeS: 0;

  public constructor(
    remoteComponentType: ComponentType,
    cbValidated: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void,
    cbFailed: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void
  ) {
    this.remoteComponentType = remoteComponentType;

    this.cbValidated = cbValidated;
    this.cbFailed = cbFailed;
  }

}

export class ServerComponentValidator extends BasicAppComponent implements ILoopUserBase_General {

  public _ILoopUserBase_General: true = true;
  
  private _unconfirmedComponentCons: GeneralTcpConnection[] = [];

  private _receiveIddleConnectionTimeLimitS: number = 500000;//asd_tst_001; 5

  public constructor(app: IApp) {
    super(app);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();

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
        con.getUserData<ValidatingConData>("validatingData").receiveIddleTimeS += deltaS;
        
        if(con.getUserData<ValidatingConData>("validatingData").receiveIddleTimeS >= this._receiveIddleConnectionTimeLimitS) {
          this.connectionErrorOccured(con);
        }
      }
    }
  }
  
  public handleNewUnconfirmedComponent(
    con: GeneralTcpConnection, 
    remoteComponentType: ComponentType, 
    cbValidated: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void, 
    cbFailed: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void
  ): void {
    con.setUserData<ValidatingConData>("validatingData", new ValidatingConData(remoteComponentType, cbValidated, cbFailed));
    
    this._unconfirmedComponentCons.push(con);
    
    con.setCbClose(this.onConnectionError.bind(this));
    con.setCbError(this.onConnectionError.bind(this));
    
    this.processConData(con);
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
          this.connectionErrorOccured(con);
          
          return ;
        }else {
          this.connectionErrorOccured(con);
          
          return ;
        }
      }
      if(!msgNetBuffer) {
        return ;
      }
      
      con.getUserData<ValidatingConData>("validatingData").receiveIddleTimeS = 0;
      
      try {
        msgLen = msgNetBuffer.readInt();
        msgType = msgNetBuffer.readInt();
      }catch(err) {
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
        this.connectionErrorOccured(con);
        
        return ;
      }
    }
  }

  protected processMsg(con: GeneralTcpConnection, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    switch (msgType) {
      case NetCodes_SC_UnvalidatedServerComponent_To_UnvalidatedClientComponent.REQUEST_COMPONENT_CONFIG:
        let remoteComponentTypeID: number = msgNetBuffer.readInt();
        
        let remoteComponentType: ComponentType = ComponentType.fromID(remoteComponentTypeID);
        if(!remoteComponentType) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        if(remoteComponentType != con.getUserData<ValidatingConData>("validatingData").remoteComponentType) {
          // The remote component is not of the expected type.
          
          this.getApp().getLog().error('ServerComponentValidator. Logic error 2.');

          return false;
        }
        
        if(!this.sendMsg_RESPONSE_COMPONENT_CONFIG(con)) {
          // Handle con error.
          
          this.getApp().getLog().error('ServerComponentValidator. Logic error 2.');

          return false;
        }
        
        this.validationOccured(con);
        
        break;
      default:
        return false;
    }
    
    return true;
  }
  
  private sendMsg_RESPONSE_COMPONENT_CONFIG(con: GeneralTcpConnection): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4 + 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_CS_UnvalidatedClientComponent_To_UnvalidatedServerComponent.RESPONSE_COMPONENT_CONFIG);
      netBlock.writeInt(this.getApp().getComponentType().id);
      // END : msg data;
      netBlock.endMsgWrite();
      
      con.sendBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }

  private validationOccured(con: GeneralTcpConnection): void {
    let index: number = this._unconfirmedComponentCons.indexOf(con);
    if (index < 0) {
      // Already handled by a different fail event.
      
      return ;
    }
    
    this._unconfirmedComponentCons.splice(index, 1);
    
    let validatingData: ValidatingConData = con.getUserData<ValidatingConData>("validatingData");
    
    this.sanitizeCon(con);
    
    validatingData.cbValidated(con, validatingData.remoteComponentType);
  }

  private onConnectionError(con: GeneralTcpConnection): void {
    this.connectionErrorOccured(con);
  }
  
  private connectionErrorOccured(con: GeneralTcpConnection): void {
    let index: number = this._unconfirmedComponentCons.indexOf(con);
    if (index < 0) {
      // Already handled by a different fail event.
      
      return ;
    }
    
    this._unconfirmedComponentCons.splice(index, 1);
    
    let validatingData: ValidatingConData = con.getUserData<ValidatingConData>("validatingData");
    
    this.sanitizeCon(con);
    
    validatingData.cbFailed(con, validatingData.remoteComponentType);
  }

  private sanitizeCon(con: GeneralTcpConnection): void {
    con.setUserData<ValidatingConData>("validatingData", null);
    
    con.setCbClose(null);
    con.setCbError(null);
  }

}
