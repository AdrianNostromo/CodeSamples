import { NetClientsHolderBase } from "./NetClientsHolderBase";

import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { BasicNetClient } from "./util/BasicNetClient";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { DataFormatError } from 'base/net/exception/DataFormatError';
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";
import { LogicError } from "base/exception/LogicError";
import { SecurityError } from "base/exception/SecurityError";
import { ILoopUserBase_General } from "base/component/loop/list/ILoopUserBase_General";
import { GlobalAccessLogger } from "base/log/GlobalAccessLogger";

class NetCodes {
  
  public static readonly SC_C_PING: number = 10010;
  public static readonly CS_R_PING_OK: number = 10011;

}
  
export class NetClientsHolderList<T extends BasicNetClient> extends NetClientsHolderBase implements ILoopUserBase_General {
  
  public _ILoopUserBase_General: true = true;
  
  private static readonly PING_RESPONSE_TIME_LIMIT_S: number = 3;

  private cbNewClient: (client: BasicNetClient) => void;
  private cbClientRemoved_pre: (client: BasicNetClient) => void;
  private cbProcessMsg: (client: T, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer) => boolean;
  // This callback is sent each tick.
  // The callback must check if ping is in progress.
  
  protected clientsList: T[] = [];

  public iddleDataReceiveThresholdS: number = 5;
  public interPingDelayS: number = 1;
  
  // Used for log purposes.
  public name: string = "";

  public constructor(
    cbNewClient: (client: BasicNetClient) => void,
    cbClientRemoved_pre: (client: BasicNetClient) => void,
    cbProcessMsg: (client: T, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer) => boolean
  ) {
    super();
    
    this.cbNewClient = cbNewClient;
    this.cbClientRemoved_pre = cbClientRemoved_pre;
    this.cbProcessMsg = cbProcessMsg;
  }

  public getClientsList(): T[] {
    return this.clientsList;
  }

  public getClientsCount(): number {
    return this.clientsList.length;
  }

  public addClient(client: T): void {
    this.clientsList.push(client);
    client.getCon().setCbClose(this.onConClose.bind(this));
    client.getCon().setCbError(this.onConError.bind(this));

    GlobalAccessLogger.getLog().debug('+NetClientsHolder(' + this.name + '). Client port: ' + client.remotePort + ' . Current list size: ' + this.clientsList.length);
    
    this.onNewClient(client);
  }

  public extractClientCon(client: T): GeneralTcpConnection {
    if(client.getCon() == null) {
      return null;
    }

    let con: GeneralTcpConnection = client.getCon();
    con.setCbClose(null);
    con.setCbError(null);
    
    client.setCon(null);
    
    return con;
  }

  protected onNewClient(client: T): void {
    if(this.getIsDisposed()) {
      return ;
    }
    
    if(this.cbNewClient != null) {
      this.cbNewClient(client);
    }
  }
  
  private onConClose(con: GeneralTcpConnection): void {
    if(this.getIsDisposed()) {
      return ;
    }
    
    let client: T = con.getUserData<T>("client");
    
    this.removeClient(client);
  }
  
  private onConError(con: GeneralTcpConnection): void {
    if(this.getIsDisposed()) {
      return ;
    }
    
    let client: T = con.getUserData<T>("client");
    
    this.removeClient(client);
  }
  
  protected removeClient(client: T): void {
    if(!client) {
      return ;
    }
    
    let index: number = this.clientsList.indexOf(client);
    if(index < 0) {
      GlobalAccessLogger.getLog().error('ServerComponentValidator. Error.');

      throw new LogicError('Error.');
    }

    this.onClientRemoved_pre(client);
    
    GlobalAccessLogger.getLog().debug('-NetClientsHolder(' + this.name + '). Client port: ' + client.remotePort + ' . Current list size: ' + this.clientsList.length);

    if(client.getCon() != null) {
      client.getCon().setCbClose(null);
      client.getCon().setCbError(null);
      
      client.getCon().forceClose();

      client.setCon(null);
    }
    
    this.clientsList.splice(index, 1);
  }

  protected onClientRemoved_pre(client: T): void {
    if(this.getIsDisposed()) {
      return ;
    }
    
    if(this.cbClientRemoved_pre != null) {
      this.cbClientRemoved_pre(client);
    }
  }
  
