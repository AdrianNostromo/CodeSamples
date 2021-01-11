import * as net from "net";

import { IpPort } from "base/net/util/IpPort";
import { AddressInfo } from "net";

export class TcpServer {

  private initIpPort: IpPort;
  private postInitIpPort: IpPort = null;

  public server: net.Server = null;
  
  private isListenerActive: boolean = false;
  private isListenerError: boolean = false;
  
  private cbNewSocket: (target: TcpServer, socket: net.Socket) => void = null;  
  private cbNetError: (target: TcpServer, error: Error) => void = null;
  private cbLogicError: (target: TcpServer, error: Error) => void = null;
  private cbListenerActive: (target: TcpServer) => void = null;
  
  public constructor(
    initIpPort: IpPort,

    cbNewSocket: (target: TcpServer, socket: net.Socket) => void, 
    cbNetError: (target: TcpServer, error: Error) => void,
    cbLogicError: (target: TcpServer, error: Error) => void,
    cbListenerActive: (target: TcpServer) => void
  ) {
    this.initIpPort = initIpPort;

    this.cbNewSocket = cbNewSocket;
    this.cbNetError = cbNetError;
    this.cbLogicError = cbLogicError;
    this.cbListenerActive = cbListenerActive;
  }
  
  public create(): void {
    this.server = net.createServer(this.onNewSocket.bind(this));

    this.server.on('error', this.onListenerError.bind(this));

    //511 is: Backlog is the maximum length of the queue of pending connections. The actual length will be determined by the OS through sysctl settings such as tcp_max_syn_backlog and somaxconn on Linux. The default value of this parameter is 511 (not 512).
    this.server.listen(this.initIpPort.port, this.initIpPort.ip, 511, this.onServerActive.bind(this));
  }
  
  private onNewSocket(socket: net.Socket): void {
    if(this.cbNewSocket != null) {
      this.cbNewSocket(this, socket);
    }
  }

  private onListenerError(err: Error): void {
    this.isListenerError = true;
    this.isListenerActive = false;
    
    if(this.cbNetError != null) {
      this.cbNetError(this, err);
    }
  }
  
  private onServerActive(): void {
    this.isListenerActive = true;

    let data: {port: number, family: string, address: string} = <AddressInfo>this.server.address();
    
    this.postInitIpPort = new IpPort(
      data.address,
      data.port
    );

    if(this.cbListenerActive != null) {
      this.cbListenerActive(this);
    }
  }
  
  public getIsListenerActive(): boolean {
    return this.isListenerActive;
  }
  
  public getIsListenerError(): boolean {
    return this.isListenerError;
  }
  
  public getIpPort(): IpPort {
    if(!this.server) {
      throw new Error('Error.');
    }
    
    return this.postInitIpPort;
  }
  
  public dispose(): void {
    this.cbNetError = null;
    this.cbLogicError = null;
    this.cbNewSocket = null;

    if(this.server != null) {
      this.server.close();
      
      this.server = null;
    }
  }
  
}
