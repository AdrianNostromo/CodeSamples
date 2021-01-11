import { BasicAppComponent } from "base/component/BasicAppComponent";

import { IApp } from "base/app/IApp";
import { ComponentType } from "base/common/ComponentType";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import * as net from "net";

class ConnectingData {
  
  public remoteIP: string;
  public remotePort: number;
  public remoteComponentType: ComponentType;
  public cbConnected: (conTrackID: number, con: GeneralTcpConnection, remoteComponentType: ComponentType) => void;
  public cbFailed: (con: GeneralTcpConnection, conTrackID: number, remoteComponentType: ComponentType) => void;
  
  public con: GeneralTcpConnection;
  public conTrackID: number;

  public constructor(
    remoteIP: string,
    remotePort: number,
    remoteComponentType: ComponentType,
    cbConnected: (conTrackID: number, con: GeneralTcpConnection, remoteComponentType: ComponentType) => void,
    cbFailed: (con: GeneralTcpConnection, conTrackID: number, remoteComponentType: ComponentType) => void,

    con: GeneralTcpConnection,
    conTrackID: number
  ) {
    this.remoteIP = remoteIP;
    this.remotePort = remotePort
    this.remoteComponentType = remoteComponentType;
    this.cbConnected = cbConnected;
    this.cbFailed = cbFailed;

    this.con = con;
    this.conTrackID = conTrackID;
  }

}

export class TcpConnector extends BasicAppComponent {

  private _waitingConsList: GeneralTcpConnection[] = [];
  
  private _conTrackIDCounter: number = 0;
  
  public constructor(app: IApp) {
    super(app);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();

    //void
  }
  
  public newCon(
    remoteIP: string, 
    remotePort: number, 
    remoteComponentType: ComponentType, 
    cbConnected: (conTrackID: number, con: GeneralTcpConnection, remoteComponentType: ComponentType) => void, 
    cbFailed: (con: GeneralTcpConnection, conTrackID: number, remoteComponentType: ComponentType) => void
  ): GeneralTcpConnection {
    let newCon: GeneralTcpConnection = new GeneralTcpConnection(new net.Socket());
    newCon.setCbConnect(this._onConnectionEstablished.bind(this));
    newCon.setCbClose(this._onConnectionFailed.bind(this));
    newCon.setCbError(this._onConnectionFailed.bind(this));
    
    let connectingData: ConnectingData = new ConnectingData(
      remoteIP,
      remotePort,
      remoteComponentType,
      cbConnected,
      cbFailed,
      
      newCon,
      ++this._conTrackIDCounter
    );
    newCon.setUserData<ConnectingData>("connectingDataconnectingData", connectingData);
    
    newCon.getSocket().connect(remotePort, remoteIP);
    
    this._waitingConsList.push(newCon);
    
    return newCon;
  }

  private _onConnectionEstablished(con: GeneralTcpConnection): void {
    let index: number = this._waitingConsList.indexOf(con);
    if (index < 0) {
      this.getApp().getLog().error('TcpConnector. Critical error.');
      
      this.getApp().getErrorHandler().error('TcpConnector. Critical error.');
      
      return ;
    }
    
    this._waitingConsList.splice(index, 1);
    
    let connectingData: ConnectingData = con.getUserData<ConnectingData>("connectingDataconnectingData");
    con.setUserData<ConnectingData>("connectingDataconnectingData", null);
    
    // Sanitize con.
    con.setCbConnect(null);
    con.setCbClose(null);
    con.setCbError(null);
    
    connectingData.cbConnected(connectingData.conTrackID, connectingData.con, connectingData.remoteComponentType);
  }

  private _onConnectionFailed(con: GeneralTcpConnection): void {
    let index: number = this._waitingConsList.indexOf(con);
    if (index < 0) {
      // Already handled by a different fail event.
      
      return ;
    }
    
    this._waitingConsList.splice(index, 1);
    
    let connectingData: ConnectingData = con.getUserData<ConnectingData>("connectingDataconnectingData");
    con.setUserData<ConnectingData>("connectingDataconnectingData", null);
    
    // Sanitize con.
    con.setCbConnect(null);
    con.setCbClose(null);
    con.setCbError(null);
    
    connectingData.cbFailed(con, connectingData.conTrackID, connectingData.remoteComponentType);
  }

}