  public appLoop_General(deltaS: number, deltaMS: number): void {
    this.appLoop_net(deltaS, deltaMS);
  }
  
  private appLoop_net(deltaS: number, deltaMS: number): void {
    for(let i: number=this.clientsList.length - 1;i>=0;i--) {
      let client: T = this.clientsList[i];
      if(client != null) {
        if(client.forceDisconnectTimeoutS >= 0) {
          client.forceDisconnectTimeoutS -= deltaS;
          
          if(client.forceDisconnectTimeoutS <= 0) {
            this.removeClient(client);
            
            continue;
          }
        }

        if(client.currentCommandTimeoutS >= 0) {
          client.currentCommandTimeoutS -= deltaS;
          
          if(client.currentCommandTimeoutS <= 0) {
            this.removeClient(client);
            
            continue;
          }
        }
        if(client.getCon().isData()) {
          client.receiveIddleTimeS = 0;
          
          this.processPlayerConData(client);
        }else {
          client.receiveIddleTimeS += deltaS;
          
          if(client.receiveIddleTimeS > this.iddleDataReceiveThresholdS) {
            this.removeClient(client);
            
            continue;
          }
          
          if(client.isPingAllowed) {
            if(client.isPingInProgress) {
              client.activePingTimeoutS -= deltaS;
              if(client.activePingTimeoutS <= 0) {
                this.removeClient(client);
                
                continue;
              }
            }else {
              if(client.receiveIddleTimeS > this.interPingDelayS) {
                this.SC_C_PING(client);
              }
            }
          }
        }
      }
    }
  }

  private SC_C_PING(client: T): void {
    if(client.isPingInProgress) {
      throw new LogicError('Error.');
    }
    
    client.isPingInProgress = true;
    client.activePingTimeoutS = NetClientsHolderList.PING_RESPONSE_TIME_LIMIT_S;

    client.getCon().sendBlock(new NetMsgsBlock(1, 
      4)
      .startMsgWrite()
      .writeInt(NetCodes.SC_C_PING)
      .endMsgWrite()
    );
  }
    
  private processPlayerConData(client: T): void {
    while(true) {
      let msgNetBuffer: StreamBuffer;
      let msgLen: number;
      let msgType: number;
      
      // Make this check 2 times for better removal handling.
      if(client.markForRemoval) {
        this.removeClient(client);

        return ;
      }
      try {
        msgNetBuffer = client.getCon().popNextMsg();
      }catch(x) {
        if(x instanceof DataFormatError) {
          this.removeClient(client);
          
          return ;
        }else {
          this.removeClient(client);
          
          return ;
        }
      }
      if(msgNetBuffer == null) {
        return ;
      }
      
      try {
        msgLen = msgNetBuffer.readInt();
        msgType = msgNetBuffer.readInt();
      }catch(x) {
        this.removeClient(client);
        
        return ;
      }
      
      let isMsgHandled: boolean;
      try {
        isMsgHandled = this.onProcessMsg(client, msgLen, msgType, msgNetBuffer);
      }catch(x) {
        GlobalAccessLogger.getLog().security('Client message error A.' + msgType);

        this.removeClient(client);
        
        return ;
      }
      if(!isMsgHandled) {
        GlobalAccessLogger.getLog().security('Client message pare error B.' + msgType);

        this.removeClient(client);
        
        return ;
      }
      // Make this check 2 times for better removal handling.
      if(client.markForRemoval) {
        this.removeClient(client);

        return ;
      }
    }
  }

  protected onProcessMsg(client: T, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    // Check if the msg is a locally handled one.
    if(this.processMsg_local(client, msgLen, msgType, msgNetBuffer)) {
      return true;
    }

    if(this.cbProcessMsg != null) {
      return this.cbProcessMsg(client, msgLen, msgType, msgNetBuffer);
    }

    return false;
  }

  protected processMsg_local(client: T, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    switch (msgType) {
      case NetCodes.CS_R_PING_OK:
        if(!client.isPingInProgress) {
          throw new SecurityError("NetClientsHolderList.processMsg_local.1.");
        }
        
        client.isPingInProgress = false;

        break;
      default:
        return false;
    }
    
    return true;
  }

}
