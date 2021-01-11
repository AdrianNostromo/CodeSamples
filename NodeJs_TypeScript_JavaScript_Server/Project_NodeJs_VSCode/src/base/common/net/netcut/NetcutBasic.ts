import { IApp } from "base/app/IApp";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "base/common/ComponentType";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";
import { DataFormatError } from 'base/net/exception/DataFormatError';

export class NetcutBasic {

    private app: IApp;
  
  protected _con: GeneralTcpConnection;
  protected _componentType: ComponentType;
  
  protected _isServerLocally: boolean;

  private _cbError: (target: NetcutBasic) => void = null;
  
  private _receiveIddleTimeS: number = 0;
  protected _sendIddleTimeS: number = 0;
  
  private _disconnectIddleThresholdTimeS = 150000;//asd_tst_001; 15
  
  private _iripMap: Map<string, Object> = new Map();
  private _dataCache: Map<string, Object> = new Map();

  public constructor(app: IApp, con: GeneralTcpConnection, componentType: ComponentType, isServerLocally: boolean) {
    this.app = app;
    
    this._con = con;
    this._componentType = componentType;
    
    this._isServerLocally = isServerLocally;
  }
  
  public getApp(): IApp {
    return this.app;
  }
  
  public create(): void {
    this._con.setCbClose(this.onConError.bind(this));
    this._con.setCbError(this.onConError.bind(this));
    
    this.create_framework();

    this.processConData();
  }

  protected create_framework(): void {
    //void
  }
  
  public logicLoop(deltaS: number, deltaMS: number): void {
    this._receiveIddleTimeS += deltaS;
    if(this._receiveIddleTimeS >= this._disconnectIddleThresholdTimeS) {
      // Remote host is unresponsize or connection lost.
      this.handleConError();
    }
    
    if(this._con.isData()) {
      this.processConData();
    }
  }
  
  private onConError(con: GeneralTcpConnection): void {
    if(!con.getUserData<boolean>("isErrorHandled")) {
      con.setUserData<boolean>("isErrorHandled", true);
      
      this.handleConError();
    }
  }
  
  protected handleConError(): void {
    if(this._cbError) {
      this._cbError(this);
    }
  }
  
  public setCbError(cbError: (target: NetcutBasic) => void): void {
    this._cbError = cbError;
  }

  public processConData(): void {
    while(true) {
      let msgNetBuffer: StreamBuffer;
      let msgLen: number;
      let msgType: number;
      
      try {
        msgNetBuffer = this._con.popNextMsg();
      }catch(x) {
        if(x instanceof DataFormatError) {
          this.handleConError();
          
          return ;
        }else {
          this.handleConError();
          
          return ;
        }
      }
      if(!msgNetBuffer) {
        return ;
      }
      
      this._receiveIddleTimeS = 0;
      
      try {
        msgLen = msgNetBuffer.readInt();
        msgType = msgNetBuffer.readInt();
      }catch(err) {
        this.handleConError();
        
        return ;
      }
      
      let isMsgHandled: boolean;
      try {
        isMsgHandled = this.processMsg(msgLen, msgType, msgNetBuffer);
      }catch(x) {
        this.handleConError();
        
        return ;
      }
      if(!isMsgHandled) {
        this.handleConError();
      }else {
        // Check if all the data was read.
        if(msgNetBuffer.getOffset() != msgNetBuffer.getBoundsLimit()) {
          this.handleConError();
        }
      }
    }
  }
  
  protected processMsg(msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    return false;
  }

  protected getCon(): GeneralTcpConnection {
    return this._con;
  }
  
  public getComponentType(): ComponentType {
    return this._componentType;
  }
  
  protected sendNetBlock(netBlock: NetMsgsBlock): void {
    this._con.sendBlock(netBlock);
    
    this._sendIddleTimeS = 0;
  }
  
  public getIsServerLocally(): boolean {
    return this._isServerLocally;
  }
  
}
